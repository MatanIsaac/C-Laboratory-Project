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
    FILE* fp;
    int file_index = 1;
    int arg_count = argc;

    if(argc < 2)
    {
        fprintf(stderr,"Usage: build/assembler.exe <filename1> <filename2> ...");
        return -1;
    }

    while(--arg_count > 0)
    {
        const char* current_file = argv[file_index];
        printf("opening filename: %s\n",argv[file_index]);
        fp = fopen(argv[file_index],"r");
        if(fp == NULL)
        {
            printf("Failed to open %s\n", argv[file_index]);
            return -1;
        }

        if(parse_macros(fp,current_file) < 0)
        {
            printf("\nDone Parsing Macros.\n");
            return -1;
        }

        /*
        printf("argc: %d\n", arg_count);
        printf("file index: %d\n", file_index);
        */
        file_index++;
    }

    return 0;
}