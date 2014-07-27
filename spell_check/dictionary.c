#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "linkedlist.h"
#include "splaytree.h"
#include "hashtable.h"
#include "dictionary.h"

void hold_up(int wait, bool verbose)
{
    if (verbose)
        printf("Halting for %d seconds\n", wait);
    clock_t ticks1, ticks2;
    ticks1=clock();
    ticks2=ticks1;
    while((ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC)<wait)
        ticks2=clock();
}

void convert_to_lower(char *str)
{
    //printf("Coverting to lowercase.\n");
    int convert_upper = ('a' - 'A');

    while (*str != 0)
    {
        // Convert all characters to lowercase
        if (*str >= 'A' && *str <= 'Z') {
            *str += convert_upper;
        }
        str++;
    }
}

void trim_newline(char *str)
{
    //printf("Trimming newlines.\n");
    while (*str != 0)
    {
        // Replace newlines and carriage returns with NULL
        if (*str == '\n' || *str == '\r')
            *str = 0;
        str++;
    }
}

char * search_result(int result)
{
    char *str;
    if (result == FOUND) {
        str = "FOUND";
    } else if (result == NOT_FOUND) {
        str = "NOT FOUND";
    } else {
        str = "ERROR";
    }
    return str;
}

void normalize_input(char *str)
{
    convert_to_lower(str);
    trim_newline(str);
}

PCounter * create_pcounter()
{
    PCounter * counter = malloc(sizeof(PCounter));
    if (!counter)
    {
        fprintf(stderr, "%s\n", "ERROR: Out of memory (PCounter).");
        exit(EXIT_FAILURE);
    }

    counter->load_clocks = 0l;
    counter->find_clocks = 0l;
    counter->find_count = 0;

    return counter;
}

Dictionary * create_dictionary()
{
    //printf("Creating dictionary\n");
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) {
        fprintf(stderr, "%s\n", "ERROR: Out of memory (Dictionary).");
        exit(EXIT_FAILURE);
    }

    //printf("Creating linked list.\n");

    dict->list = create_WLL();
    dict->list_counter = create_pcounter();

    //printf("Creating tree.\n");
    dict->tree = create_WST();
    dict->tree_counter = create_pcounter();

    dict->table = create_WHT();
    dict->table_counter = create_pcounter();

    return dict;
}

Dictionary * load_dictionary(FILE *infile)
{
    Dictionary *dict = create_dictionary();

    char line[1024];
    clock_t start;
    printf("Loading dictionary...");

    while (fgets(line, sizeof(line), infile))
    {
        //printf("Line is %s", line);
        normalize_input(line);
        if (!line)
            continue;

        //printf("Loading to list %s\n", line);

        start = clock();
        append_WLL(dict->list, line);
        dict->list_counter->load_clocks += (clock() - start);

        //printf("Loading to tree %s\n", line);

        start = clock();
        insert_WST(dict->tree, line);
        dict->tree_counter->load_clocks += (clock() - start);

        start = clock();
        insert_WHT(dict->table, line);
        dict->table_counter->load_clocks += clock() - start;

        //hold_up(0.5, false);
    }
    printf("DONE\n");

    double elapsed;

    elapsed = ((double) dict->list_counter->load_clocks) / CLOCKS_PER_SEC * 1000.0;
    printf("List loaded in %0.3f ms\n", elapsed);

    elapsed = ((double) dict->tree_counter->load_clocks) / CLOCKS_PER_SEC * 1000.0;
    printf("Tree loaded in %0.3f ms\n", elapsed);

    elapsed = ((double) dict->table_counter->load_clocks) / CLOCKS_PER_SEC * 1000.0;
    printf("Table loaded in %0.3f ms\n", elapsed);

    return dict;
}

int spell_check(Dictionary *dict, char *input)
{
    normalize_input(input);

    int results[3] = {NOT_FOUND, NOT_FOUND, NOT_FOUND};
    //int result_table;

    clock_t before;
    clock_t after;
    double elapsed;

    before = clock();
    results[0] = find_WLL(dict->list, input);
    after = clock();
    dict->list_counter->find_clocks += after - before;
    elapsed = ((double) (after - before)) / CLOCKS_PER_SEC * 1000.0;
    printf("\tList search comleted in %0.3f ms (%ld clocks). %s\n",
        elapsed, (after - before), search_result(results[0]));

    before = clock();
    results[1] = find_WST(dict->tree, input);
    after = clock();
    dict->tree_counter->find_clocks += after - before;
    elapsed = ((double) (after - before)) / CLOCKS_PER_SEC * 1000.0;
    printf("\tTree search comleted in %0.3f ms (%ld clocks). %s\n",
        elapsed, (after - before), search_result(results[1]));

    if (results[1] != results[0]) {
        fprintf(stderr, "%s\n", "ERROR: Mismatched results from list and tree!\n");
        //return NOT_FOUND;
    }

    before = clock();
    results[2] = find_WHT(dict->table, input);
    after = clock();
    dict->table_counter->find_clocks += after - before;
    elapsed = ((double) (after - before)) / CLOCKS_PER_SEC * 1000.0;
    printf("\tTable search comleted in %0.3f ms (%ld clocks). %s\n",
        elapsed, (after - before), search_result(results[2]));

    if (results[2] != results[1] || results[2] != results[0]) {
        fprintf(stderr, "%s\n", "ERROR: Mismatched results from table!\n");
        //return NOT_FOUND;
    }

    if (results[0] == NOT_FOUND && results[1] == NOT_FOUND && results[2] == NOT_FOUND)
        return NOT_FOUND;
    else
        return FOUND;
}

void print_summary_report(Dictionary *d)
{
    fprintf(stderr, "\n\n%s\n\n", "ERROR: Summary reports not implemented!");
}

void delete_dictionary(Dictionary **dict)
{
    if (*dict) {
        delete_WLL(&(*dict)->list);
        delete_WST(&(*dict)->tree);
        delete_WHT(&(*dict)->table);
        free((*dict)->list_counter);
        free((*dict)->tree_counter);
        free((*dict)->table_counter);
        free(*dict);
        *dict = NULL;
    }
}
