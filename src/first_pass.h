#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdlib.h>
#include "label_table.h"

void prepare_first_pass(const char* filepath);

int execute_first_pass(FILE* fp, LabelTable* label_table);

#endif