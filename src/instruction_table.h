
#ifndef INSTRUCTION_TABLE_H
#define INSTRUCTION_TABLE_H

#include <stddef.h>

/** @brief Number of instructions the table can hold. */
#define INSTRUCTION_TABLE_SIZE 16

/** @brief Maximum length of an instruction name (e.g., "ADD"). */
#define MAX_OP_NAME 4

/**
 * @brief Represents a single instruction.
 */
typedef struct InstructionNode 
{
   char            op_name[MAX_OP_NAME]; /* Operation name (e.g., "ADD"). */
   unsigned int    op_code;              /* Primary opcode. */
   unsigned int    funct;                /* Secondary code. */
} InstructionNode;

/**
 * @brief Prints a single instruction node.
 * @param node The instruction node to print.
 */
void instruction_node_print(InstructionNode* node);

/**
 * @brief Stores a fixed array of InstructionNode items.
 */
typedef struct InstructionTable
{
    InstructionNode instructions[INSTRUCTION_TABLE_SIZE]; /* Array of instructions. */
} InstructionTable;

/**
 * @brief Creates and initializes an instruction table.
 * @param table Pointer to the InstructionTable.
 */
void instruction_table_create(InstructionTable* table);

/**
 * @brief Destroys an instruction table (frees any allocated memory).
 * @param table Pointer to the InstructionTable.
 */
void instruction_table_destroy(InstructionTable* table);

/**
 * @brief Inserts a new instruction into the table.
 * @param table     Pointer to the InstructionTable.
 * @param op_name   The operation name.
 * @param op_code   The primary opcode.
 * @param funct     The secondary code.
 * @return 0 on success, -1 if the table is full.
 */
int instruction_table_insert(InstructionTable* table, const char* op_name, unsigned int op_code, unsigned int funct);

/**
 * @brief Retrieves an instruction by name.
 * @param table     Pointer to the InstructionTable.
 * @param op_name   The operation name to look up.
 * @return Pointer to the found InstructionNode, or NULL if not found.
 */
InstructionNode* instruction_table_get(InstructionTable* table, const char* op_name);

/**
 * @brief Removes an instruction by name.
 * @param table     Pointer to the InstructionTable.
 * @param op_name   The operation name to remove.
 */
void instruction_table_remove(InstructionTable* table, const char* op_name);

/**
 * @brief Prints all valid instructions in the table.
 * @param table Pointer to the InstructionTable.
 */
void instruction_table_print(InstructionTable* table);

/**
 * @brief Returns the index of an instruction in the instruction table by name.
 * @param op_name A C-string representing the instruction name to search for.
 * @return An integer index if found, or INVALID_RETURN if the instruction name is not recognized.
 */
int get_instruction_index(const char* op_name);

#endif /* INSTRUCTION_TABLE_H */
