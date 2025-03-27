#ifndef BINARY_TABLE_H
#define BINARY_TABLE_H
 
#include "wordfield.h"
#include <stdlib.h>
#include <stdio.h>
 
/** @brief Default capacity for a BinaryTable. */
#define DEFAULT_BINARY_TABLE_SIZE 25

/**
 * @brief Holds address, line text, and a pointer to a wordfield.
 */
typedef struct BinaryNode
{
    unsigned int address; /* Memory address. */
    char* line;           /* Original line text. */
    wordfield* word;      /* Pointer to the wordfield. */
    /* store a label if we need to fix it in the 2nd-Pass. 
       NULL if there's no unresolved label. */
    char* unresolved_label;
} BinaryNode;

/**
 * @brief Allocates and initializes a BinaryNode.
 * @return A pointer to the new BinaryNode.
 */
BinaryNode* init_binary_node();

/**
 * @brief Prints a single BinaryNode.
 * @param node The BinaryNode to print.
 */
void print_binary_node(BinaryNode* node);

/**
 * @brief A dynamic array of BinaryNode pointers.
 */
typedef struct
{
    BinaryNode** data;  /* Array of pointers to BinaryNode. */
    size_t size;        /* Current number of elements. */
    size_t capacity;    /* Maximum capacity before resizing. */
} BinaryTable;

/**
 * @brief Creates a new BinaryTable.
 * @param initial_size Initial capacity.
 * @return Pointer to the created BinaryTable.
 */
BinaryTable* binary_table_create(size_t initial_size);

/**
 * @brief Frees a BinaryTable and all associated nodes.
 * @param table Pointer to the BinaryTable.
 */
void binary_table_free(BinaryTable* table);

/**
 * @brief Adds a new BinaryNode with address and line to the table.
 * @param table Pointer to the BinaryTable.
 * @param address The address for the new node.
 * @param line The line to associate with the node.
 * @param unresolved_label The unresolved label to be set in 2nd-Pass
 */
void binary_node_add(BinaryTable* table, unsigned int address, char* line,char* unresolved_label);

/**
 * @brief Sets the wordfield of a BinaryNode by its address.
 * @param table Pointer to the BinaryTable.
 * @param address The address to search for.
 * @param word Pointer to the wordfield to set.
 */
void set_binary_node_wordfield(BinaryTable* table, unsigned int address, wordfield* word);

/**
 * @brief Sets the unresolved label of a BinaryNode
 * @param table Pointer to the BinaryTable.
 * @param address The address to search for.
 * @param label_name The name of the unresolved label to set.
 */
void set_binary_node_unresolved_label(BinaryTable* table, unsigned int address, char* name);

/**
 * @brief Prints the contents of the entire table.
 * @param table Pointer to the BinaryTable.
 */
void binary_table_print(BinaryTable* table);

/**
 * @brief Searches for a BinaryNode by address.
 * @param table Pointer to the BinaryTable.
 * @param address The address to find.
 * @return The index if found, otherwise -1.
 */
int binary_table_search(BinaryTable* table, unsigned int address);

#endif /* BINARY_TABLE_H */
