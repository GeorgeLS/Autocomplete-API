CC = gcc
CFLAGS = -Wall -ggdb

bin: queue.o test.o trie.o utils.o 
	$(CC) $(CFLAGS) queue.o test.o trie.o utils.o -o autocomplete 

queue.o : queue.c macros.h 
	$(CC) $(CFLAGS) -c queue.c 

test.o : test.c trie.h 
	$(CC) $(CFLAGS) -c test.c 

trie.o : trie.c macros.h utils.h queue.h 
	$(CC) $(CFLAGS) -c trie.c 

utils.o : utils.c utils.h macros.h 
	$(CC) $(CFLAGS) -c utils.c 

.PHONY : clear

clear :
	rm -f autocomplete queue.o test.o trie.o utils.o 


#Generated with makefile generator: https://github.com/GeorgeLS/Makefile-Generator/blob/master/mfbuilder.c
