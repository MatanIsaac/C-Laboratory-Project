#include "utility.h"

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