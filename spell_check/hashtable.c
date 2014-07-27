#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"

#define DEFAULT_SIZE        1000
#define EXTEND_SCALE       2
#define LOAD_LIMIT            0.75
#define HASH_CONSTANT    1
#define HASH_MULTIPLIER   256
#define HASH_CHAR_LEN     4
#define DEBUG_MODE          true

WordHashTable * create_extended_WHT(WordHashTable *p);
unsigned int copy_table(WordHashTable *t1, WordHashTable *t2);
unsigned int probe(WordHashTable *t, unsigned int a);

/* The argument-free create_WHT wraps the create_exteded_WHT function.
 * The public interface is the argument-less function, while the same
 * basic function is reused to create new, extended tables when needed.
 */
WordHashTable * create_WHT()
{
    WordHashTable *tmp = NULL;
    return create_extended_WHT(tmp);
}

WordHashTable * create_extended_WHT(WordHashTable *old_table)
{
    // Create the new WordHashTable
    WordHashTable *new_table;
    new_table = malloc(sizeof(WordHashTable));
    if (!new_table) {
        fprintf(stderr, "ERROR: Out of memory (WHT words)!\n");
        return NULL;
    }

    // Initialize the table's metadata
    new_table->occupied = 0;
    if (!old_table) {
        new_table->available = DEFAULT_SIZE;
    } else {
        new_table->available = old_table->available * EXTEND_SCALE;
    }


    // Allocate memory for the key array.
    char **words_ptr = malloc(new_table->available * sizeof(char *));
    if (!words_ptr) {
        fprintf(stderr, "ERROR: Out of memory (WHT words)\n");
        free(new_table);
        return NULL;
    }

    // Each array element holds a string pointer (a key). Initialize these to NULL.
    new_table->words = words_ptr;
    unsigned int i;
    for (i = 0; i < new_table->available; i++) {
        new_table->words[i] = NULL;
    }

    return new_table;
}

/* A new temporary hash table is created with additional key locations.
 * The existing strings are all rehashed and inserted in the table at a new
 * key. The temporary temporary table is then discarded, the the old table
 * is updated to point at the new key array.
*/
void extend_WHT(WordHashTable *table)
{
    WordHashTable *tmp_ptr = create_extended_WHT(table);
    if (!tmp_ptr) {
        fprintf(stderr, "%s\n", "ERROR: Extended table could not be created.\n");
        exit(EXIT_FAILURE);
    }

    // Rehash the existing data into the new, extended key array
    if (copy_table(table, tmp_ptr) == STATUS_ERROR) {
        fprintf(stderr, "%s\n", "ERROR: Table could not be copied.\n");
        exit(EXIT_FAILURE);
    }

    // Discard the old, shorter key array
    free(table->words);

    // Point the table at the new key array
    table->words = tmp_ptr->words;

    // Update the table metadata
    table->available = tmp_ptr->available;
    table->occupied = tmp_ptr->occupied;

    // Discard the temp table
    free(tmp_ptr);
}

/* For every non-NULL key in the old_table key array, rehash the
 * string, and add to new_table.*/
unsigned int copy_table(WordHashTable *old_table, WordHashTable *new_table)
{
    unsigned int i;
    unsigned int key;
    for (i = 0; i < old_table->available; i++) {

        if (old_table->words[i]) {
            key = hash(old_table->words[i], new_table->available);

            // Open address found here. Point the new key array to the string
            if (!(new_table->words[key])) {
                new_table->words[key] = old_table->words[i];

            // Collision encountered. Linear probe to find the next open address.
            // http://en.wikipedia.org/wiki/Linear_probing
            } else {
                key = probe(new_table, key);

                if (key > (new_table->available - 1)) {
                    printf("ERROR: Linear probe return invalid index (%u)!\n", key);
                    exit(EXIT_FAILURE);
                }

                new_table->words[key] = old_table->words[i];
            }
        }
        new_table->occupied++;
    } // endfor

    return STATUS_OK;
}

/* Strings are hashed by summing 4 chars at a time. Each character is added
 * to the running sum as a product of its ASCII value times an increasing
 * multiplier. For example, the string "abcde" would split into 2 substrings,
 * "abcd" and "e." Each substring, beginning with "abcd," would be added to the
 * total sum as follows:
 *          'a' * HASH_MULTIPLIER^0
 *      +   'b' * HASH_MULTIPLIER^1
 *      +   'c' * HASH_MULTIPLIER^2
 *      +   'd' * HASH_MULTIPLIER^3
 *
 * Finally, the function returns (sum % size).
 * http://algoviz.org/OpenDSA/Books/OpenDSA/html/HashFuncExamp.html
 */
unsigned int hash(char *str, unsigned int size)
{
    if (!str) {
        fprintf(stderr, "%s\n", "ERROR: Empty string cannot be hashed.");
        exit(EXIT_FAILURE);
    }

    unsigned long long sum = 0;

    // Hash the table in slices specified by HASH_CHAR_LEN
    unsigned int total_len = strlen(str);
    unsigned int slice_len = total_len / HASH_CHAR_LEN;

    unsigned int i = 0, j = 0;
    unsigned int mult;
    unsigned int slice_start, slice_end;
    // Iterates through slices in the string
    for ( ; i < slice_len; i++) {
        mult =   1;
        slice_start = i * HASH_CHAR_LEN;
        slice_end = (i * HASH_CHAR_LEN) + HASH_CHAR_LEN;

        // Iterates through chars in each slice
        for (j = slice_start; j < slice_end; j++) {
            sum += str[j] * mult;
            mult *= HASH_MULTIPLIER;
        }
    }

    // Sum any remaining characters beyond (slice_len - 1) * HASH_CHAR_LEN
    mult = 1;
    for ( ; j < total_len; j++)
    {
        sum += str[j] * mult;
        mult *= HASH_MULTIPLIER;
    }

    return sum % size;
}

/* Inserts new strings in the table. If the table is above LOAD_LIMIT, it is extended.
*
*/
int insert_WHT(WordHashTable *table, char *string)
{
    // extend if ratio > 0.75
    if (((double) table->occupied) / ((double) table->available) > LOAD_LIMIT) {
        extend_WHT(table);
    }

    // Locate the bucket for the string
    unsigned int new_index = hash(string, table->available);

    // If there's a collision, probe for an open address
    if (table->words[new_index])
        new_index = probe(table, new_index);

    // Reserve memory for the new string
    char *str_ptr = malloc((strlen(string) + 1) * sizeof(char));
    if (!str_ptr) {
        fprintf(stderr, "ERROR: Out of memory (WHT word)!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(str_ptr, string);
    // Update the bucket to point at this string
    table->words[new_index] = str_ptr;

    table->occupied++;
}

unsigned int next_address_linear(unsigned int base, unsigned int iter, unsigned int len)
{
    return (base + iter) % len;
}

unsigned int next_address_quadratic(unsigned int base, unsigned int iter, unsigned int len)
{
    return (base + (iter * iter)) % len;
}

unsigned int probe(WordHashTable *table, unsigned int base_key)
{
    unsigned int i = 0;
    unsigned int new_key = base_key;

    while(i < table->available && table->words[new_key])
    {
        i++;
        //new_key = next_address_linear(base_key, i, table->available);
        new_key = next_address_quadratic(base_key, i, table->available);
    }

    return new_key;
}

int find_WHT(WordHashTable *table, char *str)
{
    unsigned int base_key = hash(str, table->available);
    unsigned int new_key = base_key;
    unsigned int i = 0;

    while (i < table->available && table->words[new_key])
    {
        if (strcmp(str, table->words[new_key]) == 0)
            return FOUND;
        i++;
        //new_key = next_address_linear(base_key, i, table->available);
        new_key = next_address_quadratic(base_key, i, table->available);
    }

    return NOT_FOUND;
}

int delete_WHT(WordHashTable **table)
{
    unsigned int i;
    for (i = 0; i < (*table)->available; i++)
    {
        if ((*table)->words[i]) {
            free((*table)->words[i]);
            (*table)->words[i] = NULL;
        }
    }

    free((*table)->words);
    free((*table));
    *table = NULL;

    return STATUS_OK;
}
