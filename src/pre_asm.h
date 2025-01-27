#ifndef PRE_ASM_H
#define PRE_ASM_H
#include <stdio.h>

/**
 * @brief looks for macros in the file
 * @param fp fp the file to read from.
 * @return 1 on success or -1 when reaching EOF.
 */
int parse_macros(FILE* fp, const char* filepath, char* output_file);

#endif