#ifndef WORDFIELD_H
#define WORDFIELD_H

#include <stdio.h>

/* Define the bitfield structure */
typedef struct 
{
    unsigned int opcode      : 6;  /* Opcode field */
    unsigned int src_mode    : 2;  /* Source Addressing Mode */
    unsigned int src_reg     : 3;  /* Source Register */
    unsigned int dest_mode   : 2;  /* Destination Addressing Mode */
    unsigned int dest_reg    : 3;  /* Destination Register */
    unsigned int funct       : 5;  /* Function Code */
    unsigned int are         : 3;  /* A-R-E Bits (Absolute, Relocatable, External) */
} wordfield;

void print_wordfield(wordfield* w);
wordfield* create_wordfield(const char* str);

void set_wordfield_op_funct(wordfield* word, unsigned int opcode, unsigned int funct);
void set_wordfield_src(wordfield* word, unsigned int src_mode, unsigned int src_reg);
void set_wordfield_dest(wordfield* word, unsigned int src_mode, unsigned int src_reg);
void set_wordfield_are(wordfield* word, unsigned int src_mode);

#endif