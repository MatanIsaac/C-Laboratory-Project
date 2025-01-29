#include "first_pass.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "wordfield.h"
#include "isaac_logger.h"

void prepare_first_pass(const char* filepath)
{
    FILE* fp;
    LabelTable label_table;
    InstructionTable instruction_table;

    instruction_table_create(&instruction_table);
    instruction_table_insert(&instruction_table, "mov",  0,  0);
    instruction_table_insert(&instruction_table, "cmp",  1,  0);
    instruction_table_insert(&instruction_table, "add",  2,  1);
    instruction_table_insert(&instruction_table, "sub",  2,  2);
    instruction_table_insert(&instruction_table, "lea",  4,  0);
    instruction_table_insert(&instruction_table, "clr",  5,  1);
    instruction_table_insert(&instruction_table, "not",  5,  2);
    instruction_table_insert(&instruction_table, "inc",  5,  3);
    instruction_table_insert(&instruction_table, "dec",  5,  4);
    instruction_table_insert(&instruction_table, "jmp",  9,  1);
    instruction_table_insert(&instruction_table, "bne",  9,  2);
    instruction_table_insert(&instruction_table, "jsr",  9,  3);
    instruction_table_insert(&instruction_table, "red", 12,  0);
    instruction_table_insert(&instruction_table, "prn", 13,  0);
    instruction_table_insert(&instruction_table, "rts", 14,  0);
    instruction_table_insert(&instruction_table, "stop",15,  0);
    
    /*instruction_table_print(&instruction_table);*/
    
    label_table_create(&label_table);
    /*label_table_print(&label_table);*/

    log_out(__FILE__,__LINE__, "firstpass: opening filename: %s\n", filepath);
    fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", filepath);
    }

    if(execute_first_pass(fp,&label_table,&instruction_table) >= 0) /* success */
    {
        log_out(__FILE__,__LINE__, "Done First-Pass for [%s]\n.", filepath);
    }
    else /* first pass failed */
    {
        log_error(__FILE__,__LINE__, "Failed First-Pass for [%s]\n.", filepath);
    }
}   


int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table)
{
    int flag        = 0;  /* flag is used to signal if we encounted errors while executing first pass */
    unsigned int DC = 0; /* Data Counter */
    unsigned int IC = 100; /* Instruction Counter */
    char* line      = string_calloc(MAX_LINE, sizeof(char)); 
    char* word      = string_calloc(MAX_WORD, sizeof(char));


    while(read_line(fp,line) != -1)
    {
        int position = 0;
        if(line[0] == SEMICOLON || is_line_empty(line))
        {
            /* ignore comments and empty lines */
            continue;
        }
        while ((position = read_word_from_line(line, word, position)) != -1) 
        {
            if(is_instruction(word))
            {
                /*char* str;
                int temp = 0;
                temp = position - strlen(word);
                log_out(__FILE__,__LINE__, "instruction word position: [%d]\n",temp);

                str = strncpy_from_pos(line,position);
                log_out(__FILE__,__LINE__, "copied word: [%s]\n",str);                
                */
                int operands_count = 0;
                wordfield* wf = init_wordfield();
                
                log_out(__FILE__,__LINE__, 
                    "instruction line: [%s]\n\tinstruction word: [%s]\n\taddress [%u]\n"
                    ,line, word,IC);
                
                operands_count = get_operands_count(word);
                if(operands_count > 0)
                {
                    wf = create_wordfield_by_opname(word, instruction_table);
                }
                else if(operands_count == 0)
                {
                    wf = create_wordfield_by_opname(word, instruction_table);
                }
                set_wordfield_are(wf,4);
                print_wordfield(wf);
                
                position = read_word_from_line(line, word, position);
                if(word[strlen(word)-1] != COMMA)
                {
                    log_error(__FILE__,__LINE__,"Missing comma after first operand!.\n");
                    flag = -1;
                }
                word = remove_last_character(word);
                log_out(__FILE__,__LINE__, "instruction operand1: [%s]",word);
                /* 
                    TODO: 
                        1. create a function for getting operand type: 0,1,2 or 3 
                        2. check operand 1 and its src_mode and src_reg -
                        - determine if we need 1+ wordfield or not. 
                        add it or not, accordingly.
                        3. do the same for operand2 - if its a label yet to be defined - 
                        to be check in the second pass.
                */

                /* 
                TODO: 
                    1.
                    identify which lines need 1,2 or 3 wordfields.
                    and update IC accordinigly.
                    2. 
                    add address + wordfield + additional wordfield (depends on the operand number)
                    to the dynamically allocated binary table. 
                */
            }
            else if(is_directive(word))
            {

            }
            else
            {
                /*
                    TODO: 
                    1. make sure this symbol check is correct
                    2. make sure to check if symbol already exists - handle errors
                */
                if(strrchr(word,':'))
                {
                    
                    char* name;
                    name = remove_last_character(word);
                    label_table_add(label_table,name,IC,CODE); /* check if already exists in inside label_table_add() */
                }
            }
        }
        IC++;
    }
    printf("\n\n");
    label_table_print(label_table);
    return flag;
}

int get_operands_count(char* str)
{
    if(str == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't get wordfield count, str is NULL!.\n");
        return -1;
    }

    if(!is_instruction(str))
    {
        return -1;
    }

    if(strcmp(str,"stop") == 0 || strcmp(str,"rts") == 0)
    {
        
        return 0;
    }
    if(strcmp(str,"mov") == 0 || strcmp(str,"cmp") == 0 || 
        strcmp(str,"add") == 0 || strcmp(str,"sub") == 0 || 
        strcmp(str,"lea") == 0)
    {
        return 2;
    }
    return 1;
}