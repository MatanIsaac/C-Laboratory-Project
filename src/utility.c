#include "utility.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char* get_filename(const char* file)
{
    const char* last_slash;
    const char* filename;
    const char* last_dot;
    size_t name_length;
    char* result;
    
    if (file == NULL)
        return NULL;

    last_slash = strrchr(file, '\\');
    if (!last_slash) 
    {
        last_slash = strrchr(file, '/');
    }

    filename = last_slash ? last_slash + 1 : file;

    last_dot = strrchr(filename, '.');

    name_length = last_dot ? (size_t)(last_dot - filename) : strlen(filename);

    result = (char*)malloc(name_length + 1);
    if (!result)
        return NULL;

    strncpy(result, filename, name_length);
    result[name_length] = '\0';

    return result;
}

char* string_calloc(size_t element_count,size_t size_of_element)
{
    char* str = calloc(element_count,size_of_element);
    if(str == NULL)
    {
        perror("String memory allocation failed\n");
        return NULL;
    }
    return str;
}

char* string_malloc(size_t size)
{
    char* str = (char*)malloc(size);
    if(str == NULL)
    {
        perror("String memory allocation failed\n");
        return NULL;
    }
    return str;
}


char* my_strdup(const char* s) 
{
    size_t len;
    char *copy = NULL;

    if (s == NULL) 
        return NULL;

    len = strlen(s) + 1;
    copy = malloc(len);

    if (copy) 
    {
        memcpy(copy, s, len);
    }
    return copy;
}

char* remove_last_character(const char *s)
{
    char* str;
    int len = strlen(s);

    str = string_calloc(len-1,sizeof(char));
    strncpy(str,s,len-1);
    if (str == NULL) 
        return NULL;
    
    str[len-1] = '\0';

    return str;
}

char* remove_first_character(char* s)
{
    char* str;
    int len = strlen(s); 

    str = string_calloc(len,sizeof(char));
    str = strncpy_from_pos(s,1);
    if (str == NULL) 
        return NULL;
    
    str[len] = '\0';

    return str;
}

char* strncpy_from_pos(char* src,unsigned int pos)
{
    char* str;
    int i = 0, j = 0, len = 0;

    if(src == NULL)
        return NULL;
    
    len = (strlen(src)-pos);
    str = string_calloc(len,sizeof(char));
    
    i = pos;
    j = 0;
    while(j <= len)
    {
        str[j++] = src[i++];
    }
    str[j] = '\0';
    return str;
}

bool is_register(char* word)
{
    int i;
    static char* registers[MAX_REGISTERS] = { "r0", "r1","r2", "r3", "r4", "r5", "r6", "r7"};

    if(word == NULL)
        return false;
    
    for(i = 0; i < MAX_REGISTERS; i++)
    {   
        if(strcmp(word,registers[i]) == 0)
        {
            return true;
        }   
    }
    return false;
}

bool is_instruction(char* word)
{
    int i;
    static const char* instructions[MAX_INSTRUCTIONS] = 
    { "mov", "cmp","add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    for(i = 0; i < 16; i++)
    {   
        if(strcmp(word,instructions[i]) == 0)
        {
            return true;
        }   
    }
    return false;
}

bool is_directive(char* word)
{
    int i;
    static const char* directives[4] = 
    { ".data", ".string",".extern", ".entry"};
    for(i = 0; i < 4; i++)
    {   
        if(strcmp(word,directives[i]) == 0)
        {
            return true;
        }   
    }
    return false;
}

bool is_line_empty(char* line) 
{
    int i = 0;
    while (line[i] != NULL_TERMINATOR) 
    { 
        if (!isspace((unsigned char)line[i])) 
        {
            return false;
        }
        i++;
    }
    return true;
}

unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

int get_instruction_index(const char *s)
{
    switch (s[0])
    {
        case 'a':  /* add */
            if (s[1]=='d' && s[2]=='d' && s[3]=='\0')
                return 2;  /* "add" */
            break;

        case 'b':  /* bne */
            if (s[1]=='n' && s[2]=='e' && s[3]=='\0')
                return 10; /* "bne" */
            break;

        case 'c':  /* cmp, clr */
            if (s[1]=='m' && s[2]=='p' && s[3]=='\0')
                return 1;  /* "cmp" */
            if (s[1]=='l' && s[2]=='r' && s[3]=='\0')
                return 5;  /* "clr" */
            break;

        case 'd':  /* dec */
            if (s[1]=='e' && s[2]=='c' && s[3]=='\0')
                return 8;  /* "dec" */
            break;

        case 'i':  /* inc */
            if (s[1]=='n' && s[2]=='c' && s[3]=='\0')
                return 7;  /* "inc" */
            break;

        case 'j':  /* jmp, jsr */
            if (s[1]=='m' && s[2]=='p' && s[3]=='\0')
                return 9;  /* "jmp" */
            if (s[1]=='s' && s[2]=='r' && s[3]=='\0')
                return 11; /* "jsr" */
            break;

        case 'l':  /* lea */
            if (s[1]=='e' && s[2]=='a' && s[3]=='\0')
                return 4;  /* "lea" */
            break;

        case 'm':  /* mov */
            if (s[1]=='o' && s[2]=='v' && s[3]=='\0')
                return 0;  /* "mov" */
            break;

        case 'n':  /* not */
            if (s[1]=='o' && s[2]=='t' && s[3]=='\0')
                return 6;  /* "not" */
            break;

        case 'p':  /* prn */
            if (s[1]=='r' && s[2]=='n' && s[3]=='\0')
                return 13; /* "prn" */
            break;

        case 'r':  /* red, rts */
            if (s[1]=='e' && s[2]=='d' && s[3]=='\0')
                return 12; /* "red" */
            if (s[1]=='t' && s[2]=='s' && s[3]=='\0')
                return 14; /* "rts" */
            break;

        case 's':  /* sub, stop */
            if (s[1]=='u' && s[2]=='b' && s[3]=='\0')
                return 3;  /* "sub" */
            if (s[1]=='t' && s[2]=='o' && s[3]=='p' && s[4]=='\0')
                return 15; /* "stop" */
            break;
    }

    return -1;
}
