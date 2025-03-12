#ifndef COMMNET_H
#define COMMNET_H

/* This header defines basic constants, macros, and a simple 'bool' type for use across the project. */

#define bool int
#define true 1
#define false 0

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