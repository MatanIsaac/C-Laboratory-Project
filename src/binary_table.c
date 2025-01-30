#include "binary_table.h"
#include "isaac_logger.h"
#include <stdlib.h>

BinaryNode* init_binary_node()
{
    BinaryNode* word = calloc(1,sizeof(BinaryNode));
    if (word == NULL)
    {
        log_error(__FILE__,__LINE__,"Failed to allocate memory for BinaryNode\n");
        return NULL;
    }
    return word;
}

void print_binary_node(BinaryNode* node)
{
    if (node)
    {
        /* Found a valid entry */
        log_out(__FILE__,__LINE__,  "[address: %u] - ", node->address);
        print_wordfield(&node->word);
    }
    else
    {
        /* This slot is empty or invalid */
        log_error(__FILE__,__LINE__,  "(empty)\n");
    }
}

BinaryTable init_binary_table(size_t initial_size) 
{
    size_t i;
    BinaryTable table;
    table.data = (BinaryNode**)malloc(initial_size * sizeof(BinaryNode*));
    if (!table.data) 
    {
        log_error(__FILE__,__LINE__,"Failed to allocate BinaryTable");
        exit(EXIT_FAILURE);
    }

    /* Initialize each entry to NULL */
    for (i = 0; i < initial_size; i++)
        table.data[i] = NULL;

    table.size = 0;
    table.capacity = initial_size;
    return table;
}

void free_binary_table(BinaryTable* table) 
{
    size_t i;
    if (!table || !table->data) 
        return;

    for (i = 0; i < table->size; i++) 
    {
        if (table->data[i])
            free(table->data[i]);
    }
    free(table->data);
    table->data = NULL;
    table->size = 0;
    table->capacity = 0;
}

void add_binary_node(BinaryTable* table, unsigned int address, wordfield* word) 
{
    if (!table || !table->data) 
        return;

    if (table->size >= table->capacity) 
    {
        size_t i;
        size_t new_capacity = table->capacity * 2;
        BinaryNode** new_data = (BinaryNode**)realloc(table->data, new_capacity * sizeof(BinaryNode*));

        if (!new_data) 
        {
            log_error(__FILE__,__LINE__,"Failed to resize BinaryTable");
            exit(EXIT_FAILURE);
        }

        /* Initialize new elements to NULL */
        for (i = table->capacity; i < new_capacity; i++) {
            new_data[i] = NULL;
        }

        table->data = new_data;
        table->capacity = new_capacity;
    }

    /* Allocate and initialize new node */
    table->data[table->size] = (BinaryNode*)malloc(sizeof(BinaryNode));
    if (!table->data[table->size]) 
    {
        perror("Failed to allocate BinaryNode");
        exit(EXIT_FAILURE);
    }

    table->data[table->size]->address = address;
    table->data[table->size]->word = *word; 
    table->size++;
}

void print_binary_table(BinaryTable* table)
{
    size_t i;
    if (!table || !table->data) 
        return;

    for (i = 0; i < table->size; i++) 
    {
        print_binary_node(table->data[i]);
    }
}