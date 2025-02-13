#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include <stddef.h>
#define DEFAULT_MACRO_TABLE_SIZE 10

/* Node structure for the linked list in each bucket */
typedef struct MacroNode 
{
    char *macro_name;
    char *macro_definition;
} MacroNode;

/* Prints the contents of a hashtable node */
void macro_node_print(MacroNode* node);
/* Allocates memory for a macro node and returns the node */
MacroNode* macro_node_create();

/* Hash table structure */
struct MacroTable 
{
    MacroNode **buckets;
    size_t size;
    size_t next_free_index;  /* next free index */
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

/* Loops through the table and resets all macro nodes */
void macro_table_reset(MacroTable** pTable);

#endif
