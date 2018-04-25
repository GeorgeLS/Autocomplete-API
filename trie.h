#if !defined(__TRIE_HEADER__)
#define __TRIE_HEADER__

#include <stdlib.h>
#include <wchar.h>

typedef struct trie_node trie_t;
typedef struct completions completions_t;

/* Allocate memory for a trie and return a pointer to the root */
trie_t* create_trie(void);

/* Construct a trie from a list of words and returns it */
trie_t* construct_trie(wchar_t* const* word_list, size_t length);

/* Inserts a new key(word) in the trie and returns the new state of the trie */
trie_t* insert(trie_t* root, wchar_t* key);

/* Prints all the words in the trie */
void print_trie(trie_t* root);

/* Deletes the trie */
void delete_trie(trie_t* root);

completions_t* get_completions(const trie_t* root, const wchar_t* pattern);
wchar_t**  get_completions_array(completions_t* completions);
size_t get_completions_number(completions_t* completions);

#endif