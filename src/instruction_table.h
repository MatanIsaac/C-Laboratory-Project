#ifndef INSTRUCTION_TABLE_H
#define INSTRUCTION_TABLE_H

#include <stddef.h>

#define INSTRUCTION_TABLE_SIZE 16
#define MAX_OP_NAME 4

/* Represents a single instruction */
typedef struct InstructionNode 
{
   char            op_name[MAX_OP_NAME];    /* Name of the operation, e.g. "ADD", "SUB", etc. */
   unsigned int    op_code;    /* Primary op code */
   unsigned int    funct;      /* Secondary code (if relevant) */
} InstructionNode;

/* Print a single instruction node */
void instruction_node_print(InstructionNode* node);

typedef struct InstructionTable
{
    InstructionNode instructions[INSTRUCTION_TABLE_SIZE];
} InstructionTable;

/* Create a new instruction table (capacity = INSTRUCTION_TABLE_SIZE) on the stack*/
void instruction_table_create(InstructionTable* table);

/* Destroy the instruction table (freeing any dynamically allocated memory) */
void instruction_table_destroy(InstructionTable* table);

/* Insert a new instruction into the table 
   Returns 0 on success, -1 on failure (e.g., table is full). */
int instruction_table_insert(InstructionTable* table, 
                             const char* op_name, 
                             unsigned int op_code, 
                             unsigned int funct);

/* Retrieve a pointer to an instruction by operation name 
   Returns NULL if not found. */
InstructionNode* instruction_table_get(InstructionTable* table, const char* op_name);

/* Remove an instruction by operation name (if it exists) */
void instruction_table_remove(InstructionTable* table, const char* op_name);

/* Print all valid instructions in the table */
void instruction_table_print(InstructionTable* table);

#endif /* INSTRUCTION_TABLE_H */
