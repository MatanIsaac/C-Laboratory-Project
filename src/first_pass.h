#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdlib.h>
#include "label_table.h"
#include "instruction_table.h"

void prepare_first_pass(const char* filepath);
int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table);
int get_operands_count(char* str);
#endif