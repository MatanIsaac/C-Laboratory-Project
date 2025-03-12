#ifndef PRE_ASM_H
#define PRE_ASM_H
#include <stdio.h>
#include <stdlib.h>
#include "macro_table.h"

/**
 * @brief Looks for macros in the file
 * @param fp Fp the file to read from.
 * @return 1 on success or -1 when reaching EOF.
 */
int parse_macros(FILE* fp, char* filepath, char* output_file, MacroTable* macro_table);

/**
 * @brief Add a new macro to the macro table
 * @param fp The file to read from.
 * @param macro_table Stores the new macro in this table.
 * @return 1 on success or -1 when reaching EOF.
 */
int handle_new_macro(FILE* fp,MacroTable* macro_table, char* macro_name,char* filepath, int* line_count);

/**
 * @brief Opens and prepares the .am file needed for first pass
 * @param filepath The .as file to copy its name from
 * @param output_file Stores the new .am output file name
 * @return NULL if error occured, new am file to write to on success
 */
FILE* prepare_am_file(char* file, char* output_file);


/**
 * @brief Check if a given line is greater than 80 characters
 * @param line The line to check 
 * @return 0 if line is valid, -1 otherwise
 */
int check_line_length(char* line);

int check_macro_name(const char* word, char* filepath, int* line_count);

#endif