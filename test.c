#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "trie.h"
#include "utils.h"

int main(void) {
  trie_t* root = create_trie();
  root = insert(root, L"test");
  root = insert(root, L"testicles");
  root = insert(root, L"back");
  root = insert(root, L"bad");
  print_trie(root);
  completions_t* comp = get_completions(root, L"test");
  wchar_t** completions = get_completions_array(comp);
  size_t n = get_completions_number(comp);
  printf("\nCOMPLETIONS...\n");
  for (size_t i = 0U; i != n; ++i) {
    printf("%S\n", completions[i]);
  }
  for (size_t i = 0U; i != n; ++i) {
    stb_sb_free(completions[i]);
  }
  free(completions);
  delete_trie(root);
  return EXIT_SUCCESS;
}