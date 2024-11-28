#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "common.h"
#include "isaac_hashtable.h"

/* 
    #: MAKE SURE THERE ISN'T ANY DIFFERENT FONT CHARACTERS LIKE:
        '”' instead of '"'
*/

int main(int argc,char* argv[])
{
    char line[MAX_LINE];

    FILE* fp;
    int file_index = 1;

    if(argc < 2)
    {
        perror("Usage: build/assembler.exe <filename1> <filename2> ...");
        return -1;
    }
    printf("opening filename: %s\n",argv[file_index]);
    if(argc > 1)
    {
        fp = fopen(argv[file_index],"r+");
        if(fp == NULL)
        {
            printf("Failed to open %s\n", argv[file_index]);
            return -1;
        }

        while(read_line(fp,line) > 0)
        {
            printf("line:\n%s\n",line);
        }

        if(file_index < argc)
            file_index++; 
    }

    return 0;
}