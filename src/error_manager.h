#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#define MAX_ERROR_LINE 150

/**
 * @enum ErrorType
 * @brief Represents various types of errors that can occur during the assembly process.
 *
 * This enumeration is used to categorize and describe different errors found
 * during parsing, label validation, macro expansion, value formatting, and more.
 * Each type is associated with a specific error message and handling behavior.
 */
typedef enum
{
    ErrorType_InvalidLineLength,
    ErrorType_InvalidDirective_Empty,
    ErrorType_InvalidDirective_MissingQuotes,
    ErrorType_InvalidInstruction_WrongSrcOperand,
    ErrorType_InvalidInstruction_WrongTargetOperand,
    ErrorType_UnrecognizedToken,
    ErrorType_InvalidLabel_UndefinedLabel,
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
    char*       file;       /* the file the error occurred */ 
    int         line;       /* the line the error occurred */
} ErrorEntry;

/**
 * @brief Adds an error entry to the internal error tracking system.
 *
 * Creates and stores an error entry with the provided type, file, and line number.
 * If the internal error array is full, logs an error to the logger system.
 *
 * @param error_type  The specific type of error encountered.
 * @param file        The filename where the error occurred.
 * @param line        The line number where the error occurred.
 */
void add_error_entry(ErrorType error_type,const char *file, int line);

/**
 * @brief Clears all stored error entries and frees associated memory.
 *
 * Resets the internal error tracking array and deallocates memory
 * for all stored file paths and error messages.
 */
void clean_errors_array();

/**
 * @brief Checks whether any errors have been recorded.
 *
 * @return VALID_RETURN (0) if no errors are present, INVALID_RETURN (-1) if errors exist.
 */
int is_errors_array_empty();

/**
 * @brief Prints all collected error messages to the standard output.
 *
 * Iterates over the internal error array and prints each error message
 * along with its file and line number.
 */
void print_errors_array();

#endif