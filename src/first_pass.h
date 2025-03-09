#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdlib.h>
#include "label_table.h"
#include "macro_table.h"
#include "instruction_table.h"
#include "binary_table.h"
#include "wordfield.h"

/** @brief Types of operands. */
typedef enum
{
    OPERAND_TYPE_IMMEDIATE,
    OPERAND_TYPE_DIRECT,
    OPERAND_TYPE_RELATIVE,
    OPERAND_TYPE_REGISTER
} OperandType;

/** @brief Types of directives. */
typedef enum
{
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_ENTRY
} DirectiveType;

/**
 * @brief Prepares data for the first pass.
 * @param filepath Path to the source file.
 * @param macro_table Pointer to the macro table.
 */
void prepare_first_pass(const char* filepath, MacroTable* macro_table);

/**
 * @brief Executes the first pass.
 * @param fp File pointer.
 * @param label_table Pointer to the label table.
 * @param instruction_table Pointer to the instruction table.
 * @param macro_table Pointer to the macro table.
 * @return Non-zero on error, zero on success.
 */
int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table, MacroTable* macro_table,const char* filepath);

/**
 * @brief Gets the number of operands in a string.
 * @param str The input string.
 * @return Number of operands.
 */
int get_operands_count(char* str);

/**
 * @brief Determines the operand type.
 * @param str The operand string.
 * @return The operand type.
 */
OperandType get_operand_type(char* str);

/**
 * @brief Determines the directive type.
 * @param str The directive string.
 * @return The directive type.
 */
DirectiveType get_directive_type(char* str);

/**
 * @brief Creates a wordfield from a single character.
 * @param character The input character.
 * @return Pointer to the newly created wordfield.
 */
wordfield* get_char_wordfield(char* character);

/**
 * @brief Handles label processing.
 * @param label_table Pointer to the label table.
 * @param TC Current text counter.
 * @param line The entire line.
 * @param word The label word.
 * @param position Current position in the line.
 * @return Non-zero on error, zero on success.
 */
int handle_labels(LabelTable* label_table, unsigned int TC, char* line, char* word, int position);

/**
 * @brief Handles an instruction line.
 * @param binary_table Pointer to the binary table.
 * @param label_table Pointer to the label table.
 * @param instruction_table Pointer to the instruction table.
 * @param TC Pointer to the text counter.
 * @param line The entire line.
 * @param word The current word.
 * @param position Pointer to the current position in the line.
 * @return Non-zero on error, zero on success.
 */
int handle_instruction(BinaryTable* binary_table, LabelTable* label_table, InstructionTable* instruction_table, 
                       unsigned int* TC, char* line, char* word, int* position, const char* filepath);

/**
 * @brief Handles a directive line.
 * @param binary_table Pointer to the binary table.
 * @param label_table Pointer to the label table.
 * @param TC Pointer to the text counter.
 * @param DC Pointer to the data counter.
 * @param line The entire line.
 * @param word The current word.
 * @param position Pointer to the current position in the line.
 * @return Non-zero on error, zero on success.
 */
int handle_directive(BinaryTable* binary_table, LabelTable* label_table, unsigned int* TC, unsigned int* DC,
                     char* line, char* word, int* position);

#endif
