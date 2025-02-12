#include "binary_table.h"
#include "isaac_logger.h"
#include <stdlib.h>
#include "utility.h"

BinaryNode* init_binary_node()
{
    BinaryNode* node = calloc(1,sizeof(BinaryNode));
    if (node == NULL)
    {
        log_error(__FILE__,__LINE__,"Failed to allocate memory for BinaryNode\n");
        return NULL;
    }
    return node;
}

void print_binary_node(BinaryNode* node)
{
    if (node)
    {
        /* Found a valid entry */
        fprintf(stdout,  "[address: %u] - [line: %s] - \t\t", node->address, (node->line) ? node->line : "NULL");
        print_wordfield(&node->word);

    }
    else
    {
        /* This slot is empty or invalid */
        log_error(__FILE__,__LINE__,  "(empty)\n");
    }
}

BinaryTable* init_binary_table(size_t initial_size) 
{
    size_t i;
    BinaryTable* table = malloc(sizeof(BinaryTable));
    if (!table) 
    {
        log_error(__FILE__, __LINE__, "Failed to allocate BinaryTable");
        exit(EXIT_FAILURE);
    }
    table->data = (BinaryNode**)malloc(initial_size * sizeof(BinaryNode*));
    if (!table->data) 
    {
        log_error(__FILE__,__LINE__,"Failed to allocate BinaryTable data");
        exit(EXIT_FAILURE);
    }

    /* Initialize each entry to NULL */
    for (i = 0; i < initial_size; i++)
        table->data[i] = NULL;

    table->size = 0;
    table->capacity = initial_size;
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
    free(table);
}

void add_binary_node(BinaryTable* table, unsigned int address, char* line) 
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
    table->data[table->size] = init_binary_node();
    if (!table->data[table->size]) 
    {
        perror("Failed to allocate BinaryNode");
        exit(EXIT_FAILURE);
    }

    table->data[table->size]->address = address;
    table->data[table->size]->line = my_strdup(line);
    /*    table->data[table->size]->word = *word; */
    table->size++;
}

void set_binary_node_wordfield(BinaryTable* table, unsigned int address, wordfield* word)
{
    int node_index = -1;
    if (!table || !table->data || !word) 
        return;

    node_index = search_binary_table(table, address);
    if(node_index != -1)
    {
        table->data[node_index]->word = *word;    
    }
}

void print_binary_table(BinaryTable* table)
{
    size_t i;
    if (!table || !table->data) 
        return;

    log_out(__FILE__,__LINE__,"Printing the Binary Table: \n");
    for (i = 0; i < table->size; i++) 
    {
        print_binary_node(table->data[i]);
    }
}

int search_binary_table(BinaryTable* table, unsigned int address)
{
    size_t i;
    if (!table || !table->data) 
        return -1;

    for (i = 0; i < table->size; i++) 
    {
        if(table->data[i]->address == address)
        {
            return i;
        }
    }
    return -1;
}