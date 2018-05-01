# An autocomplete API in C just in case you need one for your app

Check "trie.h" for the API. Basically you have a trie in which you insert your words and whenever
you want to get completions/suggestions you just call get_completions passing the trie and the pattern to complete.
Build with makefile to test it. Report any bugs to sdi1600094@di.uoa.gr