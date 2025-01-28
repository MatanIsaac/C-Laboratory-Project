#include "pre_asm.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "macro_table.h"
#include "isaac_logger.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
    TODO: Make sure NOTHING is missing here, necessary checks, etc..
    NOTE: MUST ENSURE:
    FIXME: make sure to identify lines longer than 81 including '\n' 
    1. macro names are correct and cannot be - an instruction / directive.
    2. if there are no additional charaters in LABEL definitions : I.E: MAIN:, LOOP:, END:, STR:, LIST:,
    3. remember: spaces can appear almost everywhere, especially before and after ','
    IMPORTANT: if there is an error in the preprocessor phase
    report to the user the errors, and continue to the next line (if available)

*/

int parse_macros(FILE* fp, const char* filepath, char* output_file)
{
    int position = 0, flag = 0;
    FILE* new_fp;
    char* base_filename;
    size_t total_length;
    char* file_path;
    char* output_path           = "build/output_files/";
    char* line                  = string_calloc(MAX_LINE, sizeof(char));
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    char* current_macro_key     = string_calloc(MAX_WORD, sizeof(char));
    char* current_macro_value   = string_calloc(MAX_LINE, sizeof(char));
    MacroTable* macro_table      = macro_table_create(10);
    
    char* filename        = get_filename(filepath);
    size_t base_len             = strlen(filename);
    base_filename               = string_malloc(base_len + 4); /* +4 for ".am" and null terminator */
    strcpy(base_filename, filename);
    strcat(base_filename, ".am");

    total_length = strlen(output_path) + strlen(base_filename) + 2;
    file_path = string_malloc(total_length);
    if (!file_path) 
    {
        log_error(__FILE__,__LINE__, "Memory allocation failed for file_path\n");
        free(filename);
        free(base_filename);
        free(line);
        free(word);
        free(current_macro_key);
        free(current_macro_value);
        macro_table_destroy(macro_table);
        return -1;
    }
    
    sprintf(file_path, "%s%s", output_path, base_filename);

    /* Open new output file */
    new_fp = fopen(file_path, "w+");
    if (!new_fp) 
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for pre_asm output\n.", file_path);
        free(filename);
        free(base_filename);
        free(file_path);
        free(line);
        free(word);
        free(current_macro_key);
        free(current_macro_value);
        macro_table_destroy(macro_table);
        return -1;
    }
    
    strcpy(output_file, file_path);

    while(read_line(fp,line) != -1)
    {
        position = 0;
        
        if(line[0] == SEMICOLON || is_line_empty(line))
        {
            fprintf(new_fp,"%s",line);
            fputc(NEW_LINE,new_fp);
            continue;
        }

        if(strstr(line,"mcro") == NULL)
        {
            while ((position = read_word_from_line(line, word, position)) != -1) 
            {
                const char* temp = macro_table_get(macro_table,word);
                if(temp == NULL)
                {
                    fprintf(new_fp,"%s",line);
                    fputc(NEW_LINE,new_fp);
                    break;   
                }
                else
                {
                    fprintf(new_fp,"%s",temp);    
                    break;
                }
            }
            continue;
        }
        else
        {
            while ((position = read_word_from_line(line, word, position)) != -1) 
            {
                if (strcmp("mcro", word) == 0)
                { 
                    const char* temp = macro_table_get(macro_table,word);
                    if (temp == NULL)
                    {
                        position = read_word_from_line(line, word, position);

                        /* 
                            TODO: Make sure to check here if macro name is valid
                            'mcro label'
                                 ^ - must be space, make sure its valid ?  
                        */
                        if(is_instruction(word) || is_directive(word))
                        {
                            log_error(__FILE__,__LINE__, "Macro's name cannot be an instruction or a direective! \n");
                            flag = -1;
                        }
                        
                        strcpy(current_macro_key, word);

                        if(read_word_from_line(line, word, position) != -1)
                        {
                            log_error(__FILE__,__LINE__, "Found extraneous text after macro definition\n");
                            flag = -1;
                        }

                        while(read_line(fp, line) != -1)
                        {
                            if(strstr(line,"mcroend") == NULL)
                            {
                                strcat(current_macro_value,line);
                                strcat(current_macro_value,"\n");
                            }
                            else
                            {
                                int i = 0;
                                while(line[i] != NULL_TERMINATOR && isspace(line[i]))
                                    i++;

                                while(line[i] != NULL_TERMINATOR && !isspace(line[i]))
                                {
                                    i++;
                                }

                                while(line[i] != NULL_TERMINATOR && isspace(line[i]))
                                    i++;

                                if(line[i] != NULL_TERMINATOR && !isspace(line[i]))
                                {
                                    log_error(__FILE__,__LINE__, "Found extraneous text after macro definition\n");
                                    flag = -1;
                                }
                                
                                break;
                            }
                        }
                                    
                        macro_table_insert(macro_table, current_macro_key, current_macro_value);
                        memset(current_macro_key, 0, MAX_WORD);
                        memset(current_macro_value, 0, MAX_LINE);                        
                    }
                }
            }
        }
    }

    fclose(new_fp);
    free(filename);
    free(base_filename);
    free(file_path);
    free(line);
    free(word);
    free(current_macro_key);
    free(current_macro_value);
    macro_table_destroy(macro_table);
    return flag;
}
