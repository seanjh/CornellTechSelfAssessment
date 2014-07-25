#ifndef HASHTABLE_T
#define HASHTABLE_T

#ifndef STATUS_CODES_T
#define STATUS_CODES_T

enum {STATUS_OK = 1, STATUS_ERROR = 0};

#endif /* STATUS_CODES */

#ifndef SEARCH_CODES_T
#define SEARCH_CODES_T

enum {FOUND = 1, NOT_FOUND = 0};

#endif /* SEARCH_CODES_T */



typedef struct _WordHashTable
{
    char **words;
    unsigned int available;
    unsigned int occupied;
} WordHashTable;

WordHashTable * create_WHT();
int insert_WHT(WordHashTable *t, char *s);
int find_WHT(WordHashTable *t, char *s);
int delete_WHT(WordHashTable **t);
unsigned int hash(char *s, unsigned int m);

#endif /* HASHTABLE_T */
