#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"

WordNode * create_node(char *input)
{
    // Allocate memory for the WordNode
    WordNode *node = (WordNode *) malloc(sizeof(WordNode));
    if (!node) {
        fprintf(stderr, "ERROR: Out of memory (WordNode).\n");
        return NULL;
    }

    // Initialize the node pointer
    node->next = NULL;
    node->prev = NULL;

    // Allocate memory for the word string (and null terminator)
    int wordlen = strlen(input) + 1;
    node->word = (char *) malloc(wordlen * sizeof(char));

    if (!node->word) {
        fprintf(stderr, "ERROR: Out of memory (WordNode->word).\n");
        return NULL;
    }

    // Copy input string into the node
    strcpy(node->word, input);

    return node;
}

WordLinkedList * create_WLL()
{
    WordLinkedList *list = (WordLinkedList *) malloc(sizeof(WordLinkedList));
    if (!list) {
        fprintf(stderr, "%s\n", "ERROR: Out of memory");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;
}

char * get(WordLinkedList *list, int index)
{
    int ll_elements = list->length;
    if (index > (ll_elements - 1)) {
        fprintf(stderr, "ERROR: Index (%d) out of bounds.\n", index);
        return NULL;
    }

    /* Earlier length check here should ensure we have a
     * valid node at list->head. */
    WordNode *node = list->head;

    int i;
    // Traverse to the i-th node
    for (i = 0; i < index; i++)
    {
        node = node->next;
    }

    return node->word;
}

int append_WLL(WordLinkedList *list, char *str)
{
    if (!list || !str)
        return STATUS_ERROR;

    WordNode *new_node = create_node(str);
    if (!new_node)
        return STATUS_ERROR;

    WordNode *list_node = list->tail;
    if (!list_node) {
        /* This list is empty.
         * Assign new_node to list->head and list->tail. */
        list->head = new_node;
        list->tail = new_node;
        list->length++;
        return STATUS_OK;
    } else {
        /* This list has existing nodes. Append at tail. */
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
        list->length++;
        return STATUS_OK;
    }
}

int is_empty_WLL(WordLinkedList *list)
{
    if (list->head)
        return false;
    else
        return true;
}

int delete_node_WLL(WordNode **node)
{
  if (*node) {
    free((*node)->word);
    free(*node);
    *node = NULL;
    return STATUS_OK;
  } else
    return STATUS_ERROR;
}

int delete_all_nodes_WLL(WordLinkedList *list)
{
    WordNode *list_node = list->head;
    WordNode *last = list_node;

    while (list_node != NULL)
    {
        // Point previous to next to maintain linkage
        last = list_node;
        list_node = list_node->next;
        delete_node_WLL(&last);
    }

    if (list_node != NULL) {
        delete_node_WLL(&list_node);
    }

    list->head = NULL;
    list->length = 0;

    return STATUS_OK;
}

int delete_WLL(WordLinkedList **list)
{
    if (!(*list))
        return STATUS_ERROR;
    else {
        delete_all_nodes_WLL(*list);
        free(*list);
        *list = NULL;
        return STATUS_OK;
    }
}

/* Searches for the input in list.
 *
 * Returns:
 *      NO_MATCH (-1) if no match is found, or
 *      FOUND_MATCH (1) when a match is found
 */
int find_WLL(WordLinkedList *list, char *input)
{
    WordNode *current = list->head;

    int index = 0;
    while(strcmp(current->word, input) < 0) {
        current = current->next;
        index++;
        //printf("\tCurrent word: %s\n", current->word);
    }

    if (strcmp(current->word, input) == 0)
        return FOUND_MATCH;
    else
        return NO_MATCH;
}
