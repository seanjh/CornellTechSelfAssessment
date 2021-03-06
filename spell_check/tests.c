#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "splaytree.h"
#include "hashtable.h"

int test_linkedlist()
{
    WordLinkedList *list;
    list = create_WLL();

    printf("List empty(): %d\n", is_empty_WLL(list));
    append_WLL(list, "abaff");
    append_WLL(list, "abator");
    append_WLL(list, "epharmonic");
    append_WLL(list, "hello");
    append_WLL(list, "jujuism");
    append_WLL(list, "world");
    append_WLL(list, "zip");

    printf("List empty(): %d\n", is_empty_WLL(list));
    printf("\n\nTrying to find_WLL() \"%s\"\n", "world");
    printf("\nfind_WLL() reported %d.\n", find_WLL(list, "world"));
    printf("\n\nTrying to find_WLL() \"%s\"\n", "jazz");
    printf("\nfind_WLL() reported %d.\n", find_WLL(list, "jazz"));

    printf("Deleting list\n");
    delete_WLL(&list);

    return STATUS_OK;
}

int test_splaytree()
{
    WordSplayTree *tree;
    tree = create_WST();

    insert_WST(tree, "bread");
    printf("Word at tree root is %s\n", tree->root->word);
    insert_WST(tree, "ham");
    printf("Word at tree root is %s\n", tree->root->word);
    insert_WST(tree, "cheese");
    insert_WST(tree, "feta");
    insert_WST(tree, "apple");
    insert_WST(tree, "grape");
    insert_WST(tree, "yarn");
    insert_WST(tree, "unicorn");
    insert_WST(tree, "whale");
    insert_WST(tree, "pencil");
    insert_WST(tree, "tar");
    insert_WST(tree, "jam");
    insert_WST(tree, "hit");
    insert_WST(tree, "quote");
    insert_WST(tree, "nonce");
    insert_WST(tree, "mellow");
    insert_WST(tree, "xerox");
    insert_WST(tree, "sam");
    insert_WST(tree, "jello");
    printf("Word at tree root is %s\n", tree->root->word);
    insert_WST(tree, "nude");


    int result;
    char *search = "pencil";

    printf("Searching in tree for \"%s\"\n", search);
    result = find_WST(tree, search);
    if (result > 0) {
        printf("Found \"%s\"\n", search);
    } else {
        printf("Could not find \"%s\"\n", search);
    }

    search = "whale";
    printf("Searching in tree for \"%s\"\n", search);
    result = find_WST(tree, search);
    if (result > 0) {
        printf("Found \"%s\"\n", search);
    } else {
        printf("Could not find \"%s\"\n", search);
    }

    printf("\nDELETING TREE %p beginning at ROOT %p.\n", tree, tree->root);
    delete_WST(&tree);

    if (tree) {
        printf("free() failed! Tree still exists!\n");
    }

    return STATUS_OK;
}

int test_hashtable()
{
    WordHashTable *table;
    table = create_WHT();

    if (table)
        printf("Successfully created table (%u/%u buckets occupied)!\n", 
                table->occupied, table->available);

    insert_WHT(table, "bread");
    insert_WHT(table, "bread");
    insert_WHT(table, "ham");
    insert_WHT(table, "cheese");
    insert_WHT(table, "feta");
    insert_WHT(table, "apple");
    insert_WHT(table, "grape");
    insert_WHT(table, "yarn");
    insert_WHT(table, "unicorn");

    printf("Table has %u/%u buckets occupied!\n", 
            table->occupied, table->available);

    char *search = "ham";
    if (find_WHT(table, search) == 0)
        printf("Found %s in the table!\n", search); 
    else
        printf("Did not find %s in the table!\n", search);

    delete_WHT(&table);
    if (table)
        printf("FAILED TO DELETE TABLE!\n");
    
    return STATUS_OK;
}


int run_tests()
{
    //test_linkedlist();
    //test_splaytree();
    test_hashtable();
    return STATUS_OK;
}

