#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "common.h"

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
        word[i++] = ch;
        ch = getc(fp);
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
    char word[MAX_LINE];

    while(read_word(fp,word) != EOF)
    {
        printf("word: |-%s-|\n", word);
        if(strcmp("mcro",word) == 0)
        {
            
        }
    }
    return 0;
}