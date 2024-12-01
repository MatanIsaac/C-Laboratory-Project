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


int read_word_from_line(const char* line, int* start_index, char* word) 
{
    int i = *start_index, j = 0;

    /* Skip leading whitespaces */
    while (line[i] && isspace(line[i])) {
        i++;
    }

    /* If we reached the end of the line */
    if (line[i] == '\0') 
    {
        return 0; 
    }

    /* Read the word */
    while (line[i] && !isspace(line[i])) 
    {
        word[j++] = line[i++];
    }

    word[j] = '\0';

    /* update start index for the next word */
    *start_index = i;

    return 1;
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
    TODO: 
    There are 2 situations:

    1. found a new macro definition
        a. check if already exists in the map.
        b. if new, add to a macro hash table
        c. if exists already, give error -> macro was defined twice 

    2. found a macro tag (already define)
        a. look for it in the table
        b. if exists, parse and use the value of the macro.
        c. if unknown, give error -> macro undefined.
*/
int parse_macros(FILE* fp, const char* filepath)
{
    FILE* new_fp;
    char* word                  = calloc(MAX_WORD,sizeof(char));
    char* current_macro_key     = calloc(MAX_WORD,sizeof(char));
    char* current_macro_value   = calloc(MAX_LINE,sizeof(char));
    HashTable* macro_table      = isaac_hashtable_create(10);
    
    char *base_filename         = get_filename(filepath);
    char* final_name            = strcat(base_filename,"_output.asm");

    new_fp = fopen(final_name,"w");

    while(read_word(fp,word) != EOF)
    {
        if(strcmp("mcro",word) != 0)
        {
            const char* temp = isaac_hashtable_get(macro_table,word);
            if(temp != NULL)
            {
                printf("Macro Label Is: %s\n",word);
                printf("Macro Def Is: %s\n",temp);
                fprintf(new_fp,"%s",temp);
                continue;
            }
            fprintf(new_fp,"%s",word);
            continue;
        }
        
        read_word(fp,word); 

        
        if(isaac_hashtable_get(macro_table,word) != NULL) 
        {
            printf("Macro exists already, macro redefinition");
            return -1;
        }        

        strcpy(current_macro_key,word);

        read_word(fp,word);

        while(strcmp("mcroend",word) != 0)  
        {
            strcat(current_macro_value,word);
            strcat(current_macro_value," ");
            read_word(fp,word);
        }
        if(strcmp("mcroend",word) == 0)
        {
            isaac_hashtable_insert(macro_table, current_macro_key, current_macro_value);
            isaac_hashtable_print(macro_table);
        }

        memset(current_macro_key, 0, MAX_WORD);
        memset(current_macro_value, 0, MAX_LINE);
    }
    
    return 0;
}
