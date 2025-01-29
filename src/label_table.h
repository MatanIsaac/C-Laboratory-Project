#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_TABLE_DEFAULT_SIZE 10

/* 
    The table grows x2 every time it is full.
*/

enum LabelType { CODE, DATA };

const char* labeltype_to_string(enum LabelType type);

typedef struct LabelNode
{
    const char* name;
    unsigned int address;
    enum LabelType type;

} LabelNode;

/* Label Table Structure */
typedef struct LabelTable
{
    LabelNode* labels;        /* Dynamic array of labels */
    unsigned int size;        /* Current number of labels */
    unsigned int capacity;    /* Allocated space */

} LabelTable;

/* Function Declarations */
void label_table_create(LabelTable* table);
void label_table_destroy(LabelTable* table);
void label_table_add(LabelTable* table, const char* name, unsigned int address, enum LabelType type);
void label_table_print(LabelTable* table);
int label_table_search(LabelTable* table, const char* name);

void label_node_print(LabelNode* node);

#endif /* LABEL_TABLE_H */
