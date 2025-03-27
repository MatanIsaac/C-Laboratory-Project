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

    V 1.  Create Output Files (.ob, .ext, .ent)
            a. Open or create each file upfront.
            b. Keep track of whether you actually write anything to each file. If you don’t write any extern or entry symbols, you can safely remove the .ext or .ent file at the end.

    V 2.  Iterate Over the Binary Table 
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
    size_t i;
    FILE* ob_file;
    FILE* ent_file;
    FILE* ext_file;
    prepare_output_files(filepath,&ob_file,&ent_file,&ext_file);

    for (i = 0; i < binary_table->size; i++) 
    {
        int index;
        BinaryNode* binary_node = binary_table->data[i];
        if(binary_node->unresolved_label != NULL)
        {
            char* temp_unresolved_label = my_strdup(binary_node->unresolved_label);
            if(binary_node->unresolved_label[0] == AMPERSAND)
            {
                strcpy(temp_unresolved_label,binary_node->unresolved_label);
                remove_first_character(binary_node->unresolved_label);
            }
            else
                free(temp_unresolved_label);

            if((index = label_table_search(label_table,binary_node->unresolved_label)) != INVALID_RETURN)
            {
                LabelNode label_node = label_table->labels[index];

                if(temp_unresolved_label[0] == AMPERSAND)
                {
                    handle_distance_to_label(binary_node,&label_node);
                    
                    if(temp_unresolved_label)
                    {
                        free(temp_unresolved_label);
                        free(binary_node->unresolved_label);
                        binary_node->unresolved_label = NULL;
                    }
                    continue;
                }

                switch (label_node.type)
                {
                case LABELTYPE_CODE:
                    set_wordfield_are_num(binary_node->word,label_node.address,ARE_RELOCATABLE);  
                    break;
                case LABELTYPE_DATA:
                    set_wordfield_are_num(binary_node->word,label_node.address,ARE_RELOCATABLE);
                    break;
                case LABELTYPE_EXTERN:
                    set_wordfield_are(binary_node->word,ARE_EXTERNAL);
                    if(ext_file)
                    {
                        /* this line still does not add it to the ext_file, why ?*/
                        fprintf(ext_file,"%s %.7d\n",binary_node->unresolved_label,binary_node->address); 
                    }
                    break;
                case LABELTYPE_CODE_ENTRY:
                    /* 
                        FIXME: problems with the unresolved label address of entry types.
                        if(ent_file)
                        {
                            fprintf(ent_file,"%s %.7d\n",binary_node->unresolved_label,binary_node->address);
                        }
                    */
                    break;
                case LABELTYPE_DATA_ENTRY:
                    /* 
                        FIXME: problems with the unresolved label address of entry types. 
                    if(ent_file)
                    {
                        fprintf(ent_file,"%s %.7d\n",binary_node->unresolved_label,binary_node->address); 
                    }
                    */
                    break;
                default:
                    break;
                }

                if(binary_node->unresolved_label)
                {
                    free(binary_node->unresolved_label);
                    binary_node->unresolved_label = NULL;
                }
            }
            else
            {
                add_error_entry(ErrorType_InvalidLabel_UndefinedLabel,NULL,binary_node->address);
            }
        }
    }

    /* 
        TODO: 
        1. Close files if they are empty - keep the file destination in order to remove it by name.
    */
    fclose(ext_file);
    fclose(ent_file);
    fclose(ob_file);

    printf("\n\n");
    binary_table_print(binary_table);
    printf("\n\n");
    label_table_print(label_table);
    printf("\n\n");
    
    if(is_errors_array_empty() < 0)
    {
        print_errors_array();
        return INVALID_RETURN;
    }
    else
    {
        log_out(__FILE__,__LINE__,"\n\nSECOND-PASS Alright Alright Alright!\n\n");
    }

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

void handle_distance_to_label(BinaryNode* binary_node, LabelNode* node)
{
    /* LABEL_ADDRESS - CURRENT_OP_ADDRESS = DIFF */
    int distance = node->address - (binary_node->address-1);
    set_wordfield_are_num(binary_node->word,distance,ARE_ABSOLUTE);
}

