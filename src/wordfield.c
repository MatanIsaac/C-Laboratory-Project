#include "wordfield.h"
#include "input.h"
#include "utility.h"

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

void set_wordfield_op_funct(wordfield* word, unsigned int opcode, unsigned int funct)
{
    word->opcode    = opcode;  /* 000000 */
    word->funct     = funct;  /* 00000 */
}
void set_wordfield_src(wordfield* word, unsigned int src_mode, unsigned int src_reg)
{
    word->src_mode  = src_mode;  /* 00 */
    word->src_reg   = src_reg;  /* 000 */
}
void set_wordfield_dest(wordfield* word, unsigned int dest_mode, unsigned int dest_reg)
{
    word->dest_mode = dest_mode;  /* 00 */
    word->dest_reg  = dest_reg;  /* 000 */
}
void set_wordfield_are(wordfield* word, unsigned int are)
{
   word->are = are;  /* 000 */
}

/* 
    gets as a parameter example lines like:
add r3, LIST
prn #48
lea STR, r6
inc r6
mov r3,K
sub r1, r4
bne END
cmp K, #-6
bne &END
dec K
jmp &LOOP
stop
.string “abcd”
.data 6, -9
.data -100
.data 31

    and creates a matching wordfield for such line.
*/
wordfield* create_wordfield(const char* str)
{

    /*
    wordfield wf;
        TODO: set wordfield by type one by one :
        first op code and funct, then src, then dest, then are.
    set_wordfield_op_funct();
    set_wordfield_src();
    set_wordfield_dest();
    set_wordfield_are();*/

    return NULL;
    /*
    wordfield wf;
    char* word = string_calloc(MAX_WORD, sizeof(char));
    int position = 0;
    while ((position = read_word_from_line(str, word, position)) != -1)
    {
        if(is_instruction(word))
        {
            
        }
        else if(is_directive(word))
        {

        }
    }
    */     
}