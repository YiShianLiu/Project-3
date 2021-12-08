all: webSearch

TrieNode: TrieNode.c TrieNode.h
	gcc -c TrieNode.c -o TrieNode.o
TFIDF: TFIDF.c TFIDF.h 
	gcc -c TFIDF.c -o TFIDF.o
webSearch: webSearch.c TrieNode.o TFIDF.o
	gcc TrieNode.o TFIDF.o webSearch.c -lm -o webSearch