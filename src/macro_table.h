#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include <stddef.h>

/** @brief Default size for the macro hash table. */
#define DEFAULT_MACRO_TABLE_SIZE 10

/**
 * @brief Holds a macro's name and definition.
 */
typedef struct MacroNode 
{
    char *macro_name;       /* Macro name. */
    char *macro_definition; /* Macro definition. */
} MacroNode;


/**
 * @brief Prints the contents of a MacroNode.
 * @param node The node to print.
 */
void macro_node_print(MacroNode* node);

/**
 * @brief Creates a new MacroNode.
 * @return Pointer to the allocated MacroNode.
 */
MacroNode* macro_node_create();

/**
 * @brief Hash table containing MacroNodes.
 */
typedef struct MacroTable 
{
    MacroNode **buckets;    /* Array of MacroNode pointers. */
    size_t size;            /* Current size of the table. */
    size_t next_free_index; /* Next free index. */
} MacroTable;

/**
 * @brief Creates a new macro hash table.
 * @param size Initial size for the table.
 * @return Pointer to the created MacroTable.
 */
MacroTable* macro_table_create(size_t size);

/**
 * @brief Destroys the given macro hash table.
 * @param table Pointer to the MacroTable to destroy.
 */
void macro_table_destroy(MacroTable* table);

/**
 * @brief Inserts a key-value pair into the table.
 * @param table Pointer to the MacroTable.
 * @param key   The macro name.
 * @param value The macro definition.
 */
void macro_table_insert(MacroTable* table, const char *key, const char *value);

/**
 * @brief Retrieves a value by key from the table.
 * @param table Pointer to the MacroTable.
 * @param key   The macro name to find.
 * @return The macro definition, or NULL if not found.
 */
const char* macro_table_get(MacroTable* table, const char *key);

/**
 * @brief Removes a key-value pair from the table.
 * @param table Pointer to the MacroTable.
 * @param key   The macro name to remove.
 */
void macro_table_remove(MacroTable* table, const char *key);

/**
 * @brief Prints the entire macro table.
 * @param table Pointer to the MacroTable.
 */
void macro_table_print(MacroTable* table);

/**
 * @brief Resets all macro nodes in the table.
 * @param pTable Pointer to the pointer of the MacroTable.
 */
void macro_table_reset(MacroTable** pTable);

#endif
