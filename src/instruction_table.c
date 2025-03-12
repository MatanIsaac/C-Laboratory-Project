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
        return INVALID_RETURN;

    
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
        return VALID_RETURN;
    }

    return INVALID_RETURN;
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

int get_instruction_index(const char *s)
{
    switch (s[0])
    {
        case 'a':  /* add */
            if (s[1]=='d' && s[2]=='d' && s[3]==NULL_TERMINATOR)
                return 2;  /* "add" */
            break;

        case 'b':  /* bne */
            if (s[1]=='n' && s[2]=='e' && s[3]==NULL_TERMINATOR)
                return 10; /* "bne" */
            break;

        case 'c':  /* cmp, clr */
            if (s[1]=='m' && s[2]=='p' && s[3]==NULL_TERMINATOR)
                return 1;  /* "cmp" */
            if (s[1]=='l' && s[2]=='r' && s[3]==NULL_TERMINATOR)
                return 5;  /* "clr" */
            break;

        case 'd':  /* dec */
            if (s[1]=='e' && s[2]=='c' && s[3]==NULL_TERMINATOR)
                return 8;  /* "dec" */
            break;

        case 'i':  /* inc */
            if (s[1]=='n' && s[2]=='c' && s[3]==NULL_TERMINATOR)
                return 7;  /* "inc" */
            break;

        case 'j':  /* jmp, jsr */
            if (s[1]=='m' && s[2]=='p' && s[3]==NULL_TERMINATOR)
                return 9;  /* "jmp" */
            if (s[1]=='s' && s[2]=='r' && s[3]==NULL_TERMINATOR)
                return 11; /* "jsr" */
            break;

        case 'l':  /* lea */
            if (s[1]=='e' && s[2]=='a' && s[3]==NULL_TERMINATOR)
                return 4;  /* "lea" */
            break;

        case 'm':  /* mov */
            if (s[1]=='o' && s[2]=='v' && s[3]==NULL_TERMINATOR)
                return 0;  /* "mov" */
            break;

        case 'n':  /* not */
            if (s[1]=='o' && s[2]=='t' && s[3]==NULL_TERMINATOR)
                return 6;  /* "not" */
            break;

        case 'p':  /* prn */
            if (s[1]=='r' && s[2]=='n' && s[3]==NULL_TERMINATOR)
                return 13; /* "prn" */
            break;

        case 'r':  /* red, rts */
            if (s[1]=='e' && s[2]=='d' && s[3]==NULL_TERMINATOR)
                return 12; /* "red" */
            if (s[1]=='t' && s[2]=='s' && s[3]==NULL_TERMINATOR)
                return 14; /* "rts" */
            break;

        case 's':  /* sub, stop */
            if (s[1]=='u' && s[2]=='b' && s[3]==NULL_TERMINATOR)
                return 3;  /* "sub" */
            if (s[1]=='t' && s[2]=='o' && s[3]=='p' && s[4]==NULL_TERMINATOR)
                return 15; /* "stop" */
            break;
    }

    return INVALID_RETURN;
}