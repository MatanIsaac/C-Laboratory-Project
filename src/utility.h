#ifndef UTILITY_H
#define UTILITY_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

char* get_filename(const char* file);

char* string_calloc(size_t element_count,size_t size_of_element);

char* string_malloc(size_t size);

#endif