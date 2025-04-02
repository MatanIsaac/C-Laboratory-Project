#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>

/**
 * @brief reads words from a file one by one
 * @param fp    the file to read from
 * @param word  contains the words as we read them one by one
 * @return VALID_RETURN on success or INVALID_RETURN when reaching EOF.
 */
int read_word(FILE* fp, char* word);

/**
 * @brief Reads the next word from the given line starting from a specified position.
 * @param line      The input line from which to extract words.
 * @param word      A character array to store the extracted word. It must be large enough to hold the word.
 * @param start_pos The position in the line to start reading from. 
 * @return The updated position in the line after the extracted word. 
 *         Returns INVALID_RETURN if no more words are found.
 */
int read_word_from_line(const char* line, char* word, int start_pos);

/**
 * @brief reads lines from a file one line at a time 
 * @param fp    the file to read from
 * @param line  contains the lines as we read them one line at a time 
 * @return VALID_RETURN on success or INVALID_RETURN when reaching EOF.
 */
int read_line(FILE* fp, char* line);



#endif