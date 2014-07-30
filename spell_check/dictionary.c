#include <stdlib.h>
#include <stdint.h>
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
    FindCounter *find_counter = malloc(sizeof(FindCounter));
    if (!find_counter)
    {
        fprintf(stderr, "%s\n", "ERROR: Out of memory (PCounter).");
        exit(EXIT_FAILURE);
    }

    find_counter->find_count = 0;
    /*int i;
    int range = 1;
    for (i= 0; i < RANGE_ELEMENTS; i++) {
        find_counter->clock_ranges[i] = range;
        find_counter->clock_range_counts[i] = 0;

        //range++;
        range *= 2 ;
    }*/
    clock_t tmp_ranges[RANGE_ELEMENTS] = {5, 10, 100, 200, 500, 1000, 2000, 5000, 10000};
    memcpy(find_counter->clock_ranges, tmp_ranges, sizeof(find_counter->clock_ranges));
    int tmp_counts[RANGE_ELEMENTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    memcpy(find_counter->clock_range_counts, tmp_counts, sizeof(find_counter->clock_range_counts));
    //find_counter->clock_range_counts[RANGE_ELEMENTS] = {};
    //find_counter->clock_ranges = {1, 5, 10, 50, 100, 500, 1000, 5000, 10000};
    //find_counter->clock_range_counts = {0, 0, 0, 0, 0, 0, 0, 0, 0};


    PCounter *counter = malloc(sizeof(PCounter));
    if (!counter)
    {
        fprintf(stderr, "%s\n", "ERROR: Out of memory (PCounter).");
        exit(EXIT_FAILURE);
    }

    counter->load_clocks = 0l;
    counter->find_counter = find_counter;

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

double clocks_to_ms(clock_t clocks)
{
    return ((double) clocks) / CLOCKS_PER_SEC * 1000.0;
}

void log_find_performance(PCounter *counter, clock_t clocks)
{
    counter->find_counter->find_count++;

    int i;
    for (i = 0; i < RANGE_ELEMENTS; i++) {
        if (clocks <= counter->find_counter->clock_ranges[i]) {
            counter->find_counter->clock_range_counts[i]++;
            break;
        }
    }
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
        //log_find_performance(dict->list_counter, clock() - start);
        dict->list_counter->load_clocks += (clock() - start);

        //printf("Loading to tree %s\n", line);

        start = clock();
        insert_WST(dict->tree, line);
        //log_find_performance(dict->tree_counter, clock() - start);
        dict->tree_counter->load_clocks += (clock() - start);

        start = clock();
        insert_WHT(dict->table, line);
        //log_find_performance(dict->table_counter, clock() - start);
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
    log_find_performance(dict->list_counter, after - before);
    //dict->list_counter->find_clocks += after - before;
    elapsed = ((double) (after - before)) / CLOCKS_PER_SEC * 1000.0;
    printf("\tList search comleted in %0.3f ms (%ld clocks). %s\n",
        elapsed, (after - before), search_result(results[0]));

    before = clock();
    results[1] = find_WST(dict->tree, input);
    after = clock();
    log_find_performance(dict->tree_counter, after - before);
    //dict->tree_counter->find_clocks += after - before;
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
    log_find_performance(dict->table_counter, after - before);
    //dict->table_counter->find_clocks += after - before;
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

void print_load_report(Dictionary *dict)
{
    printf("\n\n%28s\n", "Load Times Summary Report");
    int i;
    for (i = 0; i < 35; i++) { printf("-"); }
    printf("\n");
    clock_t list, tree, table;
    list = dict->list_counter->load_clocks;
    tree = dict->tree_counter->load_clocks;
    table = dict->table_counter->load_clocks;

    printf("List: %ju clocks (%0.3fms)\n", 
            (uintmax_t) list, 
            ((double) list) / CLOCKS_PER_SEC * 1000.0);
    printf("Tree: %ju clocks (%0.3fms)\n", 
            (uintmax_t) tree, 
            ((double) tree) / CLOCKS_PER_SEC * 1000.0);
    printf("Table: %ju clocks (%0.3fms)\n", 
            (uintmax_t) table, 
            ((double) table) / CLOCKS_PER_SEC * 1000.0);

    printf("\n\n");
}

bool is_range_over_pct(FindCounter *fcount, int range_i, int pct)
{
    double result;
    result = 100 * ((double) fcount->clock_range_counts[range_i]) / ((double) fcount->find_count);
    //((double) fcount->clock_range_counts[range_i]) / ((double) fcount->find_count)
    return result >= pct;
}

void print_search_row(Dictionary *dict, int pct)
{
    //dict->list_counter->find_counter->???
    //dict->tree_counter->find_counter->???
    //dict->table_counter->find_counter->???
    int i;
    for (i = 0; i < RANGE_ELEMENTS; i++) {
        if (is_range_over_pct(dict->list_counter->find_counter, i, pct))
            printf("%2s", "L");
        else
            printf("%2s", "");
        if (is_range_over_pct(dict->tree_counter->find_counter, i, pct))
            printf("%2s", "T");
        else
            printf("%2s", "");
        if (is_range_over_pct(dict->table_counter->find_counter, i, pct))
            printf("%2s", "B");
        else
            printf("%2s", "");
        printf("\t");
    }
}

void print_x_axis(clock_t ranges[])
{
    printf("%8s", "");
    int i;
    for (i = 0; i < RANGE_ELEMENTS; i++)
    {
        //printf("%4.1fms", clocks_to_ms(ranges[i]));
        printf("%6s", "=====");
        //printf("%2s", "");
        printf("\t");
    }
    printf("\n");

    printf("%8s", "");
    for (i = 0; i < RANGE_ELEMENTS; i++)
    {
        //printf("%4.1fms", clocks_to_ms(ranges[i]));
        printf("%6ju", (uintmax_t) ranges[i]);
        //printf("%2s", "");
        printf("\t");
    }
    printf("\n");
}

void debug_print(PCounter *counter)
{
    int i;
    //printf("List (%d total)\n", counter->find_counter->find_count);
    for (i = 0; i < RANGE_ELEMENTS; i++) {
        printf("\t%d at %ju clocks\n",
            counter->find_counter->clock_range_counts[i],
            (uintmax_t) counter->find_counter->clock_ranges[i]);
    }
    printf("\n");
    printf("\n");
}

void print_row_divider()
{
    printf("%7s", "");
    int i;
    for (i = 0; i < RANGE_ELEMENTS; i++)
    {
        printf("--------");
    }
    printf("\n");
}

void print_search_report(Dictionary *dict)
{
    printf("%30s\n", "Search Times Summary Report (clocks)");
    int i;
    for (i = 0; i < 35; i++) { printf("-"); }
    printf("\n");

    int pct;
    for (pct = 100; pct > 0; pct -= 5)
    {
        print_row_divider();
        printf("%4d%% | ", pct);
        print_search_row(dict, pct);
        printf("\n");
    }

    print_x_axis(dict->list_counter->find_counter->clock_ranges);
    printf("\n\n");
    
    /*printf("List: \n");
    debug_print(dict->list_counter);
    printf("Tree: \n");
    debug_print(dict->tree_counter);
    printf("Table: \n");
    debug_print(dict->table_counter);
    printf("\n");*/
}

void print_summary_report(Dictionary *dict)
{
    print_load_report(dict);
    print_search_report(dict);
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
