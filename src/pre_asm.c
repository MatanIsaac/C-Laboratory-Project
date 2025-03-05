#include "pre_asm.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "macro_table.h"
#include "logger.h"
#include "error_manager.h"
#include <string.h>
#include <ctype.h>

/*
    NOTE: MUST ENSURE: identify lines longer than 81 including '\n' 
*/

int parse_macros(FILE* fp, char* filepath, char* output_file, MacroTable* macro_table)
{
    int position    = 0;
    int flag        = 0;
    char* line      = string_calloc(MAX_LINE, sizeof(char));
    char* word      = string_calloc(MAX_WORD, sizeof(char));
    FILE* new_fp    = prepare_am_file(filepath,output_file);

    if(new_fp == NULL)
    {
        free(word);
        free(line);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
        return -1;
    }

    while(read_line(fp,line) != -1)
    {
        position = 0;
    
        flag = check_line_length(line);
        if(flag == -1)
            add_error_entry(ErrorType_InvalidLineLength,__FILE__,__LINE__);

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
                    if(is_instruction(word) || is_directive(word) || is_register(word))
                    {
                        flag = -1;
                        add_error_entry(ErrorType_InvalidMacroName,__FILE__,__LINE__);
                    }
                    temp = macro_table_get(macro_table,word);
                    if (temp == NULL && flag != -1)
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

    /* TODO: print errors if found and return accourdingly */
    if(is_errors_array_empty() < 0)
    {
        print_errors_array();
        flag = -1;
    }
    else
    {
        log_out(__FILE__,__LINE__,"\n\nAlright Alright Alright!\n\n");
    }

    return flag;
}


int handle_new_macro(FILE* fp,MacroTable* macro_table, char* macro_name)
{
    int flag                    = 0;
    int position                = 0;
    char* line                  = string_calloc(MAX_LINE, sizeof(char));
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    char* current_macro_value   = string_calloc(MAX_LINE, sizeof(char));

    if(read_word_from_line(line, word, position) != -1)
    {
        log_error(__FILE__,__LINE__, "Found extraneous text after macro definition\n");
        /*
        NOTE: found error, but no need to free, mark error and continue.
        free(line);
        free(word);
        free(current_macro_value);
        */
        flag = -1;
        add_error_entry(ErrorType_ExtraneousText,__FILE__,__LINE__);
    }

    while(read_line(fp, line) != -1)
    {
        if(strstr(line,"mcroend") == NULL)
        {
            /*
                TODO: Check for errors in macro definition ? 
            */
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
                add_error_entry(ErrorType_ExtraneousText,__FILE__,__LINE__);
            }
            break;
        }
    }
                
    macro_table_insert(macro_table, macro_name, current_macro_value);
    free(line);
    free(word);
    free(current_macro_value);
    return flag;
}

FILE* prepare_am_file(char* file, char* output_file)
{
    FILE* new_fp;
    size_t total_length;
    char* file_path;
    char* filename      = get_filename(file);
    char* output_path   = "build/output_files/";
    
    size_t filename_len = strlen(filename);
    filename[filename_len - 1] = 'm';

    total_length = strlen(output_path) + filename_len + 1;
    file_path = string_malloc(total_length);
    if (!file_path) 
    {
        log_error(__FILE__,__LINE__, "Memory allocation failed for file_path\n");
        add_error_entry(ErrorType_MemoryAllocationFailure,__FILE__,__LINE__);
        return NULL;
    }
    
    sprintf(file_path, "%s%s",output_path, filename);

    new_fp = fopen(file_path, "w+");
    if (!new_fp) 
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for pre_asm output\n.", file_path);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
        free(file_path);
        return NULL;
    }
    
    strcpy(output_file,file_path);
    free(file_path);
    return new_fp;
}

int check_line_length(char* line)
{
    return (strlen(line) < MAX_LINE) ? 0 : -1;
}