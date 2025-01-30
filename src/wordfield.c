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

/*
    example  

int main(void)
{
    wordfield 
    mov,cmp,add,sub,
    lea,clr,not,inc,
    dec,jmp,bne,jsr,
    red,prn,rts,stop;

    set_wordfield_op_funct(&mov,0,0);
    set_wordfield_op_funct(&cmp,1,0);
    set_wordfield_op_funct(&add,2,1);
    set_wordfield_op_funct(&sub,2,2);
    set_wordfield_op_funct(&lea,4,0);
    set_wordfield_op_funct(&clr,5,1);
    set_wordfield_op_funct(&not,5,2);
    set_wordfield_op_funct(&inc,5,3);
    set_wordfield_op_funct(&dec,5,4);
    set_wordfield_op_funct(&jmp,9,1);
    set_wordfield_op_funct(&bne,9,2);
    set_wordfield_op_funct(&jsr,9,3);
    set_wordfield_op_funct(&red,12,0);
    set_wordfield_op_funct(&prn,13,0);
    set_wordfield_op_funct(&rts,14,0);
    set_wordfield_op_funct(&stop,15,0);

    print_wordfield(&mov);
    print_wordfield(&cmp);
    print_wordfield(&add);
    print_wordfield(&sub);
    print_wordfield(&lea);
    print_wordfield(&clr);
    print_wordfield(&not);
    print_wordfield(&inc);
    print_wordfield(&dec);
    print_wordfield(&jmp);
    print_wordfield(&bne);
    print_wordfield(&jsr);
    print_wordfield(&red);
    print_wordfield(&prn);
    print_wordfield(&rts);
    print_wordfield(&stop);
    return 0;
}
*/

void set_wordfield_by_num(wordfield* wf, unsigned int num)
{
    if (!wf) 
        return;

    wf->opcode      = (num >> 15) & 0x3F;  /* Extract bits 3-8 (6 bits) */
    wf->src_mode    = (num >> 13) & 0x03;  /* Extract bits 9-10 (2 bits) */
    wf->src_reg     = (num >> 10) & 0x07;  /* Extract bits 11-13 (3 bits) */
    wf->dest_mode   = (num >> 8)  & 0x03;  /* Extract bits 14-15 (2 bits) */
    wf->dest_reg    = (num >> 5)  & 0x07;  /* Extract bits 16-18 (3 bits) */
    wf->funct       = (num >> 0)  & 0x1F;  /* Extract bits 19-23 (5 bits) */
    wf->are         = 4;                   /* The first 3 bits are intentionally '100' */
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

    return NULL;
}