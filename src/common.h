#ifndef COMMNET_H
#define COMMNET_H

/* This header defines basic macros and types for use across the project. */

/** @brief Types of operands. */
typedef enum
{
    /* the types below tells us what operand are we "operating" on */
    OPERAND_TYPE_SINGLE = -2, /* for single operands instructions like inc r6, prn r2, etc.. */
    OPERAND_TYPE_FIRST = -1, /* we are "operating" on the first operand out of 2 in total for: mov, cmp, etc.. */
    /* the types below represent the type of the operands */
    OPERAND_TYPE_IMMEDIATE = 0,
    OPERAND_TYPE_DIRECT = 1,
    OPERAND_TYPE_RELATIVE = 2,
    OPERAND_TYPE_REGISTER = 3
} OperandType;

/** @brief Types of directives. */
typedef enum
{
    DIRECTIVE_TYPE_STRING,
    DIRECTIVE_TYPE_DATA,
    DIRECTIVE_TYPE_EXTERN,
    DIRECTIVE_TYPE_ENTRY
} DirectiveType;


#define OUTPUT_PATH "build/output_files/"
#define NEW_LINE '\n'
#define DOUBLE_QUOTE '\"'
#define COMMA ','
#define HASHTAG '#'
#define AMPERSAND '&'
#define COLON ':'
#define SEMICOLON ';'
#define DASH '-'
#define UNDERSCORE '_'
#define NULL_TERMINATOR '\0'
#define VALID_RETURN 0 
#define INVALID_RETURN -1 
#define MAX_LINE 81 /* max line is 80 not including '\n'*/
#define MAX_WORD 100
#define MAX_FILENAME 200
#define MAX_LABEL_LENGTH 31 /* max length for a label */
#define MAX_MACRO_LENGTH 31 /* max length for a macro name */
#define MAX_REGISTERS 8
#define MAX_INSTRUCTIONS 16
#define MAX_DIRECTIVES 8 /* including version of directives without a dot - data/.data, string/.string etc.. */
#define MAX_24_BIT_NUMBER 16777215 /* the max number a 24 bit a "memory cell" can hold */
#define ARE_ABSOLUTE  4  /* 100 in binary, 'A' bit set */
#define ARE_RELOCATABLE 2 /* 010 in binary, 'R' bit set */
#define ARE_EXTERNAL  1  /* 001 in binary, 'E' bit set */
#define START_ADDRESS 100  /* 001 in binary, 'E' bit set */
#define NO_OPERANDS_INSTRUCTION 0
#define ONE_OPERAND_INSTRUCTION 1
#define TWO_OPERANDS_INSTRUCTION 2

#endif 