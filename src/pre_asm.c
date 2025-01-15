#include "pre_asm.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "isaac_hashtable.h"

#include <string.h>

/*
    TODO: Make sure NOTHING is missing here, necessary checks, etc..
    NOTE: MUST ENSURE:
    1. macro names are correct and cannot be - an instruction / directive.
    2. if there are no additional charaters in LABEL definitions : I.E: MAIN:, LOOP:, END:, STR:, LIST:,
    IMPORTANT: if there is an error in the preprocessor phase, we can't continue,
    must stop, report to the user the errors, and continue to the next file (if available)
    3. remember: spaces can appear almost everywhere, especially before and after ','
*/

int parse_macros(FILE* fp, const char* filepath)
{
    int position = 0;
    FILE* new_fp;
    char* line                  = string_calloc(MAX_LINE,sizeof(char));
    char* word                  = string_calloc(MAX_WORD,sizeof(char));
    char* current_macro_key     = string_calloc(MAX_WORD,sizeof(char));
    char* current_macro_value   = string_calloc(MAX_LINE,sizeof(char));
    HashTable* macro_table      = isaac_hashtable_create(10);
    
    /* Create sufficient memory for the final file path */
    /* This part is to make sure output files are created in the right place */
    char *base_filename         = get_filename(filepath);
    char* final_name            = strcat(base_filename,".am");
    const char* output_path     = "build/output_files/";
    size_t total_length         = strlen(output_path) + strlen(final_name) + 1;
    char* file_path             = string_malloc(total_length);
    /* 
        IMPORTANT: snprintf was replaced with sprintf - make sure there isn't errors/bugs here.
        * snprintf is not available on ansi c 89 standard 
    */
    sprintf(file_path,"%s%s", output_path, base_filename);
    /*-----------------------------------------------------------------------*/

    /* Open new output file to write to */
    new_fp = fopen(file_path,"w");

    while(read_line(fp,line) != -1)
    {
        position = 0;
        
        /* ignore comments when line starts with ';' */
        if(line[0] == SEMICOLON)
        {
            fprintf(new_fp,"%s",line);
            fputc(NEW_LINE,new_fp);
            continue;
        }

        if(strstr(line,"mcro") == NULL)
        {
            /* read line word by word */
            while ((position = read_word_from_line(line, word, position)) != -1) 
            {
                /* check if current word is a macro call */
                const char* temp = isaac_hashtable_get(macro_table,word);
                if(temp == NULL) /* if line is not, than add line entirely */
                {
                    fprintf(new_fp,"%s",line);
                    fputc(NEW_LINE,new_fp);
                    break;   
                }
                else /* otherwise add macro's value */
                {
                    /*printf("Macro Label Is: %s\n",word);*/
                    /*printf("Macro Def Is: %s\n",temp);*/
                    fprintf(new_fp,"%s",temp);    
                    break;
                }
            }
            continue;
        }
        else /* we have a mcro definition in the line */
        {
            /* read line word by word */
            while ((position = read_word_from_line(line, word, position)) != -1) 
            {
                if (strcmp("mcro", word) == 0) /* if it is a macro definition */
                {
                    /* check if it exists in the macro table */
                    const char* temp = isaac_hashtable_get(macro_table,word);
                    if (temp == NULL) /* if macro label is not in the macro table */
                    {
                        /* Read next word, which should be the macro's label */
                        position = read_word_from_line(line, word, position);

                        /* 
                            Make sure the macro label is not an instruction or a directive 
                            if it is - print error and stop and exit program.
                        */
                        if(is_instruction(word) || is_directive(word))
                        {
                            fprintf(stderr, "Macro's name cannot be an instruction or a direective! \n");
                            fclose(new_fp);
                            remove(file_path);
                            return -1;
                        }
                        
                        strcpy(current_macro_key, word); /* Copy macro label */

                        /* read next line - should be a macro definition line */
                        while(read_line(fp, line) != -1)
                        {
                            if(strstr(line,"mcroend") == NULL)
                            {
                                strcat(current_macro_value,line);
                                strcat(current_macro_value,"\n");
                            }
                            else
                                break;
                        }
                                    
                        isaac_hashtable_insert(macro_table, current_macro_key, current_macro_value);
                        /*isaac_hashtable_print(macro_table);*/

                        memset(current_macro_key, 0, MAX_WORD);
                        memset(current_macro_value, 0, MAX_LINE);
                        
                    }
                }
            }
        }
    }

    fclose(new_fp);
    return 0;
}
