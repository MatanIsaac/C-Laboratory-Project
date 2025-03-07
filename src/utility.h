#ifndef UTILITY_H
#define UTILITY_H
 
#include <stdio.h>
#include "common.h"
 
/* ------------------------------------------------------------------ */
/* File utility functions */
 
/**
 * @brief Extracts the filename from a given file path.
 *
 * This function returns a pointer to a newly allocated string 
 * containing the last component of a file path (i.e., the filename).
 * 
 * @param[in] file A C-string representing the full path to a file.
 * @return A newly allocated string containing just the filename, or NULL on failure.
 *
 * @note It's your responsibility to free the returned pointer when done.
 */
char* get_filename(char* file);

/**
 * @brief Retrieves the root folder name from a given file path.
 * @param file The string representing of the full path to a file.
 * @return A newly allocated string containing the root folder name, or NULL on failure.
 */
char* get_root_folder_name(char* file);

/* ------------------------------------------------------------------ */

/* String utility functions */
/**
 * @brief Allocates memory for an array of strings, initializing all bytes to zero.
 * @param element_count   Number of elements to allocate.
 * @param size_of_element Size of each element (in bytes).
 * @return A pointer to the allocated memory, or NULL on failure.
 */
char* string_calloc(size_t element_count, size_t size_of_element);

/**
 * @brief Allocates memory for a string of a given size (in bytes) without zero-initialization.
 * @param size The size to allocate (in bytes).
 * @return A pointer to the allocated memory, or NULL on failure.
 */
char* string_malloc(size_t size);

/**
 * @brief Duplicates a string.(mainly used to copy const char* to char*)
 * @param src The string to duplicate.
 * @return A pointer to the newly allocated copy, or NULL on failure.
 */
char* my_strdup(const char* src);

/**
 * @brief Removes the last character from a string.
 * @param s The string to modify.
 */
void remove_last_character(char* s);

/**
 * @brief Removes the first character from a string. 
 * @param s The string to modify.
 */
void remove_first_character(char* s);

/**
 * @brief Copies a substring from position @p pos in @p src up to the end.
 * @param src The source C-string.
 * @param pos The starting position in @p src.
 * @return A newly allocated string containing the substring, or NULL on failure.
 */
char* strncpy_from_pos(char* src, unsigned int pos);

/**
 * @brief Converts an integer value to a hexadecimal string.
 * @param number The integer value to convert.
 * @return A newly allocated string holding the hex representation, or NULL on failure.  
 */
char* int_to_hex(int number);

/* ------------------------------------------------------------------ */
/* General utility functions */ 
/**
 * @brief Checks if a given word is a label.
 * optionally ignoring a trailing colon if @p ignore_colon is non-zero.
 * @param word         The word to check.
 * @param ignore_colon Whether to ignore a trailing colon (1 for yes, 0 for no).
 * @return `true` if it is recognized as a label, otherwise `false`.
 */
bool is_label(const char* word, int ignore_colon);

/**
 * @brief Determines if a given word represents a register.
 * @param word The word to check.
 * @return `true` if it is a register, otherwise `false`.
 */
bool is_register(const char* word);

/**
 * @brief Determines if a given word is a valid instruction.
 * @param word The word to check.
 * @return `true` if it is an instruction, otherwise `false`.
 */
bool is_instruction(const char* word);

/**
 * @brief Determines if a given word represents a directive (e.g., data declaration).
 * @param word The word to check.
 * @return `true` if it is a directive, otherwise `false`.
 */
bool is_directive(const char* word);

/**
 * @brief Checks if the provided line is empty or contains only whitespace.
 * @param line A C-string representing the line to check.
 * @return `true` if the line is considered empty, otherwise `false`.
 */
bool is_line_empty(char* line);

/**
 * @brief Determines if the given word can be parsed as a valid integer number (decimal or otherwise).
 * @param word The word to check.
 * @return `true` if it is a valid numeric representation, otherwise `false`.
 */
bool is_valid_number(char* word);

/**
 * @brief Returns the index of an instruction in the instruction table by name.
 * @param op_name A C-string representing the instruction name to search for.
 * @return An integer index if found, or -1 if the instruction name is not recognized.
 */
int get_instruction_index(const char* op_name);

#endif /* UTILITY_H */
