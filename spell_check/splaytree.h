#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#ifndef STATUS_CODES_T
#define STATUS_CODES_T

enum {STATUS_OK = 1, STATUS_ERROR = 0};

#endif /* STATUS_CODES */

#ifndef SEARCH_CODES_T
#define SEARCH_CODES_T

enum {FOUND = 1, NOT_FOUND = 0};

#endif /* SEARCH_CODES_T */

typedef struct _WordBTNode
{
    struct _WordBTNode *parent;
    struct _WordBTNode *left;
    struct _WordBTNode *right;
    char *word;
} WordBTNode;

typedef struct _WordTree {
    WordBTNode *root;
} WordSplayTree;

WordSplayTree * create_WST();
int insert_WST(WordSplayTree *t, char *s);
int find_WST(WordSplayTree *t, char *s);
int delete_WST(WordSplayTree **t);

#endif /* SPLAYTREE_H */
