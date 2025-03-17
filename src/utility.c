#include "utility.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "error_manager.h"

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
        root_folder[len] = NULL_TERMINATOR;
        return root_folder;
    }
    
    strncpy(root_folder, file, i);
    root_folder[i] = NULL_TERMINATOR;
    

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

    s[len-1] = NULL_TERMINATOR;
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
    s[len-1] = NULL_TERMINATOR;
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
    
    str[j] = NULL_TERMINATOR; 
    return str;
}

char* int_to_hex(int number)
{
    /*
        NOTE:
        Allocate enough space for up to 8 hex digits, and the null terminator.
        That’s 8 (for 32-bit hex digits) + 1 = 9 characters.
    */
    char* hex_str = malloc(9 * sizeof(char));
    if (hex_str == NULL) 
    {
        return NULL; 
    }
    
    sprintf(hex_str, "%06x", number);
    
    return hex_str;
}

void str_to_lower(char* str)
{
    int i = 0;
    int length = strlen(str);
    for (; i < length; i++)
    {
        /* We cast to unsigned char in order to avoid issues if char is signed */
        unsigned char ch = (unsigned char)str[i];
        if (isupper(ch)) 
        {
            /* tolower() returns an int, so we cast it back to char */
            str[i] = (char)tolower(ch);
        }
    }
}

int is_label(const char* word)
{
    int length;
    int i = 0;
    if(word == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't check label, word is null\n");
        return INVALID_RETURN;
    }
    
    length = strlen(word);
    if((length + 1) > MAX_LABEL_LENGTH) /* + 1 for '\0' */
    {
        /*
        log_error(__FILE__,__LINE__,"Invalid label!, too long, must be <= 31\n");
        */
        return INVALID_RETURN;
    }

    if(length == 1)
    {
        if(isalpha(word[0]) && word[0] != UNDERSCORE)
        {
            return VALID_RETURN;   
        }
        /*
        log_error(__FILE__,__LINE__,"Invalid label!, invalid first letter of label, must be uppercase/lowercase letter.\n");
        */
        return INVALID_RETURN;
    }

    if (is_register(word) != INVALID_RETURN)
    {
        /*
        log_error(__FILE__,__LINE__,"Invalid label!, can't be a register!\n");
        */
        return INVALID_RETURN;
    }

    if(!isalpha(word[0]) && word[0] != UNDERSCORE) /* make sure first letter is legal */
    {
        /*
        log_error(__FILE__,__LINE__,"Invalid label!, invalid first letter of label, must be uppercase/lowercase letter.\n");
        */
        return INVALID_RETURN;
    }
    
    i++; /* skip first letter of label */
    /* check other letters not including last letter and null terminator */
    for(; i < length-1; i++)
    {
        if(!isalpha(word[i]) && !isdigit(word[i]) && word[i] != UNDERSCORE)
        {
            /*
            log_error(__FILE__,__LINE__,"Invalid label!, must consist of uppercase/lowercase letters and/or numbers/underscore.\n");
            */
            return INVALID_RETURN;
        } 
    }

    return VALID_RETURN;
}

int is_register(const char* word)
{
    int i;
    char* str = my_strdup(word);
    static char* registers[MAX_REGISTERS] = { "r0", "r1","r2", "r3", "r4", "r5", "r6", "r7"};

    if(word == NULL)
    {
        free(str);
        return INVALID_RETURN;
    }
    
    for(i = 0; i < MAX_REGISTERS; i++)
    {   
        str_to_lower(str);
        if(strcmp(word,registers[i]) == 0)
        {
            free(str);
            return VALID_RETURN;
        }   
    }
    free(str);
    return INVALID_RETURN;
}

int is_instruction(const char* word)
{
    int i;
    char* str = my_strdup(word);
    static const char* instructions[MAX_INSTRUCTIONS] = 
    { "mov", "cmp","add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    
    if(word == NULL)
    {
        free(str);
        return INVALID_RETURN;
    }

    for(i = 0; i < MAX_INSTRUCTIONS; i++)
    {   
        str_to_lower(str);
        if(strcmp(str,instructions[i]) == 0)
        {
            free(str);
            return VALID_RETURN;
        }   
    }
    free(str);
    return INVALID_RETURN;
}

int is_directive(const char* word)
{
    int i;
    char* str = my_strdup(word);
    static const char* directives[MAX_DIRECTIVES] = 
    { ".data", ".string",".extern", ".entry", "data", "string", "extern", "entry"};
    
    if(word == NULL)
    {
        free(str);
        return INVALID_RETURN;
    }

    for(i = 0; i < MAX_DIRECTIVES; i++)
    {   
        if(strcmp(word,directives[i]) == 0)
        {
            free(str);
            return VALID_RETURN;
        }   
    }
    free(str);
    return INVALID_RETURN;
}

int is_line_empty(char* line) 
{
    int i = 0;
    while (line[i] != NULL_TERMINATOR) 
    { 
        if (!isspace((unsigned char)line[i])) 
        {
            return INVALID_RETURN;
        }
        i++;
    }
    return VALID_RETURN;
}

int is_valid_number(char* word)
{
    if (word == NULL || *word == NULL_TERMINATOR) 
    {
        return INVALID_RETURN; 
    }

    if (*word == DASH) 
    {
        ++word;
        if (*word == NULL_TERMINATOR) 
        {
            return INVALID_RETURN; 
        }
    }

    for (; *word != NULL_TERMINATOR; ++word) 
    {
        if (!isdigit((unsigned char)*word)) 
        {
            log_error(__FILE__,__LINE__,"Error reading immediate value, NAN.\n");
            return INVALID_RETURN;
        }
    }

    return 1;
}

