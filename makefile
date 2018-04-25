CC = gcc
CFLAGS = -Wall -ggdb

bin: trie.o utils.o 
	$(CC) $(CFLAGS) trie.o utils.o -o autocomplete 

trie.o : trie.c macros.h utils.h 
	$(CC) $(CFLAGS) -c trie.c 

utils.o : utils.c macros.h 
	$(CC) $(CFLAGS) -c utils.c 

.PHONY : clear

clear :
	rm -f autocomplete trie.o utils.o 


#Generated with makefile generator: https://github.com/GeorgeLS/Makefile-Generator/blob/master/mfbuilder.c
