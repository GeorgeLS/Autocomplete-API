#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "macros.h"
#include "utils.h"
#include "queue.h"

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

trie_t* create_trie(void) {
  return create_trie_node('\0');
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
      if (current == NULL) {
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
  if (root->end_of_word) (*completions)++;
  if (root->sibling != NULL) count_completions(root->sibling, completions);
  if (root->child != NULL) count_completions(root->child, completions);
}

static void push_completions(const trie_t* root, completions_t* completions, size_t index) {
  stb_sb_push(completions->completions[index], root->letter);
  if (root->end_of_word) stb_sb_push(completions->completions[index], '\0');
  if (root->sibling != NULL) push_completions(root->sibling, completions, index + 1);
  if (root->child != NULL) push_completions(root->child, completions, index);
}

static inline _FORCE_INLINE void push_pattern(completions_t* completions,
                                          size_t n, wchar_t* pattern) {
  size_t pattern_length = wcslen(pattern); 
  for (size_t i = 0U; i != n; ++i) {
    for (size_t j = 0U; j != pattern_length; ++j) {
      stb_sb_push(completions->completions[i], pattern[j]);
    }
  }
}

completions_t* get_completions(const trie_t* root, wchar_t* pattern) {
  if (root == NULL || pattern == NULL) {
    return NULL;
  }
  const trie_t* pattern_end = search(root, pattern);
  if (pattern_end == NULL || pattern_end->child == NULL) return NULL;
  size_t completions_number = 0U;
  count_completions(pattern_end, &completions_number);
  completions_t* completions = MALLOC(1, completions_t);
  completions->completions = CALLOC(completions_number, wchar_t*);
  completions->completions_number = completions_number;
  push_pattern(completions, completions_number, pattern);
  push_completions(pattern_end->child, completions, 0U);
  return completions;
}

void print_trie_r(trie_t* root, queue_t* queue) {
  queue = enqueue(queue, root->letter);
  queue_t* aux = NULL;
  if (root->end_of_word) {
    aux = copy_to_the_end(queue);
    while (queue != NULL) {
      queue = dequeue(queue);
    }
    putchar('\n');
  }
  if (root->sibling != NULL) {
    queue_t* queue_to_copy = (get_next(get_start(queue))) ? queue : NULL;
    aux = (aux == NULL) ? copy_before_end(queue_to_copy) : aux;
    print_trie_r(root->sibling, aux);
  }
  if (root->child != NULL) {
    queue_t* queue_to_pass = (queue) ? queue : aux;
    print_trie_r(root->child, queue_to_pass);
  }
  delete_queue(aux);
}

void print_trie(trie_t* root) {
  if (root == NULL) return;
  queue_t* queue = create_queue();
  print_trie_r(root->child, queue);
  delete_queue(queue);
}

wchar_t** get_completions_array(completions_t* completions) {
  return completions->completions;
}

size_t get_completions_number(completions_t* completions) {
  return completions->completions_number;
}