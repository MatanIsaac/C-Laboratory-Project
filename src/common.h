#ifndef COMMNET_H
#define COMMNET_H

/* This header defines basic constants, macros, and a simple 'bool' type for use across the project. */

#define bool int
#define true 1
#define false 0

#define NEW_LINE '\n'
#define TAB '\t'
#define DOUBLE_QUOTE '\"'
#define DOT '.'
#define COLON ':'
#define COMMA ','
#define HASHTAG '#'
#define AMPERSAND '&'
#define DASH '-'
#define PLUS '+'
#define SEMICOLON ';'
#define INVALID_CHAR -1
#define NULL_TERMINATOR '\0'
#define MAX_LINE 81 /* max line is 80 not including '\n'*/
#define MAX_WORD 100
#define MAX_FILENAME 200
#define MAX_LABEL_LENGTH 31 /* max length for a label */
#define MAX_MACRO_LENGTH 31 /* max length for a macro name */
#define MAX_BITS_PER_WORD 24 /* 24 bits per memory cell AKA word */
#define MAX_WORDS 3
#define MAX_MEMORY 2097152 /* memory size 2^21 */
#define MAX_OPERANDS 2
#define MAX_REGISTERS 8
#define MAX_INSTRUCTIONS 16
#define MAX_DIRECTIVES 8 /* including version of directives without a dot - data/.data, string/.string etc.. */
#define MAX_24_BIT_NUMBER 16,777,215 /* the max number a 24 bit a "memory cell" can hold */
#endif 