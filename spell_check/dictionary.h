#ifndef DICTIONARY_H
#define DICTIONARY_H

enum results {found = 1, not_found = -1};

typedef struct _PCounter
{
    clock_t load_clocks;
    clock_t find_clocks;
    int find_count;
} PCounter;

typedef struct _Dictionary
{
    WordLinkedList *list;
    PCounter *list_counter;

    WordSplayTree *tree;
    PCounter *tree_counter;

    //WordHashTable *table;
    //PCounter *table_counter;
} Dictionary;

Dictionary * load_dictionary(FILE *f);
int spell_check(Dictionary *d, char *s);
void print_summary_report(Dictionary *d);
void delete_dictionary(Dictionary **dict);

#endif /* DICTIONARY_H */
