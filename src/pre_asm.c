#include "pre_asm.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "macro_table.h"
#include "logger.h"
#include "error_manager.h"
#include <string.h>
#include <ctype.h>

int parse_macros(FILE* fp, char* filepath, char* output_file, MacroTable* macro_table)
{
    int position        = 0; /* needed for reading word at a time from a line */
    int flag            = 0; /* tracks errors */
    int line_count      = 0; /* tracks line-no for error management */
    char* line          = string_calloc(MAX_LINE, sizeof(char)); /* holds entire lines */
    char* word          = string_calloc(MAX_WORD, sizeof(char)); /* holds specific word within a line */
    char* current_file  = my_strdup(filepath); 
    FILE* new_fp        = prepare_am_file(current_file,output_file); /* am file is deleted later if we found any errors */
    free(current_file); 
    
    if(new_fp == NULL)
    {
        free(word);
        free(line);
        add_error_entry(ErrorType_OpenFileFailure,filepath,line_count);
        return -1;
    }

    while(read_line(fp,line) != -1)
    {
        position = 0;
        line_count++;

        /* checks line length */
        flag = check_line_length(line);
        if(flag == -1)
            add_error_entry(ErrorType_InvalidLineLength,filepath,line_count);

        /* skip line if empty or line is a comment */
        if(line[0] == SEMICOLON || is_line_empty(line))
        {
            fprintf(new_fp,"%s",line);
            fputc(NEW_LINE,new_fp);
            continue;
        }

        if(strstr(line,"mcro") == NULL) /* if mcro is not within the line */
        {
            if((position = read_word_from_line(line, word, position)) != -1) /* read the line one word at a time */
            {
                /* check if its a macro call, */
                if(!is_instruction(word) && !is_directive(word) && 
                    !is_register(word) && !is_label(word,false))
                {
                    /* if its not a register/instruction/directive/label - check if its a macro call */
                    const char* temp = macro_table_get(macro_table,word); /* try to get the macro */
                    if(temp != NULL) 
                    {
                        fprintf(new_fp,"%s",temp);    
                        continue;
                    }
                    else /* error: temp == NULL no macro found in macro table */
                    {
                        flag = -1;
                        add_error_entry(ErrorType_InvalidMacro_NotFound,filepath,line_count);
                        continue;
                    }
                }
                /* if its a register/instruction/directive/label simply add it to the new file */
                fprintf(new_fp,"%s",line);
                fputc(NEW_LINE,new_fp);
            }
        }
        else /* mcro is within the line */
        {
            while ((position = read_word_from_line(line, word, position)) != -1) /* read the line one word at a time */
            {
                char needed_space = word[4]; /* needed space position */
                if(strncmp("mcro", word,4) == 0 && strcmp("mcroend", word) != 0) /* if the word is starting with 'mcro' and is not 'mcroend' */
                { 
                    if(!isspace(needed_space) && needed_space != NULL_TERMINATOR) /* check for that needed space and track an error if not found */
                    {
                        flag = -1;
                        add_error_entry(ErrorType_InvalidMacro_MissingSpace,filepath,line_count);
                    }
                    
                    position = read_word_from_line(line, word, position);
                    if(position == -1)
                    {
                        flag = -1;
                        add_error_entry(ErrorType_InvalidMacro_MissingName,filepath,line_count);
                    }

                    /* 
                        looks for extra text after an invalid mcro definition and 
                        in the case of a valid definition checks if macro's name is valid - not a register etc..  
                    */
                    check_macro_name(word, &flag, filepath, &line_count); 
                
                    if (macro_table_get(macro_table,word) == NULL && flag != -1) /* get the macro value if everything is valid until now */
                    {
                        char temp[MAX_WORD];
                        if((position = read_word_from_line(line, temp, position)) != -1)
                        {
                            flag = -1;
                            add_error_entry(ErrorType_ExtraneousText_Macro,filepath,line_count);
                        }
                        flag = handle_new_macro(fp,macro_table,word,filepath,&line_count);
                    }

                }
            }
        }
    }

    fclose(new_fp);
    free(word);
    free(line);

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


int handle_new_macro(FILE* fp,MacroTable* macro_table, char* macro_name,char* filepath, int* line_count)
{
    int flag                    = 0;
    char* line                  = string_calloc(MAX_LINE, sizeof(char));
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    char* current_macro_value   = string_calloc(MAX_LINE, sizeof(char));

    while(read_line(fp, line) != -1)
    {
        (*line_count)++;
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
                add_error_entry(ErrorType_ExtraneousText_Macro,filepath,*line_count);
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
    char* filename              = get_filename(file);
    char* output_path           = "build/output_files/";
    size_t filename_len         = strlen(filename);
    filename[filename_len - 1]  = 'm';
    total_length                = strlen(output_path) + filename_len + 1;
    file_path                   = string_malloc(total_length);
    
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

void check_macro_name(const char* word, int* flag, char* filepath, int* line_count)
{
    int macro_length = strlen(word);
    if(macro_length > MAX_MACRO_LENGTH)
    {
        *flag = -1;
        add_error_entry(ErrorType_InvalidMacroName_Length,filepath,*line_count);
    }

    if(is_instruction(word))
    {
        *flag = -1;
        add_error_entry(ErrorType_InvalidMacroName_Instruction,filepath,*line_count);
    }
    if(is_directive(word))
    {
        *flag = -1;
        add_error_entry(ErrorType_InvalidMacroName_Directive,filepath,*line_count);
    }
    if(is_register(word))
    {
        *flag = -1;
        add_error_entry(ErrorType_InvalidMacroName_Register,filepath,*line_count);
    }

}