#include "wordfield.h"
#include "input.h"
#include "utility.h"
#include "isaac_logger.h"
#include <stdlib.h>

/* Function to print each bit manually in the correct order */
void print_wordfield(wordfield* w)
{
    int i;
    
    if(w == NULL)
        return;
    
    printf("|");
    for (i = 5; i >= 0; i--) { printf("%d", (w->opcode >> i) & 1); }
    printf("|");
    for (i = 1; i >= 0; i--) { printf("%d", (w->src_mode >> i) & 1); }
    printf("|");
    for (i = 2; i >= 0; i--) { printf("%d", (w->src_reg >> i) & 1); }
    printf("|");
    for (i = 1; i >= 0; i--) { printf("%d", (w->dest_mode >> i) & 1); }
    printf("|");
    for (i = 2; i >= 0; i--) { printf("%d", (w->dest_reg >> i) & 1); }
    printf("|");
    for (i = 4; i >= 0; i--) { printf("%d", (w->funct >> i) & 1); }
    printf("|");
    for (i = 2; i >= 0; i--) { printf("%d", (w->are >> i) & 1); }
    printf("|");
    printf("\n");
}

wordfield* init_wordfield()
{
    wordfield* word = calloc(1,sizeof(wordfield));
    if (word == NULL)
    {
        log_error(__FILE__,__LINE__,"Failed to allocate memory for wordfield\n");
        return NULL;
    }
    return word;
}

void set_wordfield_are_num(wordfield* wf, unsigned int num, unsigned int are)
{
    if (!wf) 
        return;

    wf->opcode      = (num >> 15) & 0x3F;  /* Extract bits 3-8 (6 bits) */
    wf->src_mode    = (num >> 13) & 0x03;  /* Extract bits 9-10 (2 bits) */
    wf->src_reg     = (num >> 10) & 0x07;  /* Extract bits 11-13 (3 bits) */
    wf->dest_mode   = (num >> 8)  & 0x03;  /* Extract bits 14-15 (2 bits) */
    wf->dest_reg    = (num >> 5)  & 0x07;  /* Extract bits 16-18 (3 bits) */
    wf->funct       = (num >> 0)  & 0x1F;  /* Extract bits 19-23 (5 bits) */
    wf->are         = are;                   /* Manually set first 3 bits */
}

void set_wordfield_by_num(wordfield* wf, unsigned int num)
{
    if (!wf) 
        return;

    /* Extract 6 bits (0x3F is binary 111111),  bits 23 to 18. */
    wf->opcode      = (num >> 18) & MASK_SIX_BITS;  

    /* Extract 2 bits (0x03 is binary 11), bits 17 to 16. */
    wf->src_mode    = (num >> 16) & MASK_TWO_BITS;  

    /* Extract 3 bits ( 0x07 is binary 111), bits 15 to 13. */
    wf->src_reg     = (num >> 13) & MASK_THREE_BITS;

    /* Extract 2 bits (0x03 is binary 11), bits 12 to 11. */
    wf->dest_mode   = (num >> 11)  & MASK_TWO_BITS;

    /* Extract 3 bits (0x07 is binary 11), bits 10 to 8. */
    wf->dest_reg    = (num >> 8)  & MASK_THREE_BITS;

    /* Extract 5 bits (0x1F is binary 11111), bits 7 to 3. */
    wf->funct       = (num >> 3)  & MASK_FIVE_BITS; 

    /* Extract 3 bits (0x03 is binary 11), bits 2 to 0. */
    wf->are         = (num >> 0)  & MASK_THREE_BITS; 
}

void set_wordfield_op_funct(wordfield* word, unsigned int opcode, unsigned int funct)
{
    if(word == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't set wordfield, word is NULL!\n");
        return;
    }

    word->opcode    = opcode;  /* 000000 */
    word->funct     = funct;  /* 00000 */
}

void set_wordfield_src(wordfield* word, unsigned int src_mode, unsigned int src_reg)
{
    if(word == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't set wordfield, word is NULL!\n");
        return;
    }

    word->src_mode  = src_mode;  /* 00 */
    word->src_reg   = src_reg;  /* 000 */
}

void set_wordfield_dest(wordfield* word, unsigned int dest_mode, unsigned int dest_reg)
{
    if(word == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't set wordfield, word is NULL!\n");
        return;
    }

    word->dest_mode = dest_mode;  /* 00 */
    word->dest_reg  = dest_reg;  /* 000 */
}

void set_wordfield_are(wordfield* word, unsigned int are)
{
    if(word == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't set wordfield, word is NULL!\n");
        return;
    }
    word->are = are;  /* 000 */
}

wordfield* create_wordfield_by_opname(char* str, InstructionTable* instruction_table)
{
    InstructionNode* node;
    wordfield* wf;

    if(str == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't create wordfield by op, str is NULL!\n");
        return NULL;
    }
    else if(instruction_table == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't create wordfield by op, instruction_table is NULL!\n");
        return NULL;
    }

    wf = init_wordfield();
    node = instruction_table_get(instruction_table,str);
    
    if(node && is_instruction(str))
    {
        set_wordfield_op_funct(wf,node->op_code,node->funct);
        return wf;
    }

    free(wf); 
    return NULL;
}