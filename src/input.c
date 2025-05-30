#include "input.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int read_word(FILE* fp, char *word)
{
    int ch, i = 0;

    /*  skip leading whitespace */
    while ((ch = getc(fp)) != EOF && isspace(ch));

    /* check if we hit EOF directly */
    if (ch == EOF)
        return EOF;

    /* Read characters until we encounter a space or EOF */
    while (ch != EOF && !isspace(ch) && i < MAX_LINE) 
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
    if (i < 0)
    {
        return INVALID_RETURN;
    }
    
    while (line[i] != NULL_TERMINATOR && isspace(line[i])) 
    {
        i++;
    }

    while (line[i] != NULL_TERMINATOR && !isspace(line[i])) 
    {
        if (j < MAX_WORD) 
        {
            word[j++] = line[i];
        }
        i++;
    }

    word[j] = NULL_TERMINATOR;
    if (j == 0) 
    {
        return INVALID_RETURN;
    }

    return i; 
}

int read_line(FILE* fp, char *line)
{
    int ch, i = 0;

    /* Read characters until we encounter a new line '\n' or EOF */
    while ((ch = getc(fp)) != EOF && ch != NEW_LINE)
    {
        if(i < MAX_LINE-1) /* to ensure not going over character per line */
        {
            line[i++] = ch;
        }
        else
        {
            /* We've reached max allowed chars, so we discard rest of line to prevent overflow */
            while ((ch = getc(fp)) != EOF && ch != NEW_LINE);
            break;
        }
    }

    line[i] = '\0';
    
    /* if no characters were read and EOF is reached */
    if (i == 0 && ch == EOF)
    {
        return INVALID_RETURN; 
    }    

    return VALID_RETURN;
}
