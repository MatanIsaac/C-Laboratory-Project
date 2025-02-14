#ifndef COMMNET_H
#define COMMNET_H

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
#define MAX_WORD 25
#define MAX_FILENAME 200
#define MAX_LABEL_LEN 31 /* the length of a symbolic label of a memory address */
#define MAX_BITS_PER_WORD 24 /* 24 bits per memory cell AKA word */
#define MAX_WORDS 3
#define MAX_MEMORY 2097152 /* memory size 2^21 */
#define MAX_OPERANDS 2
#define MAX_REGISTERS 8
#define MAX_INSTRUCTIONS 16
#endif 