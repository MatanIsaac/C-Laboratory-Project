#include "utility.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "isaac_logger.h"

char* get_filename(char* file)
{
    char* last_slash;
    char* filename;
    
    if (file == NULL)
        return NULL;

    last_slash = strrchr(file, '/');
    

    filename = (last_slash) ? last_slash + 1 : file;

    return filename;
}

char* get_root_folder_name(char* file)
{
    int len = strlen(file);
    int i = len - 1;
    int slash_count = 0;
    char* root_folder = string_calloc(len + 2, sizeof(char)); 
    
    if (file == NULL)
        return NULL;
    
    if (!root_folder)
        return NULL;
    
    for (; i >= 0; i--)
    {
        if (file[i] == '/')
        {
            slash_count++;
            if (slash_count == 3)
                break;
        }
    }

    if (i < 0)
    {
        strncpy(root_folder, file, len);
        root_folder[len] = '\0';
        return root_folder;
    }
    
    strncpy(root_folder, file, i);
    root_folder[i] = '\0';
    

    strcat(root_folder, "/");
    
    return root_folder;
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

void remove_last_character(char *s)
{
    int len = strlen(s);

    if (!s || len == 0)
        return;

    s[len-1] = '\0';
}

void remove_first_character(char* s)
{
    int i = 1;
    int len = strlen(s);

    if (!s || len == 0)
        return;

    while(i < len)
    {    
        s[i-1] = s[i]; 
        i++;
    }
    s[len-1] = '\0';
}

char* strncpy_from_pos(char* src, unsigned int pos)
{
    int len; 
    char* str;
    int i = pos, j = 0;

    if (!src || pos >= strlen(src))
        return NULL;

    len = strlen(src) - pos;
    
    str = string_calloc(len + 1, sizeof(char));
    if (!str)
        return NULL; 

    
    while (j < len)  
    {
        str[j++] = src[i++];
    }
    
    str[j] = '\0'; 
    return str;
}


bool is_label(char* word, int ignore_colon)
{
    int length;
    int i = 0;
    bool flag = true;
    if(word == NULL)
    {
        /*log_error(__FILE__,__LINE__,"Can't check label, word is null\n");*/
        return false;
    }
    
    length = strlen(word);
    if((length + 1) > MAX_LABEL_LEN) /* + 1 for '\0' */
    {
        /*log_error(__FILE__,__LINE__,"Invalid label!, too long, must be <= 31\n");*/
        return false;
    }

    if(ignore_colon == true && length == 1)
    {
        if(isalpha(word[0]))
        {
            return true;   
        }
        /*log_error(__FILE__,__LINE__,"Invalid label!, invalid first letter of label, must be uppercase/lowercase letter.\n");*/
        return false;
    }

    if (is_register(word))
    {
        /*log_error(__FILE__,__LINE__,"Invalid label!, can't be a register!\n");*/
        return false;
    }

    if(!isalpha(word[0])) /* make sure first letter is legal */
    {
        /*log_error(__FILE__,__LINE__,"Invalid label!, invalid first letter of label, must be uppercase/lowercase letter.\n");*/
        return false;
    }
    
    i++; /* skip first letter of label */
    /* check other letters not including last letter and null terminator */
    for(; i < length-1 && flag != false; i++)
    {
        if(!isalpha(word[i]) && !isdigit(word[i]))
        {
            /*log_error(__FILE__,__LINE__,"Invalid label!, must consist of uppercase/lowercase letters and/or numbers.\n");*/
            return false;
        } 
    }
    if(ignore_colon == false && word[i] != COLON)
    {
        /*log_error(__FILE__,__LINE__,"Invalid label!, missing colon ':' at the end of the label.\n");*/
        return false;
    }
    else if(ignore_colon == true && (!isalpha(word[i]) && !isdigit(word[i])))
    {
        /*log_error(__FILE__,__LINE__,"Invalid label!, must consist of uppercase/lowercase letters and/or numbers.\n");*/
        return false;
    } 
    return true;
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

bool is_valid_number(char* word)
{
    if (word == NULL || *word == '\0') 
    {
        return false; 
    }

    if (*word == DASH) 
    {
        ++word;
        if (*word == '\0') 
        {
            return false; 
        }
    }

    for (; *word != '\0'; ++word) 
    {
        if (!isdigit((unsigned char)*word)) 
        {
            log_error(__FILE__,__LINE__,"Error reading immediate value, NAN.\n");
            return false;
        }
    }

    return true;
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
