#include "second_pass.h"
#include "error_manager.h"
#include "logger.h"
#include "common.h"
#include "utility.h"

void prepare_second_pass(const char* filepath,BinaryTable* binary_table, LabelTable* label_table, int ICF, int DCF)
{
    if(execute_second_pass(binary_table,label_table,ICF,DCF,filepath) >= 0)    
    {
        log_out(__FILE__,__LINE__, "Done Second-Pass for [%s]\n.", filepath);
    }
    else /* second pass failed */
    {
        log_error(__FILE__,__LINE__, "Failed Second-Pass for [%s]\n.", filepath);
    }
}

/* 
    TODO: 2nd-Pass Plan 

    1.  Create Output Files (.ob, .ext, .ent)
            a. Open or create each file upfront.
            b. Keep track of whether you actually write anything to each file. If you don’t write any extern or entry symbols, you can safely remove the .ext or .ent file at the end.

    2.  Iterate Over the Binary Table
        For each BinaryNode (or equivalent) that might contain an unresolved label reference, do:
            a.  Check if there’s a label to resolve (e.g., node->unresolved_label != NULL or a special ARE bit).
            b.  Look up the label in your label_table:
                    If the label doesn’t exist, log an error (e.g., “Undefined label”).
                    Otherwise, retrieve the label’s address and type (e.g., data/code, extern, etc.).
            c.  Patch the Instruction:
                    If it’s relative addressing (e.g., OPERAND_TYPE_RELATIVE), calculate the distance (label_address − current_address).
                    If it’s external, set the ARE bits accordingly (mark as External) and write an entry to your .ext file (line + label + address).
                    Otherwise, just set the address bits in the instruction’s wordfield (plus appropriate ARE bits for relocatable vs. absolute).

    3.  Write the Final Machine Code to .ob
            a. After patching is done, you have the final binary (or textual machine code).
            b. Write out the instruction part (IC) followed by the data part (DC), or however your format demands.

    4.  Handle Entries
            a. During the first pass, you typically record which labels are .entry. In the second pass, you can look those up in your label table to get their final addresses.
            b. Write each entry (label + address) to the .ent file.
            c. If you don’t have any entry labels, remove the .ent file.

    5.  Close or Delete Unused Files
            a. If you never wrote anything to .ext, remove it.
            b. Same for .ent.

    6.  Report Errors
            If any label resolution failed, or if there were inconsistencies (e.g., redefinition of labels), this is the time to ensure your assembler reports them.       
*/
int execute_second_pass(BinaryTable* binary_table,LabelTable* label_table, int ICF, int DCF,const char* filepath)
{
    FILE* ob;
    FILE* ent;
    FILE* ext;
    
    prepare_output_files(filepath,&ob,&ent,&ext);


    return VALID_RETURN;
}

/*
 * NOTE: 
 *      We use double pointers (FILE**) so that this function can
 *      initialize the file pointers passed by the caller.
 *      In order to avoid warnings about uninitialized file pointers
 *      and to clarify each function's responsibility.
 */
void prepare_output_files(const char* filepath, FILE** fp_ob, FILE** fp_ent, FILE** fp_ext)
{
    char* output_path       = OUTPUT_PATH;
    size_t filename_length  = strlen(filepath);
    char* file_path         = my_strdup(filepath);
    char* filename          = get_filename(file_path);
    char* ob_file           = string_calloc(sizeof(filename_length), sizeof(char));
    char* ent_file          = string_calloc(sizeof(filename_length) + 1, sizeof(char));
    char* ext_file          = string_calloc(sizeof(filename_length) + 1, sizeof(char));

    file_path[filename_length - 2]  = 'o';
    file_path[filename_length - 1]  = 'b';
    sprintf(ob_file, "%s%s",output_path,filename);
        
    *fp_ob = fopen(ob_file, "w+");
    if(*fp_ob == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", ob_file);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    file_path[filename_length - 2]  = 'e';
    file_path[filename_length - 1]  = 'n';
    file_path[filename_length]      = 't';
    file_path[filename_length + 1]  = NULL_TERMINATOR;
    sprintf(ent_file, "%s%s",output_path,filename);
    
    *fp_ent = fopen(ent_file, "w+");
    if(*fp_ent == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", ent_file);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    file_path[filename_length - 2]  = 'e';
    file_path[filename_length - 1]  = 'x';
    file_path[filename_length]      = 't';
    file_path[filename_length + 1]  = NULL_TERMINATOR;
    sprintf(ext_file, "%s%s",output_path,filename);

    *fp_ext = fopen(ext_file, "w+");
    if(*fp_ext == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", ext_file);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    if(file_path)
        free(file_path);   
    if(ob_file)
        free(ob_file);   
    if(ent_file)
        free(ent_file);   
    if(ext_file)
        free(ext_file);   
}