#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdlib.h>
#include "label_table.h"
#include "macro_table.h"
#include "instruction_table.h"
#include "binary_table.h"
#include "wordfield.h"

typedef enum
{
    OPERAND_TYPE_IMMEDIATE,
    OPERAND_TYPE_DIRECT,
    OPERAND_TYPE_RELATIVE,
    OPERAND_TYPE_REGISTER
} OperandType;

typedef enum
{
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_ENTRY
} DirectiveType;

void prepare_first_pass(const char* filepath,MacroTable* macro_table);
int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table, MacroTable* macro_table);
int get_operands_count(char* str);

/* return -1 if operand is N/A or incorrect etc.. s*/
OperandType get_operand_type(char* str);
DirectiveType get_directive_type(char* str);

wordfield* get_char_wordfield(char* character);

int handle_labels(LabelTable* label_table, unsigned int IC, char* line, char* word,int position);

int handle_instruction(BinaryTable* binary_table, LabelTable* label_table,InstructionTable* instruction_table, 
        unsigned int* IC, char* line, char* word,int* position);
    
int handle_directive(BinaryTable* binary_table, unsigned int* IC, char* line, char* word,int* position);

#endif