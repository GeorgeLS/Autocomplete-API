#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

#include "macros.h"
#include "utils.h"
#include "queue.h"

typedef struct trie_node {
  struct trie_node* child;
  struct trie_node* sibling;
  wchar_t letter;
  bool end_of_word;
} trie_t;

static inline _FORCE_INLINE
size_t _strlen(const wchar_t* str) {
  size_t length = 0U;
  while (*str++ != '\0') {
    ++length;
  }
  return length;
}

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
  size_t key_length = _strlen(key);
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
  size_t pattern_length = _strlen(pattern);
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

wchar_t** get_completions(const trie_t* root, const wchar_t* pattern) {
  if (root == NULL || pattern == NULL) {
    return NULL;
  }
  const trie_t* pattern_end = search(root, pattern);
  if (pattern_end == NULL) return NULL;
  size_t completions_number;
  count_completions(pattern_end, &completions_number);
  wchar_t** completions = MALLOC(completions_number, wchar_t*);
}