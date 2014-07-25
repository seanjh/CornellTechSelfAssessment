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

/*
create array of size blocks
add elements using the hash function
    hash function should depend on the total size of array (i.e., to achieve uniform distribution)
when table gets > 3/4 full (i.e., occupied / available >= 0.8)
    Create a new extended table
    Rehash all elements in the existing table, given new AVAILABLE
    add to new table
    trash the old_ptr table

// desirable to make the table size prime
// rehash is (hash + constant) % tablesize
// constant and tablesize should be relatively prime

// string "fold_ptring" as a method for hashing

*/


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

/* Creates the WordHashTable, and initializes its char* pointers to
 * NULL.
 */
WordHashTable * create_extended_WHT(WordHashTable *old_ptr)
{
    unsigned int num_buckets;
    WordHashTable *new_ptr;

    if (!old_ptr) {
        // Create the new WordHashTable
        new_ptr = (WordHashTable *) malloc(sizeof(WordHashTable));
        num_buckets = DEFAULT_SIZE;
    } else {
        num_buckets = old_ptr->available * EXTEND_SCALE;
        new_ptr = old_ptr;
    }

    if (!new_ptr) {
        fprintf(stderr, "ERROR: Out of memory (WHT words)\n");
        return NULL;
    }

    // Allocate memory for the buckets, which each contain char pointers
    char **words_ptr = (char **) malloc(num_buckets * sizeof(char *));
    if (!words_ptr) {
        fprintf(stderr, "ERROR: Out of memory (WHT words)\n");
        free(new_ptr);
        return NULL;
    }

    new_ptr->words = words_ptr;

    // Initialize the pointers to NULL
    unsigned int i;
    for (i = 0; i < num_buckets; i++) {
        new_ptr->words[i] = NULL;
    }

    new_ptr->available = num_buckets;
    new_ptr->occupied = 0;

    return new_ptr;
}

/* Creates a fresh WordHashTable. This table increases in bucket count
 * by the factor of EXTEND_SCALE. All the existing elements in the old
 * table are re-hased, and inserted in the new table. Finally, the old
 * table is freed. */
unsigned int extend_WHT(WordHashTable **old)
{
    printf("Extending WHT from %u to %u\n", (*old)->available, (*old)->available * 2);
    
    WordHashTable *new_ptr = create_extended_WHT(*old);
    if (!new_ptr) {
        fprintf(stderr, "%s\n", "ERROR: Extended table could not be created.\n");
        exit(EXIT_FAILURE);
    }

    if (copy_table(*old, new_ptr) == STATUS_ERROR) {
        fprintf(stderr, "%s\n", "ERROR: Table could not be copied.\n");
        exit(EXIT_FAILURE);
    }

    if (delete_WHT(old) == STATUS_ERROR) {
        fprintf(stderr, "%s\n", "ERROR: Table could not be deleted.\n");
        exit(EXIT_FAILURE);
    }

    // Point the existing pointer to the new extended table
    *old = new_ptr;
}

unsigned int copy_table(WordHashTable *old_table, WordHashTable *new_table)
{
    unsigned int i;
    unsigned int num_buckets = old_table->available;
    unsigned int key;
    for (i = 0; i < num_buckets; i++) {

        /* INSERT FUNCTION HERE?
         * May not be necessary, since copy_table should only be invoked
         * when we have already completed a extend_WHT. That is, we know
         * the ratio will be below the threshold.
         *
         * However, custom code here will require some repitition with logic
         * in the insert function*/

        if (old_table->words[i]) {
            key = hash(old_table->words[i], num_buckets);

            // If the new address is a NULL pointer, point this new address
            // towards the current word.
            if (!(new_table->words[key])) {
                new_table->words[key] = old_table->words[i];

            // Collision encountered. Linear probe to find the next open address.
            // http://en.wikipedia.org/wiki/Linear_probing
            } else {
                key = probe(new_table, key);
                if (key > (num_buckets - 1)) {
                    printf("ERROR: Linear probe return invalid index (%u)!\n", key);
                    exit(EXIT_FAILURE);
                }
                new_table->words[key] = old_table->words[i];
            }
        }
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
    // Hash the table in slices specified by HASH_CHAR_LEN
    //if (DEBUG_MODE)
        //printf("hasing \"%s\"\n", str);
    unsigned int total_len = strlen(str);
    unsigned int slice_len = total_len / HASH_CHAR_LEN;

    unsigned long long sum = 0;

    unsigned int i, j;
    unsigned int mult;
    unsigned int slice_start, slice_end;
    for (i = 0; i < slice_len; i++) { // slices
        mult        =   1;
        slice_start =   i * HASH_CHAR_LEN;
        slice_end   =   (i * HASH_CHAR_LEN) + HASH_CHAR_LEN;

        for (j = slice_start; j < slice_end; j++) {
            sum += str[j] * mult;
            //if (DEBUG_MODE)
                //printf("\t\'%c\' * %u = %u (sum = %llu)\n",
                    //str[j], mult, str[j] * mult, sum);
            mult *= HASH_MULTIPLIER;
        }
    }

    // Sum any remaining characters beyond (slice_len - 1) * HASH_CHAR_LEN
    mult = 1;
    for (j; j < total_len; j++)
    {
        /*if (DEBUG_MODE)
            printf("\t\'%c\' * %u = %u (sum = %llu)\n",
                str[j], mult, str[j] * mult, sum);*/
        sum += str[j] * mult;
        mult *= HASH_MULTIPLIER;
    }

    /*if (DEBUG_MODE)
        printf("\tTotal sum: %llu\n", sum);*/

    return sum % size;
}

int insert_WHT(WordHashTable *table, char *string)
{
    printf("\tInserting %s (table bucket %u of %u)\n", string, table->occupied + 1, table->available);
    // extend when/if ratio > 0.75
    double ratio = ((double) table->occupied) / ((double) table->available);
    if (ratio > LOAD_LIMIT) {
        extend_WHT(&table);
    }

    // Locate the bucket for the string
    unsigned int new_index = hash(string, table->available);
    // If there's a collision, probe for an open address
    if (table->words[new_index])
        new_index = probe(table, new_index);

    // Reserve memory for the new string
    char *str_ptr = (char *) malloc((sizeof(string) + 1) * sizeof(char));
    if (!str_ptr) {
        fprintf(stderr, "ERROR: Out of memory (WHT word)!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(str_ptr, string);
    // Update the bucket to point at this string
    table->words[new_index] = str_ptr;
    
    table->occupied++;
}

unsigned int get_key_linear(unsigned int base, unsigned int iter, unsigned int len)
{
    return (base + iter) % len;
}

unsigned int get_key_quadratic(unsigned int base, unsigned int iter, unsigned int len)
{
    return (base + (iter * iter)) % len;
}

unsigned int probe(WordHashTable *table, unsigned int key)
{
    unsigned int total_len = table->available;
    unsigned int j = 0;
    unsigned int new_key = key;

    while(j < total_len && table->words[new_key])
    {
        j++;
        new_key = get_key_linear(key, j, total_len);
    }

    return new_key;
}

int find_WHT(WordHashTable *table, char *str)
{
    return find_linear_WHT(table, str);
}

int find_linear_WHT(WordHashTable *table, char* str)
{
    unsigned int total_len = table->available;
    unsigned int key = hash(str, total_len);
    unsigned int new_key = key;
    unsigned int j = 0;

    while (j < total_len && table->words[key])
    {
        if (strcmp(str, table->words[key]) == 0)
            return FOUND;
        j++;
        new_key = (key + (j * j)) % total_len;
    }

    return NOT_FOUND;
}
int find_quadratic_WHT(WordHashTable *table, char *str)
{
    //
}

int delete_WHT(WordHashTable **table)
{
    unsigned int i;
    unsigned int total_len = (*table)->available;

    for (i = 0; i < total_len; i++) 
    {
        if ((*table)->words[i]) {
            free((*table)->words[i]);
            (*table)->words[i] = NULL;
        }
    }

    free((*table));
    *table = NULL;
    
    return STATUS_OK;
}
