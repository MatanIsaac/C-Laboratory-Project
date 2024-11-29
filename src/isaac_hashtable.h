#ifndef ISAAC_HASHTABLE_H
#define ISAAC_HASHTABLE_H

#include <stddef.h>

/* Node structure for the linked list in each bucket */
typedef struct HashNode 
{
    char *key;
    char *value;
    struct HashNode *next;
} HashNode;

/* Prints the contents of a hashtable node */
void isaac_hashnode_print(HashNode* node);

/* Hash table structure */
struct HashTable 
{
    HashNode **buckets;
    size_t size;
};

/* Opaque hash table structure */
typedef struct HashTable HashTable;

/* Create a new hash table with the given initial size */
HashTable* isaac_hashtable_create(size_t size);

/* Destroy the hash table */
void isaac_hashtable_destroy(HashTable* table);

/* Insert a key-value pair into the hash table */
void isaac_hashtable_insert(HashTable* table, const char *key, const char *value);

/* Retrieve a value associated with a key */
const char* isaac_hashtable_get(HashTable* table, const char *key);
 
/* Remove a key-value pair from the hash table */
void isaac_hashtable_remove(HashTable* table, const char *key);

/* Prints the hashtable in an orderly manner */
void isaac_hashtable_print(HashTable* table);

#endif 
