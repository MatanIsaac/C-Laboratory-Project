#ifndef BINARY_TABLE_H
#define BINARY_TABLE_H

#include "wordfield.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BINARY_TABLE_SIZE 25

typedef struct BinaryNode {
    unsigned int address;
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
BinaryTable init_binary_table(size_t initial_size);
void free_binary_table(BinaryTable* table);
void add_binary_node(BinaryTable* table, unsigned int address, wordfield* word);
void print_binary_table(BinaryTable* table);

#endif /* BINARY_TABLE_H */
