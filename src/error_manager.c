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
        strcpy(error_msg,"ErrorType_InvalidLineLength: Assembler only accepts lines with length of 81 (including null terminator)");
        break;
    case ErrorType_InvalidInstruction_Missing1stOperand:
        strcpy(error_msg,"ErrorType_InvalidInstruction_Missing1stOperand: Missing 1st Operand for a 2 operands instruction");
        break;
    case ErrorType_InvalidInstruction_Missing2ndOperand:
        strcpy(error_msg,"ErrorType_InvalidInstruction_Missing2ndOperand: Missing 2nd Operand for a 2 operands instruction");
        break;
    case ErrorType_InvalidValue_MissingHashtag:
        strcpy(error_msg,"ErrorType_InvalidValue_MissingHashtag: Immediate values must start with '#'");
        break;
    case ErrorType_InvalidValue:
        strcpy(error_msg,"ErrorType_InvalidValue: Immediate values must start with '#' and contain only numeric characters");
        break;
    case ErrorType_InvalidValue_Exceeding:
        strcpy(error_msg,"ErrorType_InvalidValue_Exceeding: Assembler only accepts 24-bit numeric values (operand exceeds maximum 24-bit value 16,777,215)");
        break;
    case ErrorType_InvalidInstruction_MissingComma:
        strcpy(error_msg,"ErrorType_InvalidInstruction_MissingComma: Invalid Instruction, Missing Comma!");
        break;
    case ErrorType_InvalidMacro_NotFound:
        strcpy(error_msg,"ErrorType_InvalidMacro_NotFound: The specified macro was not found in the macro table");
        break;
    case ErrorType_InvalidMacro_MissingName:
        strcpy(error_msg,"ErrorType_InvalidMacro_MissingName: No macro name was found after 'mcro'");
        break;
    case ErrorType_InvalidMacro_MissingSpace:
        strcpy(error_msg,"ErrorType_InvalidMacro_MissingSpace: Missing space between 'macro' keyword and the macro name in the definition");
        break;
    case ErrorType_InvalidMacroName_Length:
        strcpy(error_msg,"ErrorType_InvalidMacroName_Length: Macro's name is too long, must be with a length of 31 (including null terminator)");
        break;
    case ErrorType_InvalidMacroName_Instruction:
        strcpy(error_msg,"ErrorType_InvalidMacroName_Instruction: Macro's name cannot be an instruction!");
        break;
    case ErrorType_InvalidMacroName_Directive:
        strcpy(error_msg,"ErrorType_InvalidMacroName_Directive: Macro's name cannot be a directive !");
        break;
    case ErrorType_InvalidMacroName_Register:
        strcpy(error_msg,"ErrorType_InvalidMacroName_Register: Macro's name cannot be a register!");
        break;
    case ErrorType_ExtraneousText:
        strcpy(error_msg,"ErrorType_ExtraneousText: Found Extraneous Text");
        break;
    case ErrorType_ExtraneousText_Operand:
        strcpy(error_msg,"ErrorType_ExtraneousText: Found Extraneous Text after an operand");
        break;
    case ErrorType_ExtraneousText_Macro:
        strcpy(error_msg,"ErrorType_ExtraneousText_Macro: Found Extraneous Text After Macro Definition");
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
    if(error_index == 0)
    {
        printf("No Errors Found\n");
    }
    putchar('\n');
    for (; i < error_index; i++) 
    {
        if (errors[i].error_msg != NULL)
        {
            printf("%s, found at [%s,%d]",errors[i].error_msg,errors[i].file,errors[i].line);
            printf("\n");
        }
    }
}