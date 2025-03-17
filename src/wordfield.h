#ifndef WORDFIELD_H
#define WORDFIELD_H

#include <stdio.h>
#include "instruction_table.h"

/** 
 * @brief A bitmasks for extracting two,three,five,six bits.
 */
#define MASK_SIX_BITS   0x3F 
#define MASK_FIVE_BITS  0x1F  
#define MASK_THREE_BITS 0x07
#define MASK_TWO_BITS   0x03


/**
 * @brief A structure representing the instruction format in a bitfield of 24 bits.
 */
typedef struct 
{
    unsigned int opcode      : 6;  /* Opcode field (6 bits). */
    unsigned int src_mode    : 2;  /* Source Addressing Mode (2 bits). */
    unsigned int src_reg     : 3;  /* Source Register (3 bits). */
    unsigned int dest_mode   : 2;  /* Destination Addressing Mode (2 bits). */
    unsigned int dest_reg    : 3;  /* Destination Register (3 bits). */
    unsigned int funct       : 5;  /* Function Code (5 bits). */
    unsigned int are         : 3;  /* A-R-E bits (Absolute, Relocatable, External) (3 bits). */
} wordfield;

/**
 * @brief Prints the binary representation of each bit in the given wordfield in a grouped format.
 * @param[in] word A pointer to the wordfield structure to print.
 */
void print_wordfield(wordfield* word);

/**
 * @brief Allocates and initializes a new wordfield structure to all zeros.
 * If the allocation fails, it logs an error and returns NULL.
 * @return Pointer to the newly allocated wordfield, or NULL on failure.
 */
wordfield* init_wordfield();

/**
 * @brief Creates and initializes a new wordfield based on an instruction name using the given instruction table.
 * @param[in] str               A C-string representing the name of the instruction (e.g., "mov", "add").
 * @param[in] instruction_table A pointer to an 'InstructionTable' that contains instruction definitions.
 * @return Pointer to a newly allocated and initialized wordfield, or NULL on failure.
 */
wordfield* create_wordfield_by_opname(char* str, InstructionTable* instruction_table);

/**
 * @brief Sets the fields of the given wordfield from a 21-bit number, and separately sets its ARE bits.
 * @param[out] wf  A pointer to the wordfield structure to modify.
 * @param[in]  num A 21-bit integer representing the desired fields.
 * @param[in]  are The ARE bits to set (3 bits).
 */
void set_wordfield_are_num(wordfield* wf, unsigned int num, unsigned int are);

/**
 * @brief Sets the fields of the given wordfield from a 24-bit integer (containing all fields including ARE).
 * @param[out] wf  A pointer to the wordfield structure to modify.
 * @param[in]  num A 24-bit integer representing the desired fields.
 */
void set_wordfield_by_num(wordfield* wf, unsigned int num);

/**
 * @brief Sets the opcode and function code fields in the specified wordfield.
 * @param[out] word   A pointer to the wordfield structure to modify.
 * @param[in]  opcode The opcode value (6 bits).
 * @param[in]  funct  The function code value (5 bits).
 */
void set_wordfield_op_funct(wordfield* word, unsigned int opcode, unsigned int funct);

/**
 * @brief Sets the source addressing mode and source register fields in the specified wordfield.
 * @param[out] word     A pointer to the wordfield structure to modify.
 * @param[in]  src_mode The source addressing mode (2 bits).
 * @param[in]  src_reg  The source register (3 bits).
 */
void set_wordfield_src(wordfield* word, unsigned int src_mode, unsigned int src_reg);

/**
 * @brief Sets the destination addressing mode and destination register fields in the specified wordfield.
 * @param[out] word       A pointer to the wordfield structure to modify.
 * @param[in]  dest_mode  The destination addressing mode (2 bits).
 * @param[in]  dest_reg   The destination register (3 bits).
 */
void set_wordfield_dest(wordfield* word, unsigned int dest_mode, unsigned int dest_reg);

/**
 * @brief Sets the A-R-E bits in the specified wordfield.
 * @param[out] word A pointer to the wordfield structure to modify.
 * @param[in]  are  The A-R-E bits (3 bits) to set in the structure.
 */
void set_wordfield_are(wordfield* word, unsigned int are);

/**
 * @brief Converts all fields of a wordfield into a single 24-bit integer.
 * @param[in] wf A pointer to the wordfield structure whose fields should be combined.
 * @return A 24-bit integer representing the entire wordfield.
 */
int wordfield_to_int(wordfield* wf);

#endif
