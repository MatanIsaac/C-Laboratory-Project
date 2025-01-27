#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include <stddef.h>

/* Node structure for the linked list in each bucket */
typedef struct MacroNode 
{
    char *key;
    char *value;
    struct MacroNode *next;
} MacroNode;

/* Prints the contents of a hashtable node */
void macro_node_print(MacroNode* node);

/* Hash table structure */
struct MacroTable 
{
    MacroNode **buckets;
    size_t size;
};

/* Opaque hash table structure */
typedef struct MacroTable MacroTable;

/* Create a new hash table with the given initial size */
MacroTable* macro_table_create(size_t size);

/* Destroy the hash table */
void macro_table_destroy(MacroTable* table);

/* Insert a key-value pair into the hash table */
void macro_table_insert(MacroTable* table, const char *key, const char *value);

/* Retrieve a value associated with a key */
const char* macro_table_get(MacroTable* table, const char *key);
 
/* Remove a key-value pair from the hash table */
void macro_table_remove(MacroTable* table, const char *key);

/* Prints the hashtable in an orderly manner */
void macro_table_print(MacroTable* table);

#endif
