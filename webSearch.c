#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "TrieNode.h"
#include "TFIDF.h"

int main(int argc, char** argv){
  
  int max_hop;
  long seed;

  if(argc < 3 || argc > 4){
    fprintf(stderr, "USAGE: %s file max_hop seed\n", argv[0]);
    return -1;
  }
  
  
  if(argc >= 4){
    max_hop = atoi(argv[2]);
    seed = atol(argv[3]);
  }
  else{
    seed = time(NULL);
  }

  srand(seed);
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  char* url;
  int hop;
  
  struct listNode *root = malloc(sizeof(struct listNode));;
  char* rootString = "Root";
  strncpy(root->addr, rootString, strlen(rootString)+1);
  root -> next = NULL;
  printf("Indexing...\n");

  FILE *fp = fopen(argv[1], "r");
  while((read = getline(&line, &len, fp)) != -1){
    struct listNode *pListStart, *pList;
    pListStart = malloc(sizeof(struct listNode));
    pList = pListStart;
    url = strtok(line, " ");
    hop = atoi(strtok(NULL, " "));
    clawer_link(url, hop, pList);
    addToRoot(root, pListStart);
    destroyList(pListStart);
  }
  struct TrieNode* trieRoot = init_trie();
  indexPage_List(root->next, trieRoot);
  
  char query[1000];
  while(1){
    printf("Enter a web query: ");
    fgets(query, 1000, stdin);
    query[strlen(query) - 1] = '\0';
    int success = 1;
    for(int i =0; i < strlen(query); i++){
      if((query[i] < 97 || query[i] > 122) && query[i] != 32){
        printf("Please enter a query containing only lower-case letters.\n");
        success = 0;
        break;
      }
    }
    if(strcmp(query, "") == 0){
      printf("Exiting the program\n");
      return 0;
    }
    if(success == 1){
      printf("Query is \"%s\".\n", query);
      printf("IDF scores are\n");
      char *token = strtok(query, " ");
      while(token != NULL){
        double IDF = getIDF(token, root->next);
        printf("IDF(%s): %f\n", token, IDF);
        token = strtok(NULL, " ");
      }
      
      printf("Web Pages:\n");
      struct listNode *head = root ->next;
      while(head != NULL){
        double score = 0.0;
        char *token = strtok(query, " ");
        while(token != NULL){
          score += getOneScore(token, root, head->addr);
          token = strtok(NULL, " ");
        }
        printf("%s (score: %f)\n", head->addr, score);
        head = head ->next;
      }
    }
  }

  destroyList(root);
  freeTrieMemory(trieRoot);
  fclose(fp);
  return 0;
}