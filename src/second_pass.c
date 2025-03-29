#include "second_pass.h"
#include "error_manager.h"
#include "logger.h"
#include "common.h"
#include "utility.h"

int prepare_second_pass(const char* filepath,BinaryTable* binary_table, LabelTable* label_table, int ICF, int DCF)
{
    int flag;
    if((flag = execute_second_pass(binary_table,label_table,ICF,DCF,filepath)) != INVALID_RETURN)    
    {
        log_out(__FILE__,__LINE__, "Done Second-Pass for [%s]\n.", filepath);
        return VALID_RETURN;
    }
    
    /* else second pass failed */
    log_error(__FILE__,__LINE__, "Failed Second-Pass for [%s]\n.", filepath);
    return flag;
}

int execute_second_pass(BinaryTable* binary_table,LabelTable* label_table, int ICF, int DCF,const char* filepath)
{
    int flag;
    FILE* ob_file;
    FILE* ent_file;
    FILE* ext_file;
    char* ob_filename   = NULL;
    char* ent_filename  = NULL;
    char* ext_filename  = NULL;
    prepare_output_files(filepath,&ob_file,&ent_file,&ext_file,&ob_filename,&ent_filename,&ext_filename);

    fprintf(ob_file,"\t%d %d\n",ICF,DCF);
    flag = complete_first_pass(binary_table,label_table,&ob_file,&ext_file);

    if(flag != INVALID_RETURN)
        handle_entries(label_table, &ent_file);

    if(is_file_empty(ext_file) != INVALID_RETURN || flag == INVALID_RETURN)
    {
        fclose(ext_file);
        remove(ext_filename);
    }
    else
        fclose(ext_file);
    
    if(is_file_empty(ent_file) != INVALID_RETURN || flag == INVALID_RETURN)
    {
        fclose(ent_file);
        remove(ent_filename);
    }
    else    
        fclose(ent_file);
    
    if(is_file_empty(ob_file) != INVALID_RETURN || flag == INVALID_RETURN)
    {
        fclose(ob_file);
        remove(ob_filename);
    }
    else
        fclose(ob_file);

    free(ob_filename);
    free(ent_filename);
    free(ext_filename);
    
    if(is_errors_array_empty() == INVALID_RETURN)
    {
        print_errors_array();
        binary_table_print(binary_table);
        printf("\n\n");
        label_table_print(label_table);
        return INVALID_RETURN;
    }
    else
    {
        log_out(__FILE__,__LINE__,"\n\nSECOND-PASS Alright Alright Alright!\n\n");
    }

    printf("\n\n");
    binary_table_print(binary_table);
    printf("\n\n");
    label_table_print(label_table);
    printf("\n\n");

    return VALID_RETURN;
}

/*
 * NOTE: 
 *      We use double pointers (FILE**) so that this function can
 *      initialize the file pointers passed by the caller.
 *      In order to avoid warnings about uninitialized file pointers
 *      and to clarify each function's responsibility.
 */
void prepare_output_files(const char* filepath, FILE** fp_ob, FILE** fp_ent, FILE** fp_ext,
    char** ob_filename,char** ent_filename,char** ext_filename)
{
    char* output_path       = OUTPUT_PATH;
    size_t filename_length  = strlen(filepath);
    char* file_path         = my_strdup(filepath);
    char* filename          = get_filename(file_path);
    *ob_filename             = string_calloc(sizeof(filename_length), sizeof(char));
    *ent_filename            = string_calloc(sizeof(filename_length) + 1, sizeof(char));
    *ext_filename            = string_calloc(sizeof(filename_length) + 1, sizeof(char));

    file_path[filename_length - 2]  = 'o';
    file_path[filename_length - 1]  = 'b';
    sprintf(*ob_filename, "%s%s",output_path,filename);
        
    *fp_ob = fopen(*ob_filename, "w+");
    if(*fp_ob == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", *ob_filename);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    file_path[filename_length - 2]  = 'e';
    file_path[filename_length - 1]  = 'n';
    file_path[filename_length]      = 't';
    file_path[filename_length + 1]  = NULL_TERMINATOR;
    sprintf(*ent_filename, "%s%s",output_path,filename);
    
    *fp_ent = fopen(*ent_filename, "w+");
    if(*fp_ent == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", *ent_filename);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    file_path[filename_length - 2]  = 'e';
    file_path[filename_length - 1]  = 'x';
    file_path[filename_length]      = 't';
    file_path[filename_length + 1]  = NULL_TERMINATOR;
    sprintf(*ext_filename, "%s%s",output_path,filename);

    *fp_ext = fopen(*ext_filename, "w+");
    if(*fp_ext == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", *ext_filename);
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    if(file_path)
        free(file_path);  
}

void handle_distance_to_label(BinaryNode* binary_node, LabelNode* node)
{
    int distance = node->address - (binary_node->address-1);
    set_wordfield_are_num(binary_node->word,distance,ARE_ABSOLUTE);
}

int complete_first_pass(BinaryTable* binary_table,LabelTable* label_table,FILE** ob_file,FILE** ext_file)
{
    int i, flag;
    for (i = 0; i < binary_table->size; i++) 
    {
        int index,wordfield_number;
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
                    if(ext_file && *ext_file)
                    {
                        /* this line still does not add it to the ext_file, why ?*/
                        fprintf(*ext_file,"%s %.7d\n",binary_node->unresolved_label,binary_node->address); 
                    }
                    break;
                case LABELTYPE_CODE_ENTRY: /* entries are handled later. */
                    break;
                case LABELTYPE_DATA_ENTRY: /* entries are handled later, we fill the necessary bits of the wordfield */
                    set_wordfield_are_num(binary_node->word,label_node.address,ARE_RELOCATABLE);
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
                flag = INVALID_RETURN;
            }
        }
        wordfield_number = wordfield_to_int(binary_node->word);
        fprintf(*ob_file,"%.7d %s\n",binary_node->address,int_to_hex(wordfield_number));
    }

    return flag;
}

void handle_entries(LabelTable* label_table, FILE** ent_file)
{
    int i;
    if(ent_file == NULL)
    {
        return;
    }

    for (i = 0; i < label_table->size; i++) 
    {
        LabelNode label_node = label_table->labels[i];
        switch (label_node.type)
        {
        case LABELTYPE_CODE_ENTRY:
            if(*ent_file)
            {
                fprintf(*ent_file,"%s %.7d\n",label_node.name,label_node.address);
            }
            break;
        case LABELTYPE_DATA_ENTRY:
            if(*ent_file)
            {
                fprintf(*ent_file,"%s %.7d\n",label_node.name,label_node.address);
            }
            break;
        default:
            break;
        }
    }
}

int is_file_empty(FILE* file)
{
    long size;
    long current_pos;

    /* Save current position */ 
    current_pos = ftell(file);       
    /* Go to end */
    fseek(file, 0, SEEK_END);             
    /* Get position = file size */
    size = ftell(file);              
    /* Restore original position */
    fseek(file, current_pos, SEEK_SET);   
    
    if(size == 0)
        return VALID_RETURN;
    
    return INVALID_RETURN;
}