#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdlib.h>
#include "label_table.h"
#include "macro_table.h"
#include "instruction_table.h"
#include "binary_table.h"
#include "wordfield.h"
#include "common.h"

/**
 * @brief Prepares and initiates the first pass of the assembler for a given file.
 *
 * Initializes the instruction and label tables, opens the input file, and performs
 * the first pass to collect labels, parse instructions, and populate the binary table.
 *
 * @param filepath      Path of the file currently being processed (used for error logging).
 * @param macro_table   Pointer to the macro table for macro resolution during parsing.
 */
void prepare_first_pass(const char* filepath, MacroTable* macro_table);

/**
 * @brief Executes the full logic of the first pass over the opened source file.
 *
 * Reads lines from the file, identifies and processes labels, instructions, and directives.
 * Updates binary and label tables and calculates final instruction/data counters.
 *
 * @param fp                Pointer to the opened source file.
 * @param label_table       Pointer to the label table to store encountered labels.
 * @param instruction_table Pointer to the instruction table containing all supported instructions.
 * @param macro_table       Pointer to the macro table for macro handling.
 * @param filepath          Path of the file currently being processed (used for error logging).
 * @return VALID_RETURN on success; INVALID_RETURN if any errors are encountered.
 */
int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table, MacroTable* macro_table,const char* filepath);

/**
 * @brief Determines the number of operands required by an instruction.
 *
 * Validates that the given string is a known instruction and returns
 * how many operands it expects (0, 1, or 2).
 *
 * @param str The instruction string to analyze.
 * @return Number of operands (0, 1, or 2), or INVALID_RETURN on failure.
 */
int get_operands_count(char* str);

/**
 * @brief Determines the operand type based on its string format.
 *
 * Identifies whether the operand is immediate, direct, relative, or a register.
 *
 * @param str The operand string to classify.
 * @return Corresponding OperandType enum value or INVALID_RETURN if unrecognized.
 */
OperandType get_operand_type(char* str);

/**
 * @brief Determines the type of directive based on its string representation.
 *
 * Recognizes and maps valid directive names to their corresponding enum types.
 *
 * @param str The directive string (e.g., ".data", ".string").
 * @return Corresponding DirectiveType enum value or INVALID_RETURN if unrecognized.
 */
DirectiveType get_directive_type(char* str);

/**
 * @brief Converts a single character into a wordfield representation.
 *
 * Allocates and returns a wordfield representing the ASCII value of the given character.
 *
 * @param character A string containing a single character.
 * @return Pointer to a dynamically allocated wordfield, or NULL if input is invalid.
 */
wordfield* get_char_wordfield(char* character);

/**
 * @brief Validates and handles a label definition found at the beginning of a line.
 *
 * Checks for proper formatting, redefinitions, reserved names, and adds the label to the table.
 *
 * @param label_table    Pointer to the label table to insert the label into.
 * @param TC             The current instruction counter for label addressing.
 * @param line           The full source line being processed.
 * @param word           The label token read from the line.
 * @param position       The current position in the line.
 * @param filepath       Path of the file currently being processed (used for error logging).
 * @param current_line   The line number currently being processed (used for error logging).
 * @return VALID_RETURN if the label is valid; INVALID_RETURN otherwise.
 */
int handle_labels(LabelTable* label_table, unsigned int TC, char* line, char* word, int position,
    const char* filepath, int current_line);

/**
 * @brief Handles parsing and processing of an instruction line.
 *
 * Determines operand count and delegates handling to the appropriate routine.
 *
 * @param binary_table       Pointer to the binary table.
 * @param instruction_table  Pointer to the instruction table.
 * @param TC                 Pointer to the instruction counter.
 * @param current_line       The line number currently being processed (used for error logging).
 * @param line               Full line containing the instruction.
 * @param word               The instruction keyword.
 * @param position           Pointer to the current position in the line.
 * @param filepath           Path of the file currently being processed (used for error logging).
 * @return VALID_RETURN if processed successfully; INVALID_RETURN on error.
 */
int handle_instruction(BinaryTable* binary_table, InstructionTable* instruction_table, 
    unsigned int* TC,unsigned int current_line, char* line, char* word, int* position, const char* filepath);

/**
 * @brief Handles parsing and processing of a directive line.
 *
 * Identifies the directive type and delegates processing accordingly. Also updates the label table for
 * `.extern` and `.entry` types if necessary.
 *
 * @param binary_table   Pointer to the binary table.
 * @param label_table    Pointer to the label table.
 * @param TC             Pointer to the instruction counter.
 * @param DC             Pointer to the data counter.
 * @param line           Full line containing the directive.
 * @param word           The directive keyword.
 * @param position       Pointer to the current parsing position.
 * @param filepath       Path of the file currently being processed (used for error logging).
 * @param current_line   The line number currently being processed (used for error logging).
 * @return VALID_RETURN if processed successfully; INVALID_RETURN on error.
 */
int handle_directive(BinaryTable* binary_table, LabelTable* label_table, unsigned int* TC, unsigned int* DC,
    char* line, char* word, int* position,const char* filepath, int current_line);

/**
 * @brief Validates an immediate value operand and converts it to an integer.
 *
 * Ensures the operand starts with a hashtag (`#`), is numeric, and within range.
 *
 * @param value_operand  The immediate operand string.
 * @param filepath       Path of the file currently being processed (used for error logging).
 * @param current_line   The line number currently being processed (used for error logging).
 * @return Parsed integer value if valid; otherwise, returns INVALID_RETURN.
 */
int check_immediate_value(char* value_operand, const char* filepath, int current_line);                                 

/**
 * @brief Checks and processes an immediate operand, updating the binary table accordingly.
 * 
 * This function validates the immediate operand, converts it into numeric form, and
 * inserts corresponding nodes into the binary table. It handles both the instruction 
 * wordfield and immediate value wordfield.
 *
 * @param binary_table       Pointer to the binary table to which binary nodes will be added.
 * @param line               The assembly line being processed (for logging purposes).
 * @param immediate_operand  The immediate operand string to validate and process.
 * @param TC                 Pointer to the instruction counter tracking the current binary node position.
 * @param filepath           Path of the file currently being processed (used for error logging).
 * @param current_line       The line number currently being processed (used for error logging).
 * @param wf_instruction     Pointer to the wordfield struct representing the current instruction; 
 * @param wf_immediate_value Pointer to the wordfield struct representing the immediate value; 
 * @return void
 */
void handle_immediate_operand(BinaryTable* binary_table,char* line, char* immediate_operand,unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction,wordfield* wf_immediate_value);

/**
 * @brief Processes a register operand, validates it, and updates the binary table accordingly.
 *
 * This function validates that the provided operand is a correct register operand, 
 * extracts its numeric value, and sets the corresponding fields in the instruction wordfield. 
 * It handles scenarios with single or dual operands, updating source or destination registers 
 * appropriately, and manages binary table entries accordingly.
 *
 * @param binary_table      Pointer to the binary table where binary nodes will be stored.
 * @param line              The assembly line being processed (used for logging/debugging).
 * @param word              The operand string representing the register (expected format: 'r<num>').
 * @param operand1_type     The type of the operand position (e.g., single, first, second operand).
 * @param TC                Pointer to the instruction counter tracking binary node positions.
 * @param filepath          Path to the current file being processed (used for error reporting).
 * @param current_line      The current line number in the file (used for error reporting).
 * @param wf_instruction    Pointer to the instruction's wordfield struct to be updated based 
 *                          on the operand's position and type.
 * @return void
 */
void handle_register_operand(BinaryTable* binary_table, char* line, char* word, OperandType operand1_type,
    unsigned int* TC,const char* filepath, int current_line,wordfield* wf_instruction);

/**
 * @brief Processes a single-operand instruction, validates the operand, and updates the binary table accordingly.
 *
 * This function parses and validates a single-operand instruction from the provided assembly line. It checks
 * operand validity based on instruction opcode, ensures correct operand syntax, and handles different operand
 * types (immediate, direct, relative, register). It generates appropriate binary nodes in the binary table
 * and logs errors if encountered (such as invalid or extraneous text).
 *
 * @param binary_table      Pointer to the binary table to store generated binary nodes.
 * @param line              The current assembly line being processed (used for logging and error handling).
 * @param word              Buffer used internally to read and store words from the line.
 * @param position          Pointer to the current parsing position within the line; updated internally.
 * @param TC                Pointer to the instruction counter tracking the current binary node position.
 * @param filepath          Path to the current file being processed (used for error reporting).
 * @param current_line      The current line number in the file (used for error reporting).
 * @param wf_instruction    Pointer to the instruction's wordfield struct representing the current instruction.
 * @return                  Returns `INVALID_RETURN` (-1) on failure (e.g., invalid operand, extraneous text);
 *                          otherwise, returns a flag indicating label validity status or success status.
 */
int handle_single_operand_instruction(BinaryTable* binary_table,char* line, char* word,int* position, unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction);

/**
 * @brief Processes a double-operand instruction, validates the operands, and updates the binary table accordingly.
 *
 * This function parses and validates a double-operand instruction from the provided assembly line. It checks
 * operands validity based on instruction opcode, ensures correct operand syntax, and handles different operand
 * types (immediate, direct, relative, register). It generates appropriate binary nodes in the binary table
 * and logs errors if encountered (such as invalid or extraneous text).
 *
 * @param binary_table      Pointer to the binary table to store generated binary nodes.
 * @param line              The current assembly line being processed (used for logging and error handling).
 * @param word              Buffer used internally to read and store words from the line.
 * @param position          Pointer to the current parsing position within the line; updated internally.
 * @param TC                Pointer to the instruction counter tracking the current binary node position.
 * @param filepath          Path to the current file being processed (used for error reporting).
 * @param current_line      The current line number in the file (used for error reporting).
 * @param wf_instruction    Pointer to the instruction's wordfield struct representing the current instruction.
 * @return                  Returns `INVALID_RETURN` (-1) on failure (e.g., invalid operand, extraneous text);
 *                          otherwise, returns a flag indicating label validity status or success status.
 */
int handle_double_operand_instruction(BinaryTable* binary_table,char* line, char* word,int* position, unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction);

/**
 * @brief Extracts two operands from an assembly instruction line, ensuring correct syntax and delimiter usage.
 *
 * This function parses and retrieves two operands from the given assembly line, checking for correct comma usage between them.
 * It ensures the operands are correctly separated by a comma, manages potential syntax issues (such as missing commas or operands),
 * and allocates memory dynamically for both operands. It also logs specific errors (e.g., missing comma, missing operand) as needed.
 * 
 * NOTE: we use a double pointer in order to avoid uninitialized use of char*, and for the output parameters 
 * @param line            The assembly instruction line currently being parsed.
 * @param word            Buffer used internally to temporarily store words read from the line.
 * @param position        Pointer to the current parsing position within the line; updated internally.
 * @param filepath        Path to the current file being processed (used for error reporting).
 * @param current_line    The current line number in the file (used for error reporting).
 * @param operand1        Output parameter; pointer to dynamically allocated memory containing the first operand.
 * @param operand2        Output parameter; pointer to dynamically allocated memory containing the second operand.
 * @return void
 */
void get_double_operands(char* line, char* word, int* position, const char* filepath, int current_line,
    char** operand1, char** operand2);

/**
 * @brief Checks if the provided addressing mode is valid for a given opcode as a source operand.
 *
 * This function verifies whether the specified operand addressing mode is permitted as a source operand
 * for the given instruction opcode based on the predefined validation table.
 *
 * @param opcode        The opcode of the instruction being validated.
 * @param operand_type  The addressing mode type (IMMEDIATE, DIRECT, RELATIVE, REGISTER).
 * @return Returns VALID_RETURN if the operand mode is valid, otherwise INVALID_RETURN.
 */
int check_src_operand(int opcode,OperandType operand_type);

/**
 * @brief Checks if the provided addressing mode is valid for a given opcode as a destination operand.
 *
 * This function verifies whether the specified operand addressing mode is permitted as a destination operand
 * for the given instruction opcode based on the predefined validation table.
 *
 * @param opcode        The opcode of the instruction being validated.
 * @param operand_type  The addressing mode type (IMMEDIATE, DIRECT, RELATIVE, REGISTER).
 * @return Returns VALID_RETURN if the operand mode is valid, otherwise INVALID_RETURN.
 */
int check_target_operand(int opcode,OperandType operand_type);

/**
 * @brief Handles a `.data` directive by parsing and processing a list of integers, storing them in the binary table.
 *
 * This function reads numeric values from a `.data` directive in the source line. It supports both
 * single and comma-separated lists of integers. Each value is stored as a binary node, and the function
 * ensures proper formatting and error reporting for empty directives or malformed numbers.
 *
 * @param binary_table     Pointer to the binary table where binary nodes will be added.
 * @param TC               Pointer to the instruction counter (tracks code segment position).
 * @param DC               Pointer to the data counter (tracks data segment size).
 * @param line             The full line containing the directive.
 * @param word             A word buffer used internally to parse tokens.
 * @param position         Pointer to the current character position in the line being parsed.
 * @param filepath         Path to the current file being processed (used for error reporting).
 * @param current_line     The current line number in the file (used for error reporting).
 * @return VALID_RETURN on success; INVALID_RETURN on error (e.g., empty directive or invalid numbers).
 */
int handle_data_directive(BinaryTable* binary_table,unsigned int* TC, unsigned int* DC, char* line, 
    char* word, int* position,const char* filepath, int current_line);

/**
 * @brief Handles a `.string` directive by converting a quoted string into ASCII codes and storing them in the binary table.
 *
 * This function parses a string enclosed in double quotes, validates its format, and adds each character
 * (including the null terminator `'\0'`) as individual binary nodes. Errors are reported for missing quotes or an empty directive.
 *
 * @param binary_table     Pointer to the binary table where binary nodes will be added.
 * @param TC               Pointer to the instruction counter (tracks code segment position).
 * @param DC               Pointer to the data counter (tracks data segment size).
 * @param line             The full line containing the directive.
 * @param word             A word buffer used internally to parse tokens.
 * @param position         Pointer to the current character position in the line being parsed.
 * @param filepath         Path to the current file being processed (used for error reporting).
 * @param current_line     The current line number in the file (used for error reporting).
 * @return VALID_RETURN on success; INVALID_RETURN on error (e.g., missing or malformed quotes).
 */
int handle_string_directive(BinaryTable* binary_table, unsigned int* TC, unsigned int* DC, char* line, 
    char* word, int* position,const char* filepath, int current_line);

/**
 * @brief Validates a label for use with a directive, ensuring it is not redefined unless it's an `.entry`.
 *
 * This function checks that a label used with a directive is valid and not already defined in the label table.
 * It allows redefinition only for `.entry` directives. It also ensures that a label follows the directive.
 *
 * @param label_table      Pointer to the label table storing all defined labels.
 * @param line             The full line containing the directive and label.
 * @param word             A word buffer used internally to parse tokens.
 * @param position         Pointer to the current character position in the line being parsed.
 * @param filepath         Path to the current file being processed (used for error reporting).
 * @param current_line     The current line number in the file (used for error reporting).
 * @return VALID_RETURN if the label is valid; INVALID_RETURN on errors such as redefinition or missing label.
 */
int check_directive_label(LabelTable* label_table, char* line, char* word, int* position,
    const char* filepath, int current_line);

#endif
