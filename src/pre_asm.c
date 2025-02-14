#include "pre_asm.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "macro_table.h"
#include "isaac_logger.h"
#include <string.h>
#include <ctype.h>

/*
    NOTE: MUST ENSURE: identify lines longer than 81 including '\n' 
*/

int parse_macros(FILE* fp, const char* filepath, char* output_file, MacroTable* macro_table)
{
    int position    = 0;
    int flag        = 0;
    char* line      = string_calloc(MAX_LINE, sizeof(char));
    char* word      = string_calloc(MAX_WORD, sizeof(char));
    FILE* new_fp    = prepare_am_file(filepath,output_file);

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
            if((position = read_word_from_line(line, word, position)) != -1) 
            {
                if(!is_instruction(word) && !is_directive(word) && 
                    !is_register(word) && !is_label(word,false))
                {
                    const char* temp = macro_table_get(macro_table,word);
                    if(temp != NULL) 
                    {
                        fprintf(new_fp,"%s",temp);    
                        continue;
                    }
                }
                fprintf(new_fp,"%s",line);
                fputc(NEW_LINE,new_fp);
            }
        }
        else
        {
            while ((position = read_word_from_line(line, word, position)) != -1) 
            {
                if (strcmp("mcro", word) == 0)
                { 
                    const char* temp;
                    position = read_word_from_line(line, word, position);
                    if(is_instruction(word) || is_directive(word))
                    {
                        log_error(__FILE__,__LINE__, "Macro's name cannot be an instruction or a direective! \n");
                       flag = -1;
                    }
                    temp = macro_table_get(macro_table,word);
                    if (temp == NULL)
                    {
                        flag = handle_new_macro(fp,macro_table,word);
                    }
                }
            }
        }
    }

    fclose(new_fp);
    free(line);
    free(word);

    return flag;
}


int handle_new_macro(FILE* fp,MacroTable* macro_table, char* macro_name)
{
    int flag                    = 0;
    int position                = 0;
    char* line                  = string_calloc(MAX_LINE, sizeof(char));
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    char* current_macro_key     = string_calloc(MAX_WORD, sizeof(char));
    char* current_macro_value   = string_calloc(MAX_LINE, sizeof(char));

    strcpy(current_macro_key, macro_name);

    if(read_word_from_line(line, word, position) != -1)
    {
        log_error(__FILE__,__LINE__, "Found extraneous text after macro definition\n");
        free(line);
        free(word);
        free(current_macro_key);
        free(current_macro_value);
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
            /* skip spaces */
            while(line[i] != NULL_TERMINATOR && isspace(line[i]))
                i++;
            /* skip macro name I.E.: a_mc in mcro a_mc*/
            while(line[i] != NULL_TERMINATOR && !isspace(line[i]))
            {
                i++;
            }
            /* skip spaces */
            while(line[i] != NULL_TERMINATOR && isspace(line[i]))
                i++;
            /* if we encouter something other than space or null terminator its an error. */
            if(line[i] != NULL_TERMINATOR && !isspace(line[i]))
            {
                log_error(__FILE__,__LINE__, "Found extraneous text after macro definition\n");
                flag = -1;
            }
            break;
        }
    }
                
    macro_table_insert(macro_table, current_macro_key, current_macro_value);
    free(line);
    free(word);

    return flag;
}

FILE* prepare_am_file(const char* filepath, char* output_file)
{
    FILE* new_fp;
    char* base_filename;
    size_t total_length;
    char* file_path;
    char* output_path           = "build/output_files/";

    char* filename              = get_filename(filepath);
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
        return NULL;
    }
    
    sprintf(file_path, "%s%s", output_path, base_filename);

    new_fp = fopen(file_path, "w+");
    if (!new_fp) 
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for pre_asm output\n.", file_path);
        free(filename);
        free(base_filename);
        free(file_path);
        return NULL;
    }
    
    strcpy(output_file, file_path);
    return new_fp;
}