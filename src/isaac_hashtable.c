#include "isaac_hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Hash function for strings using djb2 algorithm */
static unsigned long hash(const char *str) 
{
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

/* Create a new hash table */
HashTable* isaac_hashtable_create(size_t size) 
{
    HashTable *table = malloc(sizeof(HashTable));
    if (table == NULL)
    {
        perror("[isaac_hashtable_create]: Failed to allocate memory for HashTable\n");
        return NULL;
    }
    table->size = size;
    table->buckets = calloc(table->size, sizeof(HashNode*));
    if (table->buckets == NULL) 
    {
        free(table);
        return NULL;
    }
    return table;
}

/* Destroy the hash table */
void isaac_hashtable_destroy(HashTable *table) 
{
    size_t i = 0;

    if (table == NULL)
        return;

    for (; i < table->size; i++) 
    {
        HashNode *node = table->buckets[i];
        while (node != NULL) 
        {
            HashNode *next = node->next;
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
void isaac_hashtable_insert(HashTable *table, const char *key, const char *value) 
{
    unsigned long hashValue;
    size_t index;
    HashNode *node;
    HashNode *new_node;

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
            node->value = strdup(value);
            return;
        }
        node = node->next;
    }
    /* Key not found, insert new node */
    new_node = malloc(sizeof(HashNode));
    if (new_node == NULL)
        return;
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
}

/* Retrieve a value associated with a key */
const char* isaac_hashtable_get(HashTable *table, const char *key) 
{
    unsigned long hashValue;
    size_t index;
    HashNode *node;

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
void isaac_hashtable_remove(HashTable *table, const char *key) 
{
    unsigned long hashValue;
    size_t index;
    HashNode *node;
    HashNode *prev;
    
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
void isaac_hashtable_print(HashTable* table)
{
    size_t i;
    printf("Index  Bucket\n");
    printf("-----  -------------------------------------------------\n");
    for(i = 0; i < table->size; i++)
    {
        HashNode* node = table->buckets[i];
        printf("%-5lu  ", (unsigned long)i);
        if(node != NULL)
        {
            isaac_hashnode_print(node);
        }
        else
        {
            printf("[Empty]\n");
        }
    }
}

/* Prints the contents of a hashtable node */
void isaac_hashnode_print(HashNode* node)
{
    while(node != NULL)
    {
        printf("[%s | %s]", node->key, node->value);
        node = node->next;
        if(node != NULL)
        {
            printf(" -> ");
        }
        else
        {
            printf(" -> NULL\n");
        }
    }
}
