#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "common.h"
#include "macro_table.h"
#include "pre_asm.h"
#include "utility.h"
#include "first_pass.h"
#include "label_table.h"
#include "isaac_logger.h"

int main(int argc,char* argv[])
{
    FILE* fp;
    char current_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    int file_index          = 1;
    int arg_count           = argc;
    MacroTable* macro_table = macro_table_create(10);


    if(argc < 2)
    {
        log_error(__FILE__,__LINE__,"Usage: build/assembler <filename1> <filename2> ...");
        return -1;
    }

    while(--arg_count > 0)
    {
        strcpy(current_file,argv[file_index]);
        strcat(current_file, ".as");
        log_out(__FILE__, __LINE__,"opening filename: %s\n",current_file);
        fp = fopen(current_file,"r");
        if(fp == NULL)
        {
            log_error(__FILE__,__LINE__,"Failed to open %s, file doesn't exists.\n", current_file);
            return -1;
        }

        if(parse_macros(fp,current_file,output_file,macro_table) != -1)
        {
            log_out(__FILE__,__LINE__,"Done Parsing Macros for - %s\nMacro-Table:\n", current_file);
            macro_table_print(macro_table);
            fclose(fp);           
            /* 
                preprares first pass and executes it, 
                if theres no errors, it continues to the 2nd pass 
            */
            prepare_first_pass(output_file,macro_table);
        }
        else
        {
            log_out(__FILE__,__LINE__,"Error Parsing Macros for - %s\n", current_file);
            fclose(fp);
        }

        file_index++;
        macro_table_reset(&macro_table);
    }

    return 0;
}