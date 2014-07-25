#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "splaytree.h"
#include "tests.c"
#include "dictionary.h"

char *help_prompt = (
    "CLI Spell Checker.\n\
    \n\
    Usage:\n\
        spell_check <file>\n\
    \n\
    Options:\n\
        --test  Execute the test suite.\n\
        --help  Display this help text.\n"
);

void parse_args(char const *argv[], char *filename)
{
    if ((argv[1] && strcmp(argv[1], "--test") == 0) ||
        (argv[2] && strcmp(argv[2], "--test") == 0)) {
        run_tests();
        exit(EXIT_SUCCESS);
    } else if (argv[1] && strcmp(argv[1], "--help") == 0) {
        printf("%s\n", help_prompt);
    } else if (!argv[1]) {
        fprintf(stderr, "ERROR: Unrecognized argument(s). Refer to --help for Usage.\n");
        printf("%s\n", help_prompt);
        exit(EXIT_FAILURE);
    }  else {
        // filename (hopefully)
        strcpy(filename, argv[1]);
    }
}

bool is_exit_prompt(char *input)
{
    return (input[0] == '.');
}

void prompt_input(char *input)
{
    printf("\nPlease enter a word (\".\" to exit): ");
    scanf("%s", input);
}

/*
 * Requires 1 argument:
 *      spell_checker.bin <DICTIONARY FILENAME>
 *
*/
int main(int argc, char const *argv[])
{
    /*
        // TODO:
        // refactor PCounter to record performance of each query
        // prevent dupe inserts to linked list
        // find better way to expose, or not dupes in tree
    */


    // File is initialized, or the program exit()s
    char filename[100];
    parse_args(argv, filename);
    FILE *infile = fopen(filename, "r");
    if (!infile) {
        fprintf(stderr, "ERROR: Cannot open file %s!\n", filename);
        exit(EXIT_FAILURE);
    }

    printf("Welcome to Spell Checker!\n");
    printf("This program will check the spelling for words entered at the prompt.\n\n");

    Dictionary *dict = load_dictionary(infile);
    fclose(infile);

    char input[200];
    prompt_input(input);
    printf("\n");
    while (!is_exit_prompt(input))
    {
        if (spell_check(dict, input) == FOUND) {
            printf("\"%s\" is spelled correctly.\n", input);
        } else {
            printf("\"%s\" is misspelled!\n", input);
        }
        prompt_input(input);
        printf("\n");
    }

    print_summary_report(dict);
    delete_dictionary(&dict);

    return EXIT_SUCCESS;
}
