#ifndef PRE_ASM_H
#define PRE_ASM_H
#include <stdio.h>
#include "macro_table.h"

/**
 * @brief looks for macros in the file
 * @param fp fp the file to read from.
 * @return 1 on success or -1 when reaching EOF.
 */
int parse_macros(FILE* fp, const char* filepath, char* output_file, MacroTable* macro_table);

/**
 * @brief add a new macro to the macro table
 * @param fp fp the file to read from.
 * @param macro_table stores the new macro in this table.
 * @return 1 on success or -1 when reaching EOF.
 */
int handle_new_macro(FILE* fp,MacroTable* macro_table, char* macro_name);

#endif