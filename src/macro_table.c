#include "macro_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "isaac_logger.h"

/* Create a new hash table */
MacroTable* macro_table_create(size_t size) 
{
    MacroTable *table = malloc(sizeof(MacroTable));
    if (table == NULL)
    {
        perror("[macro_ table__create]: Failed to allocate memory for HashTable\n");
        return NULL;
    }
    table->size = size;
    table->buckets = calloc(table->size, sizeof(MacroNode*));
    if (table->buckets == NULL) 
    {
        free(table);
        return NULL;
    }
    return table;
}

/* Destroy the hash table */
void macro_table_destroy(MacroTable *table) 
{
    size_t i = 0;

    if (table == NULL)
        return;

    for (; i < table->size; i++) 
    {
        MacroNode *node = table->buckets[i];
        while (node != NULL) 
        {
            MacroNode *next = node->next;
            free(node->key);
            free(node->value);
            free(node);
            node = next;
        }
    }
    free(table->buckets);
    free(table);
}

/* Insert a key-value pair into the hash table */
void macro_table_insert(MacroTable *table, const char *key, const char *value) 
{
    unsigned long hashValue;
    size_t index;
    MacroNode *node;
    MacroNode *new_node;

    if (table == NULL || key == NULL)
        return;

    hashValue = hash(key);
    index = hashValue % table->size;

    node = table->buckets[index];
    while (node != NULL) 
    {
        if (strcmp(node->key, key) == 0) 
        {
            /* Update the value */
            free(node->value);
            node->value = my_strdup(value);
            return;
        }
        node = node->next;
    }
    /* Key not found, insert new node */
    new_node = malloc(sizeof(MacroNode));
    if (new_node == NULL)
        return;
    new_node->key = my_strdup(key);
    new_node->value = my_strdup(value);
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
}

/* Retrieve a value associated with a key */
const char* macro_table_get(MacroTable *table, const char *key) 
{
    unsigned long hashValue;
    size_t index;
    MacroNode *node;

    if (table == NULL || key == NULL)
        return NULL;
    
    hashValue = hash(key);
    index = hashValue % table->size;
    node = table->buckets[index];

    while (node != NULL) 
    {
        if (strcmp(node->key, key) == 0)
            return node->value;
        node = node->next;
    }
    return NULL; /* Key not found */
}

/* Remove a key-value pair from the hash table */
void macro_table_remove(MacroTable *table, const char *key) 
{
    unsigned long hashValue;
    size_t index;
    MacroNode *node;
    MacroNode *prev;
    
    if (table == NULL || key == NULL)
        return;

    hashValue = hash(key);
    index = hashValue % table->size;

    node = table->buckets[index];
    prev = NULL;
    while (node != NULL) 
    {
        if (strcmp(node->key, key) == 0) 
        {
            /* Key found, remove node */
            if (prev == NULL)
                table->buckets[index] = node->next;
            else
                prev->next = node->next;
            free(node->key);
            free(node->value);
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
    /* Key not found, nothing to remove */
}

/* Prints the hashtable in an orderly manner */
void macro_table_print(MacroTable* table)
{
    size_t i;
    log_error(__FILE__,__LINE__,"Index  Bucket\n");
    log_error(__FILE__,__LINE__,"-----  -------------------------------------------------\n");
    for(i = 0; i < table->size; i++)
    {
        MacroNode* node = table->buckets[i];
        log_error(__FILE__,__LINE__,"%-5lu  ", (unsigned long)i);
        if(node != NULL)
        {
            macro_node_print(node);
        }
        else
        {
            log_error(__FILE__,__LINE__,"[Empty]\n");
        }
    }
}

/* Prints the contents of a hashtable node */
void macro_node_print(MacroNode* node)
{
    while(node != NULL)
    {
        log_error(__FILE__,__LINE__,"[%s | %s]", node->key, node->value);
        node = node->next;
        if(node != NULL)
        {
            log_error(__FILE__,__LINE__," -> ");
        }
        else
        {
            log_error(__FILE__,__LINE__," -> NULL\n");
        }
    }
}
