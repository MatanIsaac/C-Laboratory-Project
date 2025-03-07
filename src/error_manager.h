#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#define DEFAULT_MAX_ERRORS 25
#define MAX_ERROR_LINE 150

typedef enum
{
    ErrorType_InvalidLineLength,
    ErrorType_InvalidMacroName,
    ErrorType_InvalidMacroName_Length,
    ErrorType_InvalidMacroName_Instruction,
    ErrorType_InvalidMacroName_Directive,
    ErrorType_InvalidMacroName_Register,
    ErrorType_ExtraneousText,
    ErrorType_ExtraneousText_Macro,
    ErrorType_MemoryAllocationFailure,
    ErrorType_OpenFileFailure
} ErrorType;

typedef struct
{
    char*       error_msg;  /* prints the reason of the error including FILE and LINE and recommnedation */
    ErrorType   error_type; /* special types for various errors */
    const char* file;       /* the file the error occurred */ 
    int         line;       /* the line the error occurred */
} ErrorEntry;

void add_error_entry(ErrorType error_type,const char *file, int line);

void clean_errors_array();

int is_errors_array_empty();


void print_errors_array();
#endif