#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#ifndef STATUS_CODES_T
#define STATUS_CODES_T

enum {STATUS_OK = 1, STATUS_ERROR = 0};

#endif /* STATUS_CODES */

#ifndef SEARCH_CODES_T
#define SEARCH_CODES_T

enum {FOUND = 1, NOT_FOUND = 0};

#endif /* SEARCH_CODES_T */

typedef struct _WordNode
{
    struct _WordNode *next;
    struct _WordNode *prev;
    char *word;
} WordNode;

typedef struct _WordLinkedList
{
    WordNode *  head;
    WordNode *  tail;
    int length;
} WordLinkedList;

WordLinkedList * create_WLL();
char * get_WLL(WordLinkedList *t, int i);
int append_WLL(WordLinkedList *t, char *s);
int is_empty_WLL(WordLinkedList *t);
int delete_WLL(WordLinkedList **t);
int find_WLL(WordLinkedList *t, char *s);

#endif /* LINKEDLIST_H */
