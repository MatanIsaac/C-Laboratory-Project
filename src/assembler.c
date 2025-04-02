/*
================================================================================
                                ASSEMBLER - MAIN ENTRY POINT
================================================================================
File        : assembler.c
Author      : Matan Itzhak
Date        : April 2, 2025
Description : Entry point for the two-pass assembler program.

Overview:
---------
This assembler processes one or more `.as` assembly source files provided as
command-line arguments. It compiles each file through a multi-stage process to
generate final machine-readable output files.

Processing Stages:
------------------
1. Macro Expansion (Pre-Assembly Phase):
   - Macros are parsed and expanded into a new `.am` file.
   - If an error occurs during this step, the temporary file is deleted.

2. First Pass:
   - Parses the expanded `.am` file.
   - Builds the symbol (label) table.
   - Validates syntax and structure.
   - Collects instruction and directive metadata.

3. Second Pass:
   - Resolves labels and updates binary values.
   - Generates output files:
     - `.ob`  - Object code
     - `.ent` - Entry labels
     - `.ext` - External labels

Design Notes:
-------------
- A shared `MacroTable` is reused across all files, and reset between them.
- Logging is handled via a custom logger that outputs metadata and context.
- All errors are collected into an internal array and printed at the end of each pass.
- Output files are automatically removed if they contain no relevant data.

Usage:
------
./assembler <filename1> <filename2> ...

Notes:
------
- Input filenames must be provided without the `.as` extension.
- The assembler expects well-formed syntax and predefined rules from MMN projects.
 
MEMORY NOTE:
------------
This assembler is estimated to use approximately 30KB (30,000 bytes) of memory in total.
The usage includes:
 - Macro table allocation and expansion
 - Label and instruction tables
 - Binary node storage and wordfield structures
 - Dynamic strings and temporary buffers

The memory footprint remains small and predictable due to the fixed-size and
dynamically growing components (e.g., errors, macros, binary nodes), making it
efficient for handling multiple small-to-medium assembly files.

All memory allocations were thoroughly checked using **Valgrind** to ensure:
 - No memory leaks
 - No use of uninitialized memory
 - No invalid read/write operations
 - No double-free errors: each malloc'd block is freed exactly once

Valgrind was used profusely throughout development as a core part of testing and
debugging the assembler's runtime behavior.
================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "common.h"
#include "macro_table.h"
#include "pre_asm.h"
#include "utility.h"
#include "first_pass.h"
#include "label_table.h"
#include "logger.h"

int main(int argc,char* argv[])
{
    FILE* fp;
    char current_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    int file_index          = 1;
    int arg_count           = argc;
    MacroTable* macro_table;

    if(argc < 2)
    {
        log_error(__FILE__,__LINE__,"Usage: build/assembler <filename1> <filename2> ...");
        return INVALID_RETURN;
    }

    macro_table = macro_table_create(10);

    while(--arg_count > 0)
    {
        strcpy(current_file,argv[file_index]);
        strcat(current_file, ".as");
        log_out(__FILE__, __LINE__,"opening filename: %s\n",current_file);
        fp = fopen(current_file,"r");
        if(fp == NULL)
        {
            log_error(__FILE__,__LINE__,"Failed to open %s, file doesn't exists.\n", current_file);
            file_index++;
            continue;
        }

        if(parse_macros(fp, current_file,output_file,macro_table) != INVALID_RETURN)
        {
            log_out(__FILE__,__LINE__,"Done Parsing Macros for - %s\n", current_file);
            fclose(fp);           
            /* 
                preprares first pass and executes it, 
                and continues to the 2nd pass     
            */
            prepare_first_pass(output_file,macro_table);
        }
        else
        {
            log_out(__FILE__,__LINE__,"Error Parsing Macros for - %s\n", output_file);
            fclose(fp);
            /* Found error in Pre-Asm -> Delete .am File */
            if (remove(output_file) != 0) 
            {
                perror("Failed to delete file");
            }
        }

        file_index++;
        macro_table_reset(&macro_table);
    }

    macro_table_destroy(macro_table);
    return VALID_RETURN;
}
