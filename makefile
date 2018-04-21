CC = gcc
CFLAGS = -Wall -ggdb

bin: queue.o trie.o utils.o 
	$(CC) $(CFLAGS) queue.o trie.o utils.o -o autocomplete 

queue.o : queue.c utils.h macros.h 
	$(CC) $(CFLAGS) -c queue.c 

trie.o : trie.c macros.h utils.h queue.h 
	$(CC) $(CFLAGS) -c trie.c 

utils.o : utils.c macros.h 
	$(CC) $(CFLAGS) -c utils.c 

.PHONY : clear

clear :
	rm -f autocomplete queue.o trie.o utils.o 


#Generated with makefile generator: https://github.com/GeorgeLS/Makefile-Generator/blob/master/mfbuilder.c
