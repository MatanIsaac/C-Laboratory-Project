#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#define DEFAULT_MAX_ERRORS 25
#define MAX_ERROR_LINE 150

typedef enum
{
    ErrorType_InvalidLineLength,
    ErrorType_InvalidDirective_Empty,
    ErrorType_InvalidDirective_MissingQuotes,
    ErrorType_InvalidInstruction_WrongSrcOperand,
    ErrorType_InvalidInstruction_WrongTargetOperand,
    ErrorType_UnrecognizedToken,
    ErrorType_InvalidLabel_InvalidColon,
    ErrorType_InvalidLabel_Redefinition,
    ErrorType_InvalidLabel_Reserved,
    ErrorType_InvalidLabel_Name,
    ErrorType_InvalidLabel_MissingSpace,
    ErrorType_InvalidLabel_RelativeAddress,
    ErrorType_InvalidLabel_EmptyLabel,
    ErrorType_InvalidLabel_NameTooLong,
    ErrorType_InvalidLabel_MissingColon,
    ErrorType_InvalidRegister_ExceedingRegisterIndex,
    ErrorType_InvalidInstruction_MissingSrcOperand,
    ErrorType_InvalidInstruction_MissingTargetOperand,
    ErrorType_InvalidValue,
    ErrorType_InvalidValue_MissingHashtag,
    ErrorType_InvalidValue_Exceeding,
    ErrorType_InvalidInstruction_MissingComma,
    ErrorType_InvalidMacro_NotFound,
    ErrorType_InvalidMacro_MissingName,
    ErrorType_InvalidMacro_MissingSpace,
    ErrorType_InvalidMacroName_Length,
    ErrorType_InvalidMacroName_Instruction,
    ErrorType_InvalidMacroName_Directive,
    ErrorType_InvalidMacroName_Register,
    ErrorType_ExtraneousText,
    ErrorType_ExtraneousText_Instruction,
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