#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define STATUS_OK           0
#define STATUS_ERROR     1
#define NO_MATCH            -1
#define FOUND_MATCH      1

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
char * get_WLL(WordLinkedList *list, int index);
int append_WLL(WordLinkedList *list, char *str);
int is_empty_WLL(WordLinkedList *list);
int delete_WLL(WordLinkedList **list);
int find_WLL(WordLinkedList *list, char *str);

#endif /* LINKEDLIST_H */
