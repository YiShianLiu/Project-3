#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "TrieNode.h"
#define MAX_ADDR_LENGTH 1000

int contains(const struct listNode *pNode, const char *addr){
	if(pNode == NULL){
	    return 0;
	}	
	if(strcmp(pNode->addr, addr)==0){
	    return 1;
    }
  else{ 
	return contains(pNode->next, addr);
  } 
}
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
void insertBack(struct listNode *pNodeRoot, const char *addr){
  struct listNode *pNode = pNodeRoot;
  if(pNode->next != NULL){
    insertBack(pNode->next, addr);
  }
  else{
    struct listNode *last=malloc(sizeof(struct listNode));
    strncpy(last->addr, addr, strlen(addr)+1);
    pNode->next=last;
    last->next=NULL;
  }
}
void printAddresses(const struct listNode *pNode){
	if(pNode !=NULL){
	    printf("%s\n",pNode->addr);
	    printAddresses(pNode->next);
	}
}
void destroyList(struct listNode *pNode){
    if(pNode != NULL){
        destroyList(pNode->next);
        free(pNode);
    }
}
int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];
  int numLinks = 0;
  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }
  fscanf(pipe, "%d\n", &numLinks);
  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);
    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}
void clawer_link(char* input_link, int numHops, struct listNode *pListStartRoot){
  struct listNode *pListStart = pListStartRoot;
  
  char startAddr[MAX_ADDR_LENGTH];
  char destAddr[MAX_ADDR_LENGTH];
  
  int hopNum;

  strncpy(startAddr, input_link, MAX_ADDR_LENGTH);
  startAddr[MAX_ADDR_LENGTH - 1] = '\0';

  if(pListStart == NULL){
    fprintf(stderr, "ERROR: could not allocate memory\n");
    return;
  }
  strncpy(pListStart->addr, startAddr, MAX_ADDR_LENGTH);
  pListStart->next = NULL;
  for(hopNum=1; hopNum <= numHops; hopNum++){
    int res = getLink(startAddr, destAddr, MAX_ADDR_LENGTH);
    if(!res){
      break;
    }
    if(contains(pListStart, destAddr)){
      hopNum--; 
    }
    else{
      insertBack(pListStart, destAddr);
      strncpy(startAddr, destAddr, MAX_ADDR_LENGTH);
    }
  }
}
void addToRoot(struct listNode *root, struct listNode *pList){
  if(pList != NULL){
    if(!contains(root, pList ->addr)){
      insertBack(root, pList ->addr);
    }
    addToRoot(root,pList ->next);
  }
}
void indexPage(const char* url, struct TrieNode *Root, int print) {
	
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
	if(print == 1){
	  printf("%s\n", url);
  }
	char *token = strtok(newBuffer, "\n");
	while(token != NULL){
    if(print == 1){
		  printf("\t%s\n", token);
    }
		addWordOccurrence(token, Root);
		
		token = strtok(NULL, "\n");
	}
	return;
}
int addWordOccurrence(const char* word, struct TrieNode* curr) {

	int len = strlen(word);
	if(len == 1) {
		int letter = word[0] -97;
    //Jimmy revise >>>
    if(curr->next[letter] == NULL){
      struct TrieNode* ptr;
      ptr = init_trie();
      curr->next[letter] = ptr;
    }
    //<<< 
		curr->next[letter]->wordCount++;
		return curr->next[letter]->wordCount;
	}
	
	char newWord[len];
	
	for(int n = 0; n < len - 1; n++) {
		newWord[n] = word[n + 1];
	}
	
	newWord[len - 1] = '\0';
	int letter = word[0] -97;
	
	//Jimmy revise >>>
	if(curr->next[letter] == NULL){
		struct TrieNode* ptr;
		ptr = init_trie();
		curr->next[letter] = ptr;
	}
	//<<<
	
	addWordOccurrence(newWord, curr->next[letter]);
	
	return 0;
}
void indexPage_List(struct listNode *start, struct TrieNode *Root){
  if(start != NULL){
    indexPage(start->addr, Root, 1);
    indexPage_List(start ->next, Root);
  }
}
int getWordCount(struct TrieNode *curr){
  int total = 0;

  if(curr->wordCount > 0){
		total += curr->wordCount;
	}

	for(int n = 0; n < 26; n++){
		if(curr->next[n] != NULL){
			total += getWordCount(curr->next[n]);
		}
	}

	return total;
}
struct TrieNode* init_trie(){
  struct TrieNode* curr = (struct TrieNode*)malloc(sizeof(struct TrieNode));
  for(int i =0; i < 26; i++){
    curr->next[i] = NULL;
  }
  curr->wordCount = 0;
  return curr;
}