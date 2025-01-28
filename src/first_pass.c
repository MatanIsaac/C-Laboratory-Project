#include "first_pass.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "instruction_table.h"
#include "wordfield.h"
#include "isaac_logger.h"

void prepare_first_pass(const char* filepath)
{
    /*
    InstructionTable op_table;
    instruction_table_create(&op_table);
    instruction_table_insert(&op_table, "mov",  0,  0);
    instruction_table_insert(&op_table, "cmp",  1,  0);
    instruction_table_insert(&op_table, "add",  2,  1);
    instruction_table_insert(&op_table, "sub",  2,  2);
    instruction_table_insert(&op_table, "lea",  4,  0);
    instruction_table_insert(&op_table, "clr",  5,  1);
    instruction_table_insert(&op_table, "not",  5,  2);
    instruction_table_insert(&op_table, "inc",  5,  3);
    instruction_table_insert(&op_table, "dec",  5,  4);
    instruction_table_insert(&op_table, "jmp",  9,  1);
    instruction_table_insert(&op_table, "bne",  9,  2);
    instruction_table_insert(&op_table, "jsr",  9,  3);
    instruction_table_insert(&op_table, "red", 12,  0);
    instruction_table_insert(&op_table, "prn", 13,  0);
    instruction_table_insert(&op_table, "rts", 14,  0);
    instruction_table_insert(&op_table, "stop",15,  0);
    instruction_table_print(&op_table);
    instruction_table_destroy(&op_table);
    */
    
    FILE* fp;
    LabelTable label_table;
    label_table_create(&label_table);
    label_table_print(&label_table);

    log_out(__FILE__,__LINE__, "firstpass: opening filename: %s\n", filepath);
    fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", filepath);
    }

    if(execute_first_pass(fp,&label_table) >= 0) /* success */
    {
        log_error(__FILE__,__LINE__, "Done First-Pass for [%s]\n.", filepath);
    }
    else /* first pass failed */
    {
        log_error(__FILE__,__LINE__, "Failed First-Pass for [%s]\n.", filepath);
    }
}   


int execute_first_pass(FILE* fp, LabelTable* label_table)
{
    int flag        = 0;
    unsigned int DC = 0;
    unsigned int IC = 100;
    char* line      = string_calloc(MAX_LINE, sizeof(char));
    char* word      = string_calloc(MAX_WORD, sizeof(char));


    while(read_line(fp,line) != -1)
    {
        int position = 0;
        if(line[0] == SEMICOLON || is_line_empty(line))
        {
            /* ignore these kind of lines and update IC */
            IC++;
            continue;
        }
        while ((position = read_word_from_line(line, word, position)) != -1) 
        {
            if(is_instruction(word))
            {
                char* str;
                /*wordfield* wf;*/
                int temp = 0;
                
                log_out(__FILE__,__LINE__, "instruction line: [%s]\n",line);
                log_out(__FILE__,__LINE__, "instruction word: [%s]\n",word);
                
                temp = position - strlen(word);
                log_out(__FILE__,__LINE__, "instruction word position: [%d]\n",temp);

                str = strncpy_from_pos(line,temp);
                log_out(__FILE__,__LINE__, "copied word: [%s]\n",str);
                /*
                wf = create_wordfield(str);
                print_wordfield(wf)
                ;*/
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
                    log_out(__FILE__,__LINE__, "name: %s\n", name);
                    label_table_add(label_table,name,IC,CODE);
                }
            }
        }
        IC++;
    }
    label_table_print(label_table);
    return flag;
}