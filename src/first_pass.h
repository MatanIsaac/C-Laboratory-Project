#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdlib.h>
#include "label_table.h"
#include "instruction_table.h"

typedef enum OperandType
{
    IMMEDIATE,
    DIRECT,
    RELATIVE,
    REGISTER
} OperandType;

void prepare_first_pass(const char* filepath);
int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table);
int get_operands_count(char* str);

/* return -1 if operand is N/A or incorrect etc.. s*/
OperandType get_operand_type(char* str);
#endif