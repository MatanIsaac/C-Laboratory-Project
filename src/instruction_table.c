#include "instruction_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "utility.h"
#include "logger.h"

/* Print a single instruction node */
void instruction_node_print(InstructionNode* node)
{
    if (node && node->op_name[0] != NULL_TERMINATOR)
    {
        /* Found a valid entry */
        log_out(__FILE__,__LINE__,  "[op-name: %s] - [op_code: %u, funct: %u]\n",
                node->op_name, node->op_code, node->funct);
    }
    else
    {
        /* This slot is empty or invalid */
        log_out(__FILE__,__LINE__,  "(empty)\n");
    }
}

/* Create a new instruction table */
void instruction_table_create(InstructionTable* table)
{
    if (!table)
        return;

    /* Zero-initialize the instruction table */
    memset(table, 0, sizeof(InstructionTable));
}


/* Destroy an instruction table */
void instruction_table_destroy(InstructionTable* table)
{
    if (!table)
        return;

    /* No need to free memory; simply clear the structure */
    memset(table, 0, sizeof(InstructionTable));
}

int instruction_table_insert(InstructionTable* table, const char* op_name, unsigned int op_code, unsigned int funct)
{
    size_t index;

    if (!table || !op_name || op_name[0] == NULL_TERMINATOR)
        return -1;

    
    index = get_instruction_index(op_name);

    if (table->instructions[index].op_name[0] == NULL_TERMINATOR)
    {
        if(strcmp(op_name,"stop") == 0)
        {
            strncpy(table->instructions[index].op_name, op_name, MAX_OP_NAME);
            table->instructions[index].op_name[MAX_OP_NAME] = NULL_TERMINATOR; /* ensure null-termination */
        }   
        else
        {
            strncpy(table->instructions[index].op_name, op_name, MAX_OP_NAME - 1);
            table->instructions[index].op_name[MAX_OP_NAME - 1] = NULL_TERMINATOR; /* ensure null-termination */
        }

        
        table->instructions[index].op_code = op_code;
        table->instructions[index].funct   = funct;
        return 0;
    }

    return -1;
}

/* Retrieve an instruction by name */
InstructionNode* instruction_table_get(InstructionTable* table, const char* op_name)
{
    size_t          index;

    if (!table || !op_name || op_name[0] == NULL_TERMINATOR)
        return NULL;

    index     = get_instruction_index(op_name);

    /* Check if the slot has the same op_name */
    if (strncmp(table->instructions[index].op_name, op_name, MAX_OP_NAME) == 0)
    {
        return &table->instructions[index];
    }

    return NULL;
}


void instruction_table_remove(InstructionTable* table, const char* op_name)
{
    size_t          index;

    if (!table || !op_name || op_name[0] == NULL_TERMINATOR)
        return;

    index     = get_instruction_index(op_name);

    if (strcmp(table->instructions[index].op_name, op_name) == 0)
    {
        memset(&(table->instructions[index]), 0, sizeof(InstructionNode));
    }
    
}

/* Print the entire instruction table */
void instruction_table_print(InstructionTable* table)
{
    size_t i = 0;
    if (!table)
        return;

    log_out(__FILE__,__LINE__, "Instruction Table: (size = %d)\n", INSTRUCTION_TABLE_SIZE);
    for (; i < INSTRUCTION_TABLE_SIZE; i++)
    {
        log_out(__FILE__,__LINE__, "[%2lu] ", (unsigned long)i);
        instruction_node_print(&table->instructions[i]);
    }
    log_out(__FILE__,__LINE__, "\n");
}