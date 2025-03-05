#include "error_manager.h"
#include "utility.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>

static int error_index = 0;
static ErrorEntry errors[DEFAULT_MAX_ERRORS];

static char* get_error_msg(ErrorType error_type)
{
    char* error_msg = string_calloc(MAX_ERROR_LINE,sizeof(char));
    switch (error_type)
    {
    case ErrorType_InvalidLineLength:
        strcpy(error_msg,"ErrorType_InvalidLineLength: Assembler only accepts lines with length of 80 or less (not including '\0')");
        break;
    case ErrorType_InvalidMacroName:
        strcpy(error_msg,"ErrorType_InvalidMacroName: Macro's name cannot be an instruction, a direective or a register!");
        break;
    case ErrorType_ExtraneousText:
        strcpy(error_msg,"ErrorType_ExtraneousText: Found Extraneous Text");
        break;
    case ErrorType_MemoryAllocationFailure:
        strcpy(error_msg,"ErrorType_MemoryAllocationFailure: Failed to allocate memory!");
        break;
    case ErrorType_OpenFileFailure:
        strcpy(error_msg,"ErrorType_OpenFileFailure: Failed to open file!");
        break;
    default:
        break;
    }
    return error_msg;
}

void add_error_entry(ErrorType error_type,const char *file, int line)
{
    if (error_index < DEFAULT_MAX_ERRORS)
    {
        errors[error_index].error_msg =  get_error_msg(error_type);
        errors[error_index].error_type = error_type;
        errors[error_index].file = my_strdup(file);
        errors[error_index].line = line;
        error_index++;
    }
    else
    {
        log_error(__FILE__, __LINE__, "Error array is full!");
    }
}

void clean_errors_array()
{
    int i = 0;
    for (; i < error_index; i++) 
    {
        if (errors[i].error_msg != NULL)
            free(errors[i].error_msg);
    }
    error_index = 0;
}

int is_errors_array_empty()
{
    return (error_index == 0) ? 1 : -1;
}

void print_errors_array()
{
    int i = 0;
    putchar('\n');
    for (; i < error_index; i++) 
    {
        if (errors[i].error_msg != NULL)
            printf("%s, found at [%s,%d]",errors[i].error_msg,errors[i].file,errors[i].line);
    }
}