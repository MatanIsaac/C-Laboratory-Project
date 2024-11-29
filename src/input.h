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
 * @brief reads words from a line word by word
 * @param fp the line to read from
 * @param word contains the words as we read them one by one
 * @return 1 on success or -1 when reaching '\0'
 */
int read_word_from_line(char* line, char* word);

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
int parse_macros(FILE* fp);

#endif