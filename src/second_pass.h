#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdlib.h>
#include "label_table.h"
#include "binary_table.h"

/**
 * @brief Prepares the second pass.
 */
void prepare_second_pass(const char* filepath,BinaryTable* binary_table,LabelTable* label_table, int ICF, int DCF);

/**
 * @brief Executes the second pass.
 */
int execute_second_pass(BinaryTable* binary_table,LabelTable* label_table, int ICF, int DCF,const char* filepath);

void prepare_output_files(const char* filepath, FILE** fp_ob, FILE** fp_ent, FILE** fp_ext);

#endif