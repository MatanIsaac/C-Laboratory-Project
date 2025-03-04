#include "macro_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "logger.h"

/* Prints the contents of a hashtable node */
void macro_node_print(MacroNode* node)
{
    if(node != NULL)
    {   
        printf("macro-name:\n\t    %s\nmacro-definition:\n%s\n", node->macro_name, node->macro_definition);
    }
}

MacroNode* macro_node_create()
{
    MacroNode* node = malloc(sizeof(MacroNode));
    if(node == NULL)
    {
        log_error(__FILE__,__LINE__,"Failed to allocate memory for Macro Node !\n");
        return NULL;
    }
    return node;
}

/* Create a new hash table */
MacroTable* macro_table_create(size_t size) 
{
    MacroTable *table = malloc(sizeof(MacroTable));
    
    if (table == NULL)
    {
        log_error(__FILE__,__LINE__,"Failed to allocate memory for Macro Table\n");
        return NULL;
    }

    table->next_free_index  = 0;
    table->size             = size;
    table->buckets          = calloc(table->size, sizeof(MacroNode*));

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
        if(node != NULL) 
        {
            free(node->macro_name);
            free(node->macro_definition);
            free(node);
        }
    }
    free(table->buckets);
    free(table);
}

/* Insert a key-value pair into the hash table */
void macro_table_insert(MacroTable *table, const char *key, const char *value) 
{
    size_t index;
    if (table == NULL || key == NULL)
        return;

    
    if(macro_table_get(table,key) == NULL)
    {
        if (table->next_free_index >= table->size) 
        {
            /* Expand or do something else */
        }
        else 
        {
            index = table->next_free_index++;    
            table->buckets[index] = macro_node_create();
            table->buckets[index]->macro_name = my_strdup(key);
            table->buckets[index]->macro_definition = my_strdup(value);
            return;
        } 
    }
    log_out(__FILE__,__LINE__,"Macro Node index not empty.\n");
}

/* Retrieve a value associated with a key or NULL if not found */
const char* macro_table_get(MacroTable *table, const char *key) 
{
    size_t i;

    if (table == NULL || key == NULL)
        return NULL;
    
    for(i = 0; i < table->size; i++)
    {
        MacroNode* node = table->buckets[i];

        if (node != NULL && strcmp(node->macro_name, key) == 0)
            return node->macro_definition;
    }
    return NULL; /* Key not found */
}

/* Remove a key-value pair from the hash table */
void macro_table_remove(MacroTable *table, const char *key) 
{
    size_t i;

    if (table == NULL || key == NULL)
        return;
    
    for(i = 0; i < table->size; i++)
    {
        MacroNode* node = table->buckets[i];        
        if (node != NULL && strcmp(node->macro_name, key) == 0) 
        {
            free(node->macro_name);
            free(node->macro_definition);
            free(node);
            table->buckets[i] = NULL;
            return;
        }
    }
}

/* Prints the hashtable in an orderly manner */
void macro_table_print(MacroTable* table)
{
    size_t i;
    log_out(__FILE__,__LINE__,"Macro-Table:\n");
    log_out(__FILE__,__LINE__,"--------------------------------------------------------\n");
    for(i = 0; i < table->size; i++)
    {
        MacroNode* node = table->buckets[i];
        log_out(__FILE__,__LINE__,"At Index [%lu]", (unsigned long)i);
        if(node != NULL)
        {
            printf("\n");
            macro_node_print(node);
        }
        else
        {
            printf("\t[Empty]\n");
        }
    }
    printf("\n");
}

void macro_table_reset(MacroTable** table)
{
    macro_table_destroy(*table);
    *table = macro_table_create(DEFAULT_MACRO_TABLE_SIZE);
}
