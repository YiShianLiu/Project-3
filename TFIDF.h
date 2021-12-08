#ifndef TFIDF_H_
#define TFIDF_H_

double getIDF(char* token, struct listNode *root);
int isInPage(char* url, char* token);
double getOneScore(char* token, struct listNode *root, char* url);

#endif