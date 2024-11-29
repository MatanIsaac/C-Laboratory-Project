#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "common.h"
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

int read_line(FILE* fp, char *line)
{
    int ch, i = 0;

    /* Read characters until we encounter a new line '\n' or EOF */
    while ((ch = getc(fp)) != EOF && ch != NEW_LINE)
    {
        if(i < MAX_LINE) /* to ensure not going over character per line */
        {
            line[i++] = ch;
        }
        else
            break;
    }
    
    /* if no characters were read and EOF is reached, return 0 */
    if (i == 0 && ch == EOF)
    {
        return 0; 
    }

    line[i] = '\0';

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
int parse_macros(FILE* fp)
{
    char* word                  = calloc(MAX_WORD,sizeof(char));
    char* current_macro_key     = calloc(MAX_WORD,sizeof(char));
    char* current_macro_value   = calloc(MAX_LINE,sizeof(char));
    HashTable* macro_table      = isaac_hashtable_create(10);
    

    while(read_word(fp,word) != EOF)
    {
        if(strcmp("mcro",word) != 0)
        {
            /* 
                TODO: 
                1. put words into a the new .asm file as long as its not a macro definition or call
                2. if its a macro call, replace in the new file with the definition, print to stdout for now.            
            */
            const char* temp = isaac_hashtable_get(macro_table,word);
            if(temp != NULL)
            {
                printf("Macro Label Is: %s\n",word);
                printf("Macro Def Is: %s\n",temp);
            }
            continue;
        }
        
        /* 
            read the next word should be the macro label 
            NOTE: do not insert the macro label to the new file, or "mcro" / "mcroend"
            replace with macro definition. 
        */
        read_word(fp,word); 

        /* check if already exists in the map. */
        if(isaac_hashtable_get(macro_table,word) != NULL) 
        {
            printf("Macro exists already, macro redefinition");
            return -1;
        }        

        strcpy(current_macro_key,word);
        /* read the next word should be the first word of the macro definition */
        read_word(fp,word);
     
        /* if doesnt exists - insert into a hashtable for macros */
        while(strcmp("mcroend",word) != 0)  /* get the macro definition */
        {
            /* insert into a hashtable for macros */
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