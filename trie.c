#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "macros.h"
#include "utils.h"

typedef struct completions {
  wchar_t** completions;
  size_t completions_number;
} completions_t;

typedef struct trie_node {
  struct trie_node* child;
  struct trie_node* sibling;
  wchar_t letter;
  bool end_of_word;
} trie_t;

static inline _FORCE_INLINE
trie_t* create_trie_node(wchar_t letter) {
  trie_t* new_trie_node = MALLOC(1, trie_t);
  if (new_trie_node == NULL) {
    fatal("Couldn't allocate memory for a trie node\nExiting");
  }
  new_trie_node->child = new_trie_node->sibling = NULL;
  new_trie_node->letter = letter;
  new_trie_node->end_of_word = false;
  return new_trie_node;
}

trie_t* insert(trie_t* root, const wchar_t* key) {
  if (root == NULL) {
    root = create_trie_node('\0');
  }
  trie_t* father = root;
  trie_t* current = root->child;
  size_t key_length = wcslen(key);
  for (size_t i = 0U; i != key_length; ++i) {
    if (current == NULL) {
      current = create_trie_node(key[i]);
      father->child = current;      
    } else if (current->letter != key[i]) {
      father = current;
      current = current->sibling;
      while (current != NULL && current->letter != key[i]) {
        father = current;
        current = current->sibling;
      }
      if (current != NULL) {
        current = create_trie_node(key[i]);
        father->sibling = current;
      }
    }
    father = current;
    current = current->child;
  }
  father->end_of_word = true;
  return root;
}

trie_t* construct_trie(wchar_t* const* word_list, size_t length) {
  trie_t* new_trie;
  for (size_t i = 0U; i != length; ++i) {
    new_trie = insert(new_trie, word_list[i]);
  }
  return new_trie;
}

void delete_trie(trie_t* root) {
  if (root->sibling != NULL) {
    delete_trie(root->sibling);
  }
  if (root->child != NULL) {
    delete_trie(root->child);
  }
  free(root);
}

static inline _FORCE_INLINE
const trie_t* search(const trie_t* root, const wchar_t* pattern) {
  if (root == NULL || pattern == NULL) {
    return NULL;
  }
  size_t pattern_length = wcslen(pattern);
  const trie_t* father = root;
  const trie_t* current = root->child;
  for (size_t i = 0U; i != pattern_length; ++i) {
    if (current == NULL) return NULL;
    else if (current->letter != pattern[i]) {
      father = current;
      current = current->sibling;
      while (current != NULL && current->letter != pattern[i]) {
        father = current;
        current = current->sibling;
      }
      if (current == NULL) return NULL;
    }
    father = current;
    current = current->child;
  }
  return ((father != NULL && father->end_of_word) ? father : NULL);
}

static void count_completions(const trie_t* root, size_t* completions) {
  if (root->end_of_word) ++(*completions);
  if (root->sibling != NULL) count_completions(root->sibling, completions);
  if (root->child != NULL) count_completions(root->child, completions);
}

static void push_completions(const trie_t* root, BufHdr** completions, size_t index) {
  buf_push(completions[index], root->letter);
  if (root->end_of_word) buf_push(completions[index], '\0');
  if (root->sibling != NULL) push_completions(root->sibling, completions, index + 1);
  if (root->child != NULL) push_completions(root->child, completions, index);
}

static inline _FORCE_INLINE void init_completions(completions_t* completions) {
  completions->completions = NULL;
  completions->completions_number = 0U;
}

completions_t* get_completions(const trie_t* root, const wchar_t* pattern) {
  if (root == NULL || pattern == NULL) {
    return NULL;
  }
  const trie_t* pattern_end = search(root, pattern);
  if (pattern_end == NULL || pattern_end->child == NULL) return NULL;
  size_t completions_number;
  count_completions(pattern_end, &completions_number);
  completions_t* completions = MALLOC(1, completions_t);
  init_completions(completions);
  completions->completions = MALLOC(completions_number, wchar_t*);
  completions->completions_number = completions_number;
  BufHdr** aux_sbuffers = MALLOC(completions_number, BufHdr*);
  for (size_t i = 0U; i != completions_number; ++i) {
    aux_sbuffers[i] = MALLOC(1, BufHdr);
  }
  push_completions(pattern_end->child, aux_sbuffers, 0U);
  size_t pattern_length = wcslen(pattern);
  for (size_t i = 0U; i != completions_number; ++i) {
    completions->completions[i] = MALLOC(pattern_length + buf_len(aux_sbuffers[i]) + 1, wchar_t);
    wmemcpy(completions->completions[i], aux_sbuffers[i]->buf, buf_len(aux_sbuffers[i]));
  }
  for (size_t i = 0U; i != completions_number; ++i) {
    buf_free(aux_sbuffers[i]);
  }
  free(aux_sbuffers);
  return completions;
}