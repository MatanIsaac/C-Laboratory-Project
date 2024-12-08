#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "common.h"
#include "utility.h"
#include "isaac_hashtable.h"

int read_word(FILE* fp, char *word)
{
    int ch, i = 0;

    /*  skip leading whitespace */
    while ((ch = getc(fp)) != EOF && isspace(ch));

    /* check if we hit EOF directly */
    if (ch == EOF)
        return EOF;

    /* Read characters until we encounter a space or EOF */
    while (ch != EOF && !isspace(ch)) 
    {
        word[i++]   = ch;
        ch          = getc(fp);
    } 

    word[i] = '\0';

    return 1;
}


int read_word_from_line(const char* line, char* word, int start_pos) 
{
    int i = start_pos;
    int j = 0;

    while (line[i] != '\0' && isspace(line[i])) 
    {
        i++;
    }

    while (line[i] != '\0' && !isspace(line[i])) 
    {
        if (j < MAX_WORD - 1) 
        {
            word[j++] = line[i];
        }
        i++;
    }

    word[j] = '\0';
    if (j == 0) 
    {
        return -1;
    }

    return i; 
}

int read_line(FILE* fp, char *line)
{
    int ch, i = 0;

    /* Read characters until we encounter a new line '\n' or EOF */
    while ((ch = getc(fp)) != EOF && ch != NEW_LINE)
    {
        if(i < MAX_LINE - 1) /* to ensure not going over character per line */
        {
            line[i++] = ch;
        }
    }

    line[i] = '\0';
    
    /* if no characters were read and EOF is reached, return 0 */
    if (i == 0 && ch == EOF)
    {
        return -1; 
    }    

    return 1;
}

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
    char* final_name            = strcat(base_filename,"_output.am");
    const char* output_path     = "output_files/preprocessor/";
    size_t total_length         = strlen(output_path) + strlen(final_name) + 1;
    char* file_path             = string_malloc(total_length);
    snprintf(file_path, total_length, "%s%s_output.am", output_path, base_filename);
    /*-----------------------------------------------------------------------*/

    /* Open new output file to write to */
    new_fp = fopen(file_path,"w");

    while(read_line(fp,line) != -1)
    {
        position = 0;
        
        if(strstr(line,"mcro") == NULL)
        {
            while ((position = read_word_from_line(line, word, position)) != -1)
            {
                const char* temp = isaac_hashtable_get(macro_table,word);
                if(temp == NULL)
                {
                    fprintf(new_fp,"%s",line);
                    fputc(NEW_LINE,new_fp);
                    break;   
                }
                else
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
            while ((position = read_word_from_line(line, word, position)) != -1) 
            {
                if (strcmp("mcro", word) == 0) /* if it is a macro definition */
                {
                    /* check if it exists in the macro table */
                    const char* temp = isaac_hashtable_get(macro_table,word);
                    if (temp == NULL) /* if macro label is not in the macro table */
                    {
                        /* 1. Make sure the macro label is not an instruction or a directive */
                        /* 2. Read next word, which should be the macro's label */
                        position = read_word_from_line(line, word, position);
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