#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include "common.h"

char* get_filename(const char* file);

/*------------------------------------------------------------------*/
/* string utility functions */
char* string_calloc(size_t element_count,size_t size_of_element);
char* string_malloc(size_t size);
char* my_strdup(const char* s);
char* remove_last_character(const char* s);
char* remove_first_character(char* s);
char* strncpy_from_pos(char* src,unsigned int pos); /* copies n characters starting from pos */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* first pass utility functions */
bool is_register(char* word);
bool is_instruction(char* word);
bool is_directive(char* word);
bool is_line_empty(char* line);
/* 
    returns the index for the instruction with the name 'op-name' 
    in the instruction table 
*/
int get_instruction_index(const char* op_name);
/*------------------------------------------------------------------*/

/* Hash function for strings using djb2 algorithm */
unsigned long hash(const char* str);

#endif