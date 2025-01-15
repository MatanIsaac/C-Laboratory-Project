#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include "common.h"

char* get_filename(const char* file);

char* string_calloc(size_t element_count,size_t size_of_element);

char* string_malloc(size_t size);

char* my_strdup(const char *s);

bool is_instruction(char* word);

bool is_directive(char* word);

#endif