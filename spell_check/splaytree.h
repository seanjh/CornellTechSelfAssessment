#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#define STATUS_OK        0
#define STATUS_ERROR     1
#define NO_MATCH         -1
#define FOUND_MATCH      1

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
int insert_WST(WordSplayTree *t, char *i);
int delete_WST(WordSplayTree **t);
int find_WST(WordSplayTree *t, char *s);
void splay(WordSplayTree *t, WordBTNode *n);

#endif /* SPLAYTREE_H */
