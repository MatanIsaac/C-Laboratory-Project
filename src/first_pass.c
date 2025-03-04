#include "first_pass.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "logger.h"

#include <ctype.h>

void prepare_first_pass(const char* filepath, MacroTable* macro_table)
{
    FILE* fp;
    LabelTable label_table;
    InstructionTable instruction_table;

    instruction_table_create(&instruction_table);
    instruction_table_insert(&instruction_table, "mov",  0,  0);
    instruction_table_insert(&instruction_table, "cmp",  1,  0);
    instruction_table_insert(&instruction_table, "add",  2,  1);
    instruction_table_insert(&instruction_table, "sub",  2,  2);
    instruction_table_insert(&instruction_table, "lea",  4,  0);
    instruction_table_insert(&instruction_table, "clr",  5,  1);
    instruction_table_insert(&instruction_table, "not",  5,  2);
    instruction_table_insert(&instruction_table, "inc",  5,  3);
    instruction_table_insert(&instruction_table, "dec",  5,  4);
    instruction_table_insert(&instruction_table, "jmp",  9,  1);
    instruction_table_insert(&instruction_table, "bne",  9,  2);
    instruction_table_insert(&instruction_table, "jsr",  9,  3);
    instruction_table_insert(&instruction_table, "red", 12,  0);
    instruction_table_insert(&instruction_table, "prn", 13,  0);
    instruction_table_insert(&instruction_table, "rts", 14,  0);
    instruction_table_insert(&instruction_table, "stop",15,  0);
    
    label_table_create(&label_table);

    log_out(__FILE__,__LINE__, "firstpass: opening filename: %s\n", filepath);
    fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", filepath);
    }

    if(execute_first_pass(fp,&label_table,&instruction_table, macro_table) >= 0) /* success */
    {
        log_out(__FILE__,__LINE__, "Done First-Pass for [%s]\n.", filepath);
        if(fp)
        {
            fclose(fp);
        }
    }
    else /* first pass failed */
    {
        log_error(__FILE__,__LINE__, "Failed First-Pass for [%s]\n.", filepath);
    }
}   


int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table, MacroTable* macro_table)
{
    int flag                    = 0;    /* flag is used to signal if we encounted errors while executing first pass */
    unsigned int DC             = 0;    /* data counter */
    unsigned int TC             = 100;  /* total counter */
    unsigned int DCF            = -1;   /* final data counter */
    unsigned int ICF            = -1;   /* final instruction counter */
    char* line                  = string_calloc(MAX_LINE, sizeof(char)); 
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    BinaryTable* binary_table   = binary_table_create(5);

    while(read_line(fp,line) != -1)
    {
        int position = 0;
        if(line[0] == SEMICOLON || is_line_empty(line))
        {
            /* ignore comments and empty lines */
            continue;
        }

        while ((position = read_word_from_line(line, word, position)) != -1) 
        {
            if(is_instruction(word))
            {
                flag = handle_instruction(binary_table,label_table,instruction_table,&TC,line,word,&position);
            }
            else if(is_directive(word))
            {
                flag = handle_directive(binary_table,label_table,&TC,&DC,line,word,&position);
            }
            else if(is_label(word,false))
            {
                flag = handle_labels(label_table,TC,line,word,position);
            }
        }
    }

    ICF = TC - 100 - DC;
    DCF = DC;

    printf("\n\n");
    printf("\n\n");
    binary_table_print(binary_table);
    printf("\n\n");
    label_table_print(label_table);
    label_table_destroy(label_table);
    free(line);
    free(word);
    binary_table_free(binary_table);

    log_out(__FILE__,__LINE__,"TC: %u\t DC: %u\n",TC,DC);
    log_out(__FILE__,__LINE__,"ICF: %u\t DCF: %u\n",ICF,DCF);
    
    return flag;
}

int get_operands_count(char* str)
{
    if(str == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't get wordfield count, str is NULL!.\n");
        return -1;
    }

    if(!is_instruction(str))
    {
        return -1;
    }

    if(strcmp(str,"stop") == 0 || strcmp(str,"rts") == 0)
    {
        
        return 0;
    }
    if(strcmp(str,"mov") == 0 || strcmp(str,"cmp") == 0 || 
        strcmp(str,"add") == 0 || strcmp(str,"sub") == 0 || 
        strcmp(str,"lea") == 0)
    {
        return 2;
    }
    return 1;
}

OperandType get_operand_type(char* str)
{
    if(str[0] == HASHTAG) 
    {
        return OPERAND_TYPE_IMMEDIATE; /* addressing mode: 0 */
    }
    else if(str[0] == AMPERSAND)
    {
        return OPERAND_TYPE_RELATIVE; /* addressing mode: 2*/
    }
    else if(is_register(str))
    {
        return OPERAND_TYPE_REGISTER; /* addressing mode: 3 */
    }
    else if(is_label(str,true))
    {
        return OPERAND_TYPE_DIRECT;
    }
    
    return -1;
}

DirectiveType get_directive_type(char* str)
{
    if(strcmp(str,".string") == 0) /* .string */
    {
        return DIRECTIVE_TYPE_STRING; 
    }
    else if(strcmp(str,".data") == 0) /* .data 2*/
    {
        return DIRECTIVE_TYPE_DATA; 
    }
    else if(strcmp(str,".extern") == 0) /* .extern */
    {
        return DIRECTIVE_TYPE_EXTERN;
    }
    else if(strcmp(str,".entry") == 0) /* .entry */
    {
        return DIRECTIVE_TYPE_ENTRY;
    }
    
    return -1;
}

wordfield* get_char_wordfield(char* character)
{
    wordfield* wf;
    if(character == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't convert to ascii, character is NULL, returning NULL\n");
        return NULL;
    }

    wf = init_wordfield();
    set_wordfield_by_num(wf,(int)*character);
    return wf;
}

int handle_labels(LabelTable* label_table, unsigned int TC, char* line, char* word,int position)
{
    char temp[MAX_LABEL_LEN];
    char* label_name        = string_calloc(strlen(word)+1, sizeof(char));
    int flag                = 0;
    int temp_position       = position;
    strcpy(label_name,word);
    remove_last_character(label_name);
    
    if(label_table_search(label_table,label_name) != -1)
    {
        log_error(__FILE__,__LINE__,"Label Redefinition - the label [%s] already exists in the label tabel.\n",label_name);
        flag = -1;
    }

    else if(is_instruction(label_name) || is_directive(label_name) || is_register(label_name))
    {
        log_error(__FILE__,__LINE__,"Invalid label name - the label [%s] can't be an instruction or a directive or a register\n",label_name);
        flag = -1;
    }

    /* add the valid label to the label table and set its type as CODE */
    label_table_add(label_table,label_name,TC,LABELTYPE_CODE);
    
    /* if the next word is a directive, change label type to DATA */
    temp_position = read_word_from_line(line, temp, temp_position);
    if(is_directive(temp))
    {
        label_table_set_label_type(label_table,TC,LABELTYPE_DATA);
    }

    return flag;
}

int handle_instruction(BinaryTable* binary_table, LabelTable* label_table,InstructionTable* instruction_table, 
    unsigned int* TC, char* line, char* word, int* position)
{
    int flag            = 0;
    int operands_count  = 0;
    wordfield* wf       = create_wordfield_by_opname(word, instruction_table);
    set_wordfield_are(wf,4);

    operands_count      = get_operands_count(word);
    if(operands_count == -1)
    {
        log_error(__FILE__,__LINE__, "Failed to get operands count!\n");
        free(wf);
    }
    else if(operands_count == 0)
    {
        binary_node_add(binary_table,*TC,line);
        set_binary_node_wordfield(binary_table,*TC,wf);
        free(wf);
        (*TC)++;
    }
    else if (operands_count == 1)
    {
        OperandType operand1_type;
        wordfield* new_wf   = init_wordfield();
        *position           = read_word_from_line(line, word, *position);
        operand1_type       = get_operand_type(word);
        switch (operand1_type)
        {
        case OPERAND_TYPE_IMMEDIATE:
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,wf);
            free(wf);
            (*TC)++;
            remove_first_character(word);
            flag    = is_valid_number(word);
            set_wordfield_are_num(new_wf,atoi(word),4);
            binary_node_add(binary_table,*TC,"Immediate value");
            set_binary_node_wordfield(binary_table,*TC,new_wf);
            free(new_wf);
            (*TC)++;
            break;
        case OPERAND_TYPE_DIRECT:
            /* checks if its a valid label and will print errors accordingly */
            flag = is_label(word,true);
            set_wordfield_dest(wf,OPERAND_TYPE_DIRECT,0);
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,wf);
            free(wf);
            (*TC)++;
            binary_node_add(binary_table,*TC,"Address of Label");
            set_binary_node_wordfield(binary_table,*TC,new_wf);
            free(new_wf);
            (*TC)++;
            break;
        case OPERAND_TYPE_RELATIVE:
            if(word[0] != AMPERSAND)
            {
                log_error(__FILE__,__LINE__, "Error missing '&' before label for relative type addressing!.\n");
                flag = -1;    
            }
            set_wordfield_dest(wf,OPERAND_TYPE_RELATIVE,0);
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,wf);
            free(wf);
            (*TC)++;
            binary_node_add(binary_table,*TC,"Distance to Label");
            set_binary_node_wordfield(binary_table,*TC,new_wf);
            free(new_wf);
            (*TC)++;
            
            /* 
                IMPORTANT: this is relevant only for: jmp, jsr, bne
                TODO:
                1. check if label already exists in label table and declared before
                2. if not - 
                add an empty extra line with *TC to the binary table 
                for the unknown label we've encountered, will be handled in the 2nd pass.
                3. if exists - already declared - calculate distance from current address to label's address :
                if label is in 300 and jmp is 500: jumping distance is -200
                    LABEL_ADDRESS - CURRENT_OP_ADDRESS = DIFF
                set jump instruction wf dest mode to 02 (decimal) relative addressing, dest register 0.
                add a new wordfield, set are field to 100 'A=1,R=0,E=0',
                add DIFF to the new wordfield at the bit 23-3.
                add both new and instruction wordfield to the binary table.                            
            */
            break;
        case OPERAND_TYPE_REGISTER:
            /* checks if its a valid register */
            if(!is_register(word)) 
            {
                log_error(__FILE__,__LINE__,"Register name invalid!\n");
                free(wf);
                return -1;
            }
            remove_first_character(word);
            set_wordfield_dest(wf,OPERAND_TYPE_REGISTER,atoi(word));
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,wf);
            free(wf);
            (*TC)++;
            free(new_wf);
            break;
        default:
            break;
        }

    }
    else /* operands count is 2 */
    {
        OperandType operand1_type, operand2_type;
        wordfield* new_wf1 = NULL; /* = init_wordfield(); */
        wordfield* new_wf2 = NULL; /* = init_wordfield(); */

        *position = read_word_from_line(line, word, *position);
        if(word[strlen(word)-1] != COMMA)
        {
            log_error(__FILE__,__LINE__,"Missing comma after first operand!.\n");
            free(wf);
            return -1;
        }
        remove_last_character(word);
        operand1_type   = get_operand_type(word);
        switch (operand1_type)
        {
        case OPERAND_TYPE_IMMEDIATE:
            remove_first_character(word);
            is_valid_number(word);
            new_wf1 = init_wordfield();
            set_wordfield_are_num(new_wf1,atoi(word),4);
            binary_node_add(binary_table,*TC,line);
            (*TC)++;
            break;
        case OPERAND_TYPE_DIRECT:
            is_label(word,true); /* checks if its a valid label and will print errors accordingly */
            set_wordfield_src(wf,OPERAND_TYPE_DIRECT,0);
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,wf);
            (*TC)++;
            new_wf1 = init_wordfield();
            binary_node_add(binary_table,*TC,"Address of Label");
            set_binary_node_wordfield(binary_table,*TC,new_wf1);
            free(new_wf1);
            (*TC)++;
            break;
        case OPERAND_TYPE_RELATIVE:
            (*TC)++;
            
            /* no use for this if 1st operand is relative - processed in 2nd Pass */
            free(new_wf1);
            break;
        case OPERAND_TYPE_REGISTER:
            /* checks if its a valid register */
            if(!is_register(word)) 
            {
                log_error(__FILE__,__LINE__,"Register name invalid!\n");
                free(new_wf2);
                free(wf);
                return -1;
            }
            remove_first_character(word);
            set_wordfield_src(wf,OPERAND_TYPE_REGISTER,atoi(word));
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,wf);
            (*TC)++;

            /* no use for this if 1st operand is a register */
            free(new_wf1);
            break;
        default:
            break;
        }

        *position = read_word_from_line(line, word, *position);
        if(*position == -1)
        {
            free(new_wf2);
            free(wf);   
            return flag;    
        }
        operand2_type = get_operand_type(word);                    
        switch (operand2_type)
        {
        case OPERAND_TYPE_IMMEDIATE:
            remove_first_character(word); /* removing '#' */
            is_valid_number(word);
            new_wf2 = init_wordfield();
            set_wordfield_are_num(new_wf2,atoi(word),4);
            binary_node_add(binary_table,*TC,"Immediate value");
            set_binary_node_wordfield(binary_table,*TC,new_wf2);
            free(new_wf2);
            free(wf);
            (*TC)++;
            break;
        case OPERAND_TYPE_DIRECT:
            is_label(word,true); /* checks if its a valid label and will print errors accordingly */
            set_wordfield_dest(wf,OPERAND_TYPE_DIRECT,0);
            set_binary_node_wordfield(binary_table,*TC-1,wf);
            new_wf2 = init_wordfield();
            binary_node_add(binary_table,*TC,"Address of Label");
            set_binary_node_wordfield(binary_table,*TC,new_wf2);
            free(new_wf2);
            free(wf);
            (*TC)++;
            break;
        case OPERAND_TYPE_RELATIVE:
            (*TC)++;
            
            /* no use for this if 2nd operand if its relative - processed in 2nd Pass */
            free(new_wf2);
            free(wf);
            break;
        case OPERAND_TYPE_REGISTER:
            /* checks if its a valid register */
            if(!is_register(word)) 
            {
                log_error(__FILE__,__LINE__,"Register name invalid!\n");
                free(wf);
                return -1;
            }
            remove_first_character(word);
            set_wordfield_dest(wf,OPERAND_TYPE_REGISTER,atoi(word));
            if(operand1_type != OPERAND_TYPE_REGISTER)
            {
                set_binary_node_wordfield(binary_table,*TC-2,wf);
                free(wf);
            }
            else /* we did not update *TC if operand1 is register, reset node wordfield to updated one */
            {
                set_binary_node_wordfield(binary_table,*TC-1,wf); 
                free(wf);
            }
            
            /* no use for this if 2nd operand is a register */
            free(new_wf2);
            break;
        default:
            free(wf);
            break;
        }
    } 

    return flag;
}

int handle_directive(BinaryTable* binary_table, LabelTable* label_table,unsigned int* TC,unsigned int* DC,char* line, char* word,int* position)
{
    DirectiveType directive_type;
    int flag                    = 0;
    int str_length              = -1;
    int i                       = 0; 
    int numbers_count           = 0;
    int char_count              = 0;
    char last_num_str[MAX_WORD] = "Integer "; 
    wordfield* last_wf          = init_wordfield(); 
    
    /*
        IMPORTANT: .entry is handled in the 2nd pass. 
    */
    directive_type = get_directive_type(word);
    switch (directive_type)
    {
    case DIRECTIVE_TYPE_STRING:
        *position = read_word_from_line(line, word, *position);
    
        str_length = strlen(word);
        if((word[0] != DOUBLE_QUOTE || word[str_length] != DOUBLE_QUOTE) && (word[0] != DOUBLE_QUOTE && word[str_length] != DOUBLE_QUOTE))
        {
            log_error(__FILE__,__LINE__,"Error reading string data, missing quotes.\n");
            flag = -1;
        }
        remove_first_character(word);
        remove_last_character(word);
        str_length -= 2; /* not including 2 double quotes */

        /* 
            NOTE:
            adding to DC the current length, I.E: "abcd" == 4
            + 1 for the null terminator - '\0'
        */
        *DC += str_length + 1;
        
        for( ; i < str_length; i++)
        {
            wordfield* tmp;
            wordfield char_wf = {0};
            char character[2];
            char ascii_str[MAX_WORD] = "Ascii code ";
            strncpy(character,&word[i],1);
            tmp = get_char_wordfield(character);
            if (tmp != NULL) 
            {
                char_wf = *tmp; 
                free(tmp);       
            }
            
            if(char_count == 0)
            {
                binary_node_add(binary_table,*TC,line);
                set_binary_node_wordfield(binary_table,*TC,&char_wf);
                (*TC)++;
                char_count++;
                continue;
            }
            strncat(ascii_str, character,1);
            binary_node_add(binary_table,*TC,ascii_str);
            set_binary_node_wordfield(binary_table,*TC,&char_wf);
            (*TC)++;
            char_count++;
        }
        binary_node_add(binary_table,*TC,"Ascii code \'\\0\'");
        set_binary_node_wordfield(binary_table,*TC,last_wf);
        free(last_wf);
        (*TC)++;
        break;
    case DIRECTIVE_TYPE_DATA:
        while ((*position = read_word_from_line(line, word, *position)) != -1 && word[strlen(word)-1] == COMMA)
        {
            char final_str[MAX_WORD] = "Integer ";
            wordfield num_wf = {0};
            remove_last_character(word);
            set_wordfield_by_num(&num_wf,(unsigned int)atoi(word));
            if(numbers_count == 0)
            {
                binary_node_add(binary_table,*TC,line);
                set_binary_node_wordfield(binary_table,*TC,&num_wf);
                numbers_count++;
                (*TC)++;
                continue;
            }
            
            strcat(final_str, word);
            binary_node_add(binary_table,*TC, final_str);
            set_binary_node_wordfield(binary_table,*TC,&num_wf);
            (*TC)++;
            numbers_count++;
        }
        if(numbers_count == 0) /* single number I.E: .data 100 */
        {
            set_wordfield_by_num(last_wf,(unsigned int)atoi(word));
            binary_node_add(binary_table,*TC,line);
            set_binary_node_wordfield(binary_table,*TC,last_wf);
            free(last_wf);
            (*TC)++;
            numbers_count++; 
            *DC += numbers_count;
            break;
        }
        set_wordfield_by_num(last_wf,(unsigned int)atoi(word));
        strcat(last_num_str, word);
        binary_node_add(binary_table,*TC, last_num_str);
        set_binary_node_wordfield(binary_table,*TC,last_wf);
        free(last_wf);
        (*TC)++;
        numbers_count++; 
        /* NOTE: numbers count should be valid here */
        *DC += numbers_count;
        break;
    case DIRECTIVE_TYPE_EXTERN:
        *position = read_word_from_line(line, word, *position);
        is_label(word,true);        
        if(label_table_search(label_table,word) != -1)
        {
            log_error(__FILE__,__LINE__,"Label Redefinition - the label [%s] already exists in the label tabel.\n",word);
            flag = -1;
            break;
        }
        /* add the valid label to the label table and set its type as CODE */
        label_table_add(label_table,my_strdup(word),0,LABELTYPE_EXTERN);
        free(last_wf);
        break;
    case DIRECTIVE_TYPE_ENTRY:
        while((*position = read_word_from_line(line, word, *position)) != -1);
        free(last_wf);
        break;
    default:
        break;
    }

    return flag;
}