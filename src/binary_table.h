#ifndef BINARY_TABLE_H
#define BINARY_TABLE_H

#include "wordfield.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BINARY_TABLE_SIZE 25

typedef struct BinaryNode {
    unsigned int address;
    char* line;
    wordfield word;
} BinaryNode;

BinaryNode* init_binary_node();
void print_binary_node(BinaryNode* node);

/* Typedef for a dynamic array of BinaryNode pointers */
typedef struct 
{
    BinaryNode** data;
    size_t size;      /* Number of elements currently in the table */
    size_t capacity;  /* Maximum capacity before resizing */
} BinaryTable;

/* Functions for initializing and managing a BinaryTable */
BinaryTable* binary_table_create(size_t initial_size);
void binary_table_free(BinaryTable* table);
void binary_node_add(BinaryTable* table, unsigned int address, char* line);
/* sets a binary node in the table, by its address */
void set_binary_node_wordfield(BinaryTable* table, unsigned int address, wordfield* word);
void binary_table_print(BinaryTable* table);
/* returns -1 if not found, otherwise returns the index where found */
int binary_table_search(BinaryTable* table, unsigned int address);
#endif /* BINARY_TABLE_H */
