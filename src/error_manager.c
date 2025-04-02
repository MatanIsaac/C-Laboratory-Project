#include "error_manager.h"
#include "utility.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_ERROR_CAPACITY 25
#define ERROR_GROWTH_FACTOR 2

static ErrorEntry* errors = NULL;
static int error_index = 0;
static int error_capacity = 0;

static char* get_error_msg(ErrorType error_type)
{
    char* error_msg = string_calloc(MAX_ERROR_LINE,sizeof(char));
    switch (error_type)
    {
    case ErrorType_InvalidLineLength:
        strcpy(error_msg,"ErrorType_InvalidLineLength: Assembler only accepts lines with length of 81 (including null terminator)");
        break;
    case ErrorType_InvalidDirective_Empty:
        strcpy(error_msg,"ErrorType_InvalidDirective_Empty: The directive contains no valid data and appears empty");
        break;
    case ErrorType_InvalidDirective_MissingQuotes:
        strcpy(error_msg,"ErrorType_InvalidDirective_MissingQuotes: ErrorType_InvalidDirective_MissingQuotes: String directive is missing its enclosing quotation marks (\"\")");
        break;
    case ErrorType_InvalidInstruction_WrongTargetOperand:
        strcpy(error_msg,"ErrorType_InvalidInstruction_WrongTargetOperand: The target operand is invalid or not allowed for this instruction");
        break;
    case ErrorType_InvalidInstruction_WrongSrcOperand:
        strcpy(error_msg,"ErrorType_InvalidInstruction_WrongSrcOperand: The source operand is invalid or not allowed for this instruction");
        break;
    case ErrorType_UnrecognizedToken:
        strcpy(error_msg,"ErrorType_UnrecognizedToken: Unrecognized token, Expected an instruction, directive, or label");
        break;
    case ErrorType_InvalidLabel_UndefinedLabel:
        strcpy(error_msg,"ErrorType_InvalidLabel_UndefinedLabel: Label not found — it does not exist in the label table");
        break;
    case ErrorType_InvalidLabel_InvalidColon:
        strcpy(error_msg,"ErrorType_InvalidLabel_InvalidColon: Label definition is invalid — must have a ':' immediately after the label name (no spaces in between)");
        break;
    case ErrorType_InvalidLabel_Redefinition:
        strcpy(error_msg,"Label Redefinition - the label defined already exists in the label tabel");
        break;
    case ErrorType_InvalidLabel_Reserved:
        strcpy(error_msg,"ErrorType_InvalidLabel_Reserved: Invalid label name — the label defined conflicts with a reserved word (instruction, directive, or register)");
        break;
    case ErrorType_InvalidLabel_Name:
        strcpy(error_msg,"ErrorType_InvalidLabel_Name: Invalid label name, must contain only uppercase/lowercase letters and/or numbers/underscore");
        break;
    case ErrorType_InvalidLabel_MissingSpace:
        strcpy(error_msg,"ErrorType_InvalidLabel_MissingSpace: Label definition is invalid — must include at least one space immediately after ':'");
        break;
    case ErrorType_InvalidLabel_RelativeAddress:
        strcpy(error_msg,"ErrorType_InvalidLabel_RelativeAddress: Missing '&' prefix before label for relative addressing, ensure that relative labels start with '&'");
        break;
    case ErrorType_InvalidLabel_EmptyLabel:
        strcpy(error_msg,"ErrorType_InvalidLabel_EmptyLabel: Found an empty label, provide a valid label");
        break;
    case ErrorType_InvalidLabel_NameTooLong:
        strcpy(error_msg,"ErrorType_InvalidLabel_NameTooLong: Assembler only accepts label name with length of 31 (including null terminator)");
        break;
    case ErrorType_InvalidLabel_MissingColon:
        strcpy(error_msg,"ErrorType_InvalidLabel_MissingColon: A label must end with a colon. I.E: STR:, MAIN:, etc");
        break;
    case ErrorType_InvalidRegister_ExceedingRegisterIndex:
        strcpy(error_msg,"ErrorType_InvalidRegister_ExceedingRegisterIndex: Register index out of range. The assembler only accepts register numbers from 0 to 7 (inclusive)");
        break;
    case ErrorType_InvalidInstruction_MissingSrcOperand:
        strcpy(error_msg,"ErrorType_InvalidInstruction_MissingSrcOperand: Missing source operand for an instruction");
        break;
    case ErrorType_InvalidInstruction_MissingTargetOperand:
        strcpy(error_msg,"ErrorType_InvalidInstruction_MissingTargetOperand: Missing target operand for an instruction");
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
        strcpy(error_msg,"ErrorType_InvalidMacroName_Directive: Macro's name cannot be a directive!");
        break;
    case ErrorType_InvalidMacroName_Register:
        strcpy(error_msg,"ErrorType_InvalidMacroName_Register: Macro's name cannot be a register!");
        break;
    case ErrorType_ExtraneousText:
        strcpy(error_msg,"ErrorType_ExtraneousText: Found Extraneous Text");
        break;
    case ErrorType_ExtraneousText_Instruction:
        strcpy(error_msg,"ErrorType_ExtraneousText: Found Extraneous Text after an instruction");
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

void add_error_entry(ErrorType error_type, const char *file, int line)
{
    if (errors == NULL)
    {
        error_capacity = INITIAL_ERROR_CAPACITY;
        errors = malloc(sizeof(ErrorEntry) * error_capacity);
        if (!errors)
        {
            log_error(__FILE__, __LINE__, "Failed to allocate memory for error array.");
            return;
        }
    }

    /* Resize when we reach full capacity */
    if (error_index >= error_capacity)
    {
        int new_capacity = error_capacity * ERROR_GROWTH_FACTOR;
        ErrorEntry* new_errors = realloc(errors, sizeof(ErrorEntry) * new_capacity);
        if (!new_errors)
        {
            log_error(__FILE__, __LINE__, "Failed to grow error array.");
            return;
        }
        errors = new_errors;
        error_capacity = new_capacity;
    }

    errors[error_index].error_msg   = get_error_msg(error_type);
    errors[error_index].error_type  = error_type;
    errors[error_index].file        = my_strdup(file);
    errors[error_index].line        = line;
    error_index++;
}

void clean_errors_array()
{
    int i = 0;
    for (; i < error_index; i++) 
    {
        if (errors[i].error_msg != NULL)
        {
            free(errors[i].error_msg);
            errors[i].error_msg = NULL;
        }
        if (errors[i].file != NULL)
        {
            free(errors[i].file);
            errors[i].file = NULL;
        }
    }
    error_index = 0;
}

int is_errors_array_empty()
{
    return (error_index == 0) ? VALID_RETURN : INVALID_RETURN;
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