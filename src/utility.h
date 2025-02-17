#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include "common.h"

/*------------------------------------------------------------------*/
/* files utility functions */
char* get_filename(char* file);
char* get_root_folder_name(char* file);
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* string utility functions */
char* string_calloc(size_t element_count,size_t size_of_element);
char* string_malloc(size_t size);
char* my_strdup(const char* src);
void remove_last_character(char* s);
void remove_first_character(char* s);
char* strncpy_from_pos(char* src,unsigned int pos); /* copies n characters starting from pos */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* first pass utility functions */
bool is_label(char* word, int ignore_colon);
bool is_register(char* word);
bool is_instruction(char* word);
bool is_directive(char* word);
bool is_line_empty(char* line);
bool is_valid_number(char* word);
/* 
    returns the index for the instruction with the name 'op-name' 
    in the instruction table 
*/
int get_instruction_index(const char* op_name);
/*------------------------------------------------------------------*/

#endif
