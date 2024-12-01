#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>

/**
 * @brief reads words from a file one by one
 * @param fp the file to read from
 * @param word contains the words as we read them one by one
 * @return 1 on success or -1 when reaching EOF.
 */
int read_word(FILE* fp, char* word);

/**
 * 
 */
int read_word_from_line(const char* line, int* start_index, char* word);

/**
 * @brief reads lines from a file one by one 
 * @param fp the file to read from
 * @param line contains the lines as we read them one by one
 * @return 1 on success or -1 when reaching EOF.
 */
int read_line(FILE* fp, char* line);

/**
 * @brief looks for macros in the file
 * @param fp fp the file to read from.
 * @return 1 on success or -1 when reaching EOF.
 */
int parse_macros(FILE* fp, const char* filepath);

#endif