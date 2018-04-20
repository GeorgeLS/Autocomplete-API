#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "macros.h"
#include "utils.h"
#include "queue.h"

#define MALLOC(size, type) ((type*) malloc((size) * sizeof(type)))

typedef struct trie_node {
  struct trie_node* child;
  struct trie_node* sibling;
  wchar_t letter;
  bool end_of_word;
} trie_t;

static inline _FORCE_INLINE trie_t* create_trie_node(wchar_t letter) {
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
  size_t key_length = strlen(key);
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
    new_trie = insert(word_list[i]);
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

whcar_t** get_completions(trie_t* root, const wchar_t* pattern) {

}