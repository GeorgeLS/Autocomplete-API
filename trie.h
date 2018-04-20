#if !defined(__TRIE_HEADER__)
#define __TRIE_HEADER__

#include <stdlib.h>
#include <wchar.h>

typedef struct trie_node trie_t;

/* Construct a trie from a list of words and returns it */
trie_t* construct_trie(wchar_t* const* word_list, size_t length);

/* Inserts a new key(word) in the trie and returns the new state of the trie */
trie_t* insert(trie_t* root, wchar_t* key);

/* Deletes the trie */
void delete_trie(trie_t* root);

#endif