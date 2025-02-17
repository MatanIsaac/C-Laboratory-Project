#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_TABLE_DEFAULT_SIZE 10

/* 
    The table grows x2 every time it is full.
*/

enum LabelType 
{ 
    LABELTYPE_CODE, 
    LABELTYPE_DATA, 
    LABELTYPE_EXTERN
};

const char* labeltype_to_string(enum LabelType type);

typedef struct LabelNode
{
    char* name;
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
void label_table_add(LabelTable* table, char* name, unsigned int address, enum LabelType type);
void label_table_print(LabelTable* table);
/* if found returns the index in the label table -> labels, else -1 */
int label_table_search(LabelTable* table, char* name);
int label_table_search_by_address(LabelTable* table, unsigned int address);

/* 
    Sets a label node's address and type in the label table by its name. 
    returns -1 if not found.
*/
int label_table_set_node_by_name(LabelTable* table,char* name, unsigned int address, enum LabelType type);
/*
    Sets a label's type by its address
*/
int label_table_set_label_type(LabelTable* table,unsigned int address, enum LabelType type);

void label_node_print(LabelNode* node);

#endif /* LABEL_TABLE_H */
