#ifndef DICTIONARY_H
#define DICTIONARY_H

#define RANGE_ELEMENTS 9

typedef struct _FindCounter
{
    int find_count;
    clock_t clock_ranges[RANGE_ELEMENTS];
    int clock_range_counts[RANGE_ELEMENTS];
} FindCounter;

typedef struct _PCounter
{
    clock_t load_clocks;
    FindCounter *find_counter;
} PCounter;

typedef struct _Dictionary
{
    WordLinkedList *list;
    PCounter *list_counter;

    WordSplayTree *tree;
    PCounter *tree_counter;

    WordHashTable *table;
    PCounter *table_counter;
} Dictionary;

Dictionary * load_dictionary(FILE *f);
int spell_check(Dictionary *d, char *s);
void print_summary_report(Dictionary *d);
void delete_dictionary(Dictionary **dict);

#endif /* DICTIONARY_H */
