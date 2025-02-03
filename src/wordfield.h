#ifndef WORDFIELD_H
#define WORDFIELD_H

#include <stdio.h>
#include "instruction_table.h"

#define MASK_SIX_BITS   0x3F 
#define MASK_FIVE_BITS  0x1F  
#define MASK_THREE_BITS 0x07
#define MASK_TWO_BITS   0x03


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

void print_wordfield(wordfield* word);
wordfield* init_wordfield();
wordfield* create_wordfield_by_opname(char* str, InstructionTable* instruction_table);

/* Function to set the wordfield from a 21-bit number */
void set_wordfield_are_num(wordfield* wf, unsigned int num, unsigned int are);
void set_wordfield_by_num(wordfield* wf, unsigned int num);

void set_wordfield_op_funct(wordfield* word, unsigned int opcode, unsigned int funct);
void set_wordfield_src(wordfield* word, unsigned int src_mode, unsigned int src_reg);
void set_wordfield_dest(wordfield* word, unsigned int src_mode, unsigned int src_reg);
void set_wordfield_are(wordfield* word, unsigned int are);

#endif