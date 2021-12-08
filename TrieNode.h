#ifndef TRIENODE_H_
#define TRIENODE_H_

#define MAX_ADDR_LENGTH 1000

struct listNode{
  char addr[MAX_ADDR_LENGTH];
  struct listNode *next;
};
struct TrieNode {
	struct TrieNode* next[26];
	int wordCount;
};

int contains(const struct listNode *pNode, const char *addr);
int getText(const char* srcAddr, char* buffer, const int bufSize);
void insertBack(struct listNode *pNode, const char *addr);
void printAddresses(const struct listNode *pNode);
void destroyList(struct listNode *pNode);
int getLink(const char* srcAddr, char* link, const int maxLinkLength);
void clawer_link(char* input_link, int numHops, struct listNode *pListStartRoot);
void addToRoot(struct listNode *root, struct listNode *pList);
void indexPage(const char* url, struct TrieNode *Root, int print);
int addWordOccurrence(const char* word, struct TrieNode* curr);
void printTrieContents(struct TrieNode* curr, char* wordSoFar);
int freeTrieMemory(struct TrieNode* curr );
void indexPage_List(struct listNode *start, struct TrieNode *Root);
int getWordCount(struct TrieNode *trieRoot);
struct TrieNode* init_trie();
#endif