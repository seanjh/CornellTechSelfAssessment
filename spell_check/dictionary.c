#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "linkedlist.h"
#include "splaytree.h"
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

/* Labels are single-character strings (e.g., "A", "B", "C")
 * that mark the beginning of each letter's words.*/
bool is_label(char *str)
{
    //printf("Checking if label.\n");
    return (str[0] >= 'A' && str[0] <= 'Z' && str[1]);
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

void wipe_string(char *str)
{
    while (*str != 0)
    {
        printf("Wiping char %c\n", *str);
        *str = 0;
        str++;
    }
}

void normalize_input(char *str)
{
    convert_to_lower(str);
    trim_newline(str);
    if (is_label(str))
        wipe_string(str);
}

PCounter * create_pcounter()
{
    PCounter * counter = (PCounter *) malloc(sizeof(PCounter));
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
    printf("Creating dictionary\n");
    Dictionary *dict = (Dictionary *) malloc(sizeof(Dictionary));
    if (!dict) {
        fprintf(stderr, "%s\n", "ERROR: Out of memory (Dictionary).");
        exit(EXIT_FAILURE);
    }

    printf("Creating linked list.\n");

    dict->list = create_WLL();
    dict->list_counter = create_pcounter();

    printf("Creating tree.\n");
    dict->tree = create_WST();
    dict->tree_counter = create_pcounter();

    //dict->table = create_WHT();
    //dict->table_counter = create_pcounter();

    return dict;
}

Dictionary * load_dictionary(FILE *infile)
{
    Dictionary *dict = create_dictionary();

    char line[1024];
    clock_t start;
    printf("Reading lines to line %p.\n", line);

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
        
        //before = clock();
        //insert_WHT(dict->table, line);
        //dict->table_counter += clock() - before;
        
        //hold_up(0.5, false);
    }

    double elapsed;

    elapsed = ((double) dict->list_counter->load_clocks) / CLOCKS_PER_SEC * 1000.0;
    printf("List loaded in %0.3f ms\n", elapsed);

    elapsed = ((double) dict->tree_counter->load_clocks) / CLOCKS_PER_SEC * 1000.0;
    printf("Tree loaded in %0.3f ms\n", elapsed);

    return dict;
}

int spell_check(Dictionary *dict, char *input)
{
    int last_result;
    int result;
    //int result_table;
    
    clock_t before;
    clock_t after;
    double elapsed;

    before = clock();
    result = find_WLL(dict->list, input);
    after = clock();
    dict->list_counter->find_clocks += after - before;
    elapsed = ((double) (after - before)) / CLOCKS_PER_SEC * 1000.0;
    printf("List search comleted in %0.3f ms (%ld clocks)\n", elapsed, (after - before));

    last_result = result;
    before = clock();
    result = find_WST(dict->tree, input);
    after = clock();
    dict->tree_counter->find_clocks += after - before;
    elapsed = ((double) (after - before)) / CLOCKS_PER_SEC * 1000.0;
    printf("Tree search comleted in %0.3f ms (%ld clocks)\n", elapsed, (after - before));

    if (last_result != result) {
        fprintf(stderr, "%s\n", "ERROR: Mismatched results from list and tree!\n");
        return not_found;
    }

    if (result == -1)
        return not_found;
    else
        return found;
}

void print_summary_report(Dictionary *d)
{
    fprintf(stderr, "%s\n", "ERROR: Summary reports not implemented!");
}

void delete_dictionary(Dictionary **dict)
{
    if (*dict) {
        delete_WLL(&(*dict)->list);
        delete_WST(&(*dict)->tree);
        //delete_WHT((*dict)->table);
        free((*dict)->list_counter);
        free((*dict)->tree_counter);
        //free((*dict)->table_counter);
        free(*dict);
        *dict = NULL;
    }
}
