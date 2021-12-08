#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "TrieNode.h"
#include "TFIDF.h"


double getIDF(char* token, struct listNode *root){
  struct listNode *curr = root;
  int wordInDocument = 1;
  int documentCount = 1;
  double IDF;
  while(curr != NULL){
    if(isInPage(curr->addr, token) != 0){
      wordInDocument++;
    }
    documentCount++;
    curr = curr->next;
  }
  IDF = (double)documentCount/(double)wordInDocument;
  return log(IDF);
}
int isInPage(char* url, char* token){
  char buffer[300000];
	char newBuffer[300000];
	int newBuffercount = 0;

	getText(url, buffer, 300000);
	for(int i = 0; i < 300000 && buffer[i] != '\0'; i++){
		char temp = buffer[i];
		if(buffer[i] >= 65 && buffer[i] <= 90){
			temp += 32;
			newBuffer[newBuffercount] = temp;
			newBuffercount++;
		}
		else if(buffer[i] >= 97 && buffer[i] <= 122){
		  newBuffer[newBuffercount] = temp;
		  newBuffercount++;
		}
		else{
			if(newBuffer[newBuffercount - 1] != '\n'){
				newBuffer[newBuffercount] = '\n';
				newBuffercount++;
			}
		}
	}
	newBuffer[newBuffercount] = '\0';
  int wordCount = 0;
  char *tok = strtok(newBuffer, "\n");
  while(tok != NULL){
    if(strcmp(tok,token) == 0){
      wordCount++;
    }
    tok = strtok(NULL, "\n");
  }
  return wordCount;
}
double getOneScore(char* token, struct listNode *root, char* url){
  double TF;
  double IDF = getIDF(token, root->next);
  struct TrieNode *trieroot = init_trie();
  indexPage(url, trieroot, 0);
  double total = getWordCount(trieroot);
  double freq = isInPage(url, token);
  TF = freq/total;
  return TF*IDF;
}