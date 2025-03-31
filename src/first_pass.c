#include "first_pass.h"
#include "input.h"
#include "utility.h"
#include "logger.h"
#include "error_manager.h"
#include "second_pass.h"
#include <ctype.h>

void prepare_first_pass(const char* filepath, MacroTable* macro_table)
{
    /* 
        tables created locally - since the stack frame of this function will remain valid
        until we return/ fininshed with the first pass. 
    */
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
        add_error_entry(ErrorType_OpenFileFailure,__FILE__,__LINE__);
    }

    if(execute_first_pass(fp,&label_table,&instruction_table, macro_table,filepath) >= 0) /* success */
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


int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table, MacroTable* macro_table, const char* filepath)
{
    int flag                    = 0;    /* flag is used to signal if we encounted errors while executing first pass */
    unsigned int DC             = 0;    /* data counter */
    /* 
        total counter - we start from 100 (START_ADDRESS) and start counting instructions and directives 
        we calculate the instruction counter final at the end
    */
    unsigned int TC             = START_ADDRESS;  
    unsigned int DCF            = -1;   /* final data counter */
    unsigned int ICF            = -1;   /* final instruction counter */
    char* line                  = string_calloc(MAX_LINE, sizeof(char)); 
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    BinaryTable* binary_table   = binary_table_create(5);

    while(read_line(fp,line) != INVALID_RETURN)
    {
        static int current_line = 0;
        int position = 0;
        current_line++;
        if(line[0] == SEMICOLON || (is_line_empty(line) == VALID_RETURN))
        {
            /* ignore comments and empty lines */
            continue;
        }
        while ((position = read_word_from_line(line, word, position)) != INVALID_RETURN) 
        {
            if(is_instruction(word) != INVALID_RETURN)
            {
                flag = handle_instruction(binary_table,instruction_table,&TC, current_line,line,word,&position,filepath);
            }
            else if(is_directive(word) != INVALID_RETURN)
            {
                flag = handle_directive(binary_table,label_table,&TC,&DC,line,word,&position,filepath,current_line);
            }
            else if(is_label(word) != INVALID_RETURN)
            {
                flag = handle_labels(label_table,TC,line,word,position,filepath,current_line);
            }      
            else
            {
                add_error_entry(ErrorType_UnrecognizedToken,filepath,current_line);    
                break;
            }

            if(flag == INVALID_RETURN)
                break;
        }
    }

    ICF = TC - START_ADDRESS - DC;
    DCF = DC;

    printf("\n\n");
    binary_table_print(binary_table);
    printf("\n\n");
    label_table_print(label_table);
    printf("\n\n");

    if(word)
        free(word);
    if(line)
        free(line);

    log_out(__FILE__,__LINE__,"TC: %u\t DC: %u\n",TC,DC);
    log_out(__FILE__,__LINE__,"ICF: %u\t DCF: %u\n",ICF,DCF);
    
    if(is_errors_array_empty()  == INVALID_RETURN)
    {
        print_errors_array();
        flag = INVALID_RETURN;
    }
    else
    {
        log_out(__FILE__,__LINE__,"\n\nFIRST-PASS Alright Alright Alright!\n\n");
    }

    flag = prepare_second_pass(filepath,binary_table,label_table,ICF,DCF);

    return flag;
}

int get_operands_count(char* str)
{
    if(str == NULL)
    {
        log_error(__FILE__,__LINE__,"Can't get wordfield count, str is NULL!.\n");
        return INVALID_RETURN;
    }

    if(is_instruction(str) != VALID_RETURN)
    {
        return INVALID_RETURN;
    }

    if(strcmp(str,"stop") == 0 || strcmp(str,"rts") == 0)
    {
        
        return NO_OPERANDS_INSTRUCTION;
    }
    if(strcmp(str,"mov") == 0 || strcmp(str,"cmp") == 0 || 
        strcmp(str,"add") == 0 || strcmp(str,"sub") == 0 || 
        strcmp(str,"lea") == 0)
    {
        return TWO_OPERANDS_INSTRUCTION;
    }
    return ONE_OPERAND_INSTRUCTION;
}

OperandType get_operand_type(char* str)
{
    if(str == NULL)
    {
        log_out(__FILE__,__LINE__,"str is null! can't get operand type!\n");
        return INVALID_RETURN;
    }
    if(str[0] == HASHTAG || isdigit(*str)) 
    {
        return OPERAND_TYPE_IMMEDIATE; /* addressing mode: 0 */
    }
    else if(str[0] == AMPERSAND)
    {
        return OPERAND_TYPE_RELATIVE; /* addressing mode: 2*/
    }
    else if(is_register(str) != INVALID_RETURN)
    {
        return OPERAND_TYPE_REGISTER; /* addressing mode: 3 */
    }
    else if(is_label(str) != INVALID_RETURN)
    {
        return OPERAND_TYPE_DIRECT;
    }
    
    return INVALID_RETURN;
}

DirectiveType get_directive_type(char* str)
{
    if(str == NULL)
    {
        log_out(__FILE__,__LINE__,"str is null! can't get directive type!\n");
        return INVALID_RETURN;
    }
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
    
    return INVALID_RETURN;
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

int handle_labels(LabelTable* label_table, unsigned int TC, char* line, char* word,int position,
    const char* filepath, int current_line)
{
    char temp[MAX_LABEL_LENGTH];
    char* label_name        = string_calloc(strlen(word)+1, sizeof(char));
    int flag                = 0;
    int temp_position       = position;
    int label_index;
    
    if(word[strlen(word) - 1] != COLON)
    {    
        /* 
            get the next word - if its an instruction or a directive or just a single ':'
            we are missing a ':' for the label
            otherwise we have an unrecognized token
        */
        temp_position = read_word_from_line(line, temp, temp_position);
        if(temp[strlen(temp) - 1] == COLON)
        {
            add_error_entry(ErrorType_InvalidLabel_InvalidColon,filepath,current_line);
        }
        else if(is_instruction(temp) != INVALID_RETURN || is_directive(temp) != INVALID_RETURN)
        {
            add_error_entry(ErrorType_InvalidLabel_MissingColon,filepath,current_line);
        }
        else
        {
            add_error_entry(ErrorType_UnrecognizedToken,filepath,current_line);
        }
        if(label_name != NULL)
            free(label_name);
        return INVALID_RETURN;
    }
    
    if(!isspace(line[temp_position+1]))
    {
        add_error_entry(ErrorType_InvalidLabel_MissingSpace,filepath,current_line);
        if(label_name != NULL)
            free(label_name);
        return INVALID_RETURN;
    }

    strcpy(label_name,word);
    remove_last_character(label_name);
    
    if((label_index = label_table_search(label_table,label_name)) != INVALID_RETURN)
    {
        if(label_table->labels[label_index].type != LABELTYPE_ENTRY)
        {    
            add_error_entry(ErrorType_InvalidLabel_Redefinition,filepath,current_line);
            if(label_name != NULL)
                free(label_name);
            return INVALID_RETURN;
        }
        else
        {
            label_table->labels[label_index].address = TC;
        }    
    }

    else if(is_instruction(label_name) != INVALID_RETURN || is_directive(label_name) != INVALID_RETURN  || is_register(label_name) != INVALID_RETURN)
    {
        add_error_entry(ErrorType_InvalidLabel_Reserved,filepath,current_line);
        if(label_name != NULL)
            free(label_name);
        return INVALID_RETURN;
    }

    temp_position = read_word_from_line(line, temp, temp_position);
    if(temp_position == INVALID_RETURN)
    {
        add_error_entry(ErrorType_InvalidLabel_EmptyLabel,filepath,current_line);
        if(label_name != NULL)
            free(label_name);
        flag = INVALID_RETURN;
    }

    if(flag != INVALID_RETURN && label_index == -1)
    {
        /* add the valid label to the label table and set its type as CODE */
        label_table_add(label_table,my_strdup(label_name),TC,LABELTYPE_CODE);
    }

    if((is_directive(temp) == VALID_RETURN) && flag != INVALID_RETURN && label_index != INVALID_RETURN)
    {
        if(label_table->labels[label_index].type != LABELTYPE_ENTRY)
            label_table_set_label_type(label_table,TC,LABELTYPE_DATA);
        else 
            label_table_set_label_type(label_table,TC,LABELTYPE_DATA_ENTRY);
    }

    if(label_name != NULL)
        free(label_name);
    return flag;
}

int handle_instruction(BinaryTable* binary_table,InstructionTable* instruction_table, 
    unsigned int* TC, unsigned int current_line, char* line, char* word, int* position,const char* filepath)
{
    int flag                = 0;
    int operands_count      = 0;
    wordfield* wf           = create_wordfield_by_opname(word, instruction_table);
    set_wordfield_are(wf,ARE_ABSOLUTE);
    operands_count          = get_operands_count(word);
    
    if(operands_count == INVALID_RETURN)
    {
        log_error(__FILE__,__LINE__, "Failed to get operands count!\n");
        free(wf); 
        wf = NULL;
        return INVALID_RETURN;
    }
    else if(operands_count == NO_OPERANDS_INSTRUCTION)
    {
        binary_node_add(binary_table,*TC,line,NULL); /* no operands, no need for the unresolved label */
        set_binary_node_wordfield(binary_table,*TC,wf);
        (*TC)++;
        
        /* try to get the next word in line, if we recieve a valid position in line, extraneous text found */
        if((*position = read_word_from_line(line, word, *position)) != INVALID_RETURN)
        {    
            add_error_entry(ErrorType_ExtraneousText_Instruction,filepath,current_line);
            return INVALID_RETURN;
        } 
    }
    else if (operands_count == ONE_OPERAND_INSTRUCTION)
    {
        flag = handle_single_operand_instruction(binary_table,line,word,position,TC,filepath,current_line,wf);        
    }
    else 
    {
        flag = handle_double_operand_instruction(binary_table,line,word,position,TC,filepath,current_line,wf);
    } 

    if(wf != NULL)
    {
        free(wf);
        wf = NULL;   
    }
    
    return flag;
}

int handle_directive(BinaryTable* binary_table, LabelTable* label_table, unsigned int* TC, unsigned int* DC,
    char* line, char* word, int* position,const char* filepath, int current_line)
{
    DirectiveType directive_type;
    int flag                    = 0;
    int index;
    
    /*
        IMPORTANT: .entry is handled in the 2nd pass. 
    */
    directive_type = get_directive_type(word);
    switch (directive_type)
    {
    case DIRECTIVE_TYPE_STRING:
        flag = handle_string_directive(binary_table,TC,DC,line,word,position,filepath,current_line);
        break;
    case DIRECTIVE_TYPE_DATA:
        flag = handle_data_directive(binary_table,TC,DC,line,word,position,filepath,current_line);
        break;
    case DIRECTIVE_TYPE_EXTERN:
        flag = check_directive_label(label_table,line,word,position,filepath,current_line);
        /* add the valid label to the label table and set its type as CODE */
        if(flag == VALID_RETURN)
            label_table_add(label_table,my_strdup(word),0,LABELTYPE_EXTERN);
        break;
    case DIRECTIVE_TYPE_ENTRY:
        flag = check_directive_label(label_table,line,word,position,filepath,current_line);
        if((index = label_table_search(label_table,word)) != INVALID_RETURN)
        {
            if(label_table->labels[index].type == LABELTYPE_CODE)
                label_table->labels[index].type = LABELTYPE_CODE_ENTRY;
            else if(label_table->labels[index].type == LABELTYPE_DATA)
                label_table->labels[index].type = LABELTYPE_DATA_ENTRY;
        }
    
        if(flag == VALID_RETURN && index == INVALID_RETURN)
        {
            label_table_add(label_table,my_strdup(word),0,LABELTYPE_ENTRY);    
        }

        flag = INVALID_RETURN; /* in order to 'skip' the whole line, we set it to invalid */
        break;
    default:
        break;
    }

    return flag;
}


int check_immediate_value(char* operand, const char* filepath, int current_line)
{
    int value;
    if(operand[0] != HASHTAG) /* checks for '#' */
    {
        add_error_entry(ErrorType_InvalidValue_MissingHashtag,filepath,current_line);
        return INVALID_RETURN;
    }    
    remove_first_character(operand);
    if(is_valid_number(operand) == INVALID_RETURN)
    {
        add_error_entry(ErrorType_InvalidValue,filepath,current_line);
        return INVALID_RETURN;
    }
    value = atoi(operand);
    if(value > MAX_24_BIT_NUMBER)
    {
        add_error_entry(ErrorType_InvalidValue_Exceeding,filepath,current_line);
        return INVALID_RETURN;
    }
    return value;
}            

void handle_immediate_operand(BinaryTable* binary_table,char* line, char* word,unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction,wordfield* wf_immediate_value)
{
    int num = check_immediate_value(word,filepath,current_line);
    
    if(wf_instruction != NULL)
    {
        binary_node_add(binary_table,*TC,line,NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_instruction);
        (*TC)++;
    }
    
    if(wf_immediate_value != NULL)
    {
        set_wordfield_are_num(wf_immediate_value,num,ARE_ABSOLUTE); 
        binary_node_add(binary_table,*TC,"Immediate value",NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_immediate_value);
        (*TC)++;
    }
}


void handle_register_operand(BinaryTable* binary_table, char* line, char* word, OperandType operand1_type,unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction)
{
    int num = 0;
    if(is_register(word) == INVALID_RETURN) /* TODO: Add an error entry if register is invalid. */
    {
        log_error(__FILE__,__LINE__,"Register invalid! Not Valid Register Name.\n");
        free(wf_instruction);
        free(word);
        return;
    }
    remove_first_character(word);
    num = atoi(word);
    if(num > MAX_REGISTERS)
    {
        add_error_entry(ErrorType_InvalidRegister_ExceedingRegisterIndex,filepath,current_line);
        return;
    }
    
    if(operand1_type == OPERAND_TYPE_SINGLE)
    {
        set_wordfield_dest(wf_instruction,OPERAND_TYPE_REGISTER,num);
        binary_node_add(binary_table,*TC,line,NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_instruction);
        return;
    }

    if(operand1_type == OPERAND_TYPE_FIRST)
    {
        /* we have oeprand1_type as -1 - we are in the first operand of 2 in total */
        set_wordfield_src(wf_instruction,OPERAND_TYPE_REGISTER,num);
        binary_node_add(binary_table,*TC,line,NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_instruction);
        return;
    }

    set_wordfield_dest(wf_instruction,OPERAND_TYPE_REGISTER,num);
    /* first operand has an extra wordfield, the instruction wordfield is 2 addresses back */
    if(operand1_type != OPERAND_TYPE_REGISTER && operand1_type != OPERAND_TYPE_FIRST)
    {
        set_binary_node_wordfield(binary_table,*TC-2,wf_instruction);
    }
    else 
    {
        /* we are in the second operand - 1st operand was not a register - has an extra wordfield */
        set_binary_node_wordfield(binary_table,*TC-1,wf_instruction); 
    }
}

int handle_single_operand_instruction(BinaryTable* binary_table,char* line, char* word,int* position, unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction)
{
    char* single_operand;
    OperandType single_operand_type;
    int flag            = 0;
    wordfield* new_wf   = init_wordfield();
    *position           = read_word_from_line(line, word, *position);
    single_operand_type = get_operand_type(word);

    if(check_target_operand(wf_instruction->opcode,single_operand_type) == INVALID_RETURN)
    {
        free(new_wf);
        new_wf = NULL;
        add_error_entry(ErrorType_InvalidInstruction_WrongTargetOperand,filepath,current_line);
        return INVALID_RETURN;
    }

    if(*position == INVALID_RETURN)
    {
        free(new_wf);
        new_wf = NULL;
        add_error_entry(ErrorType_InvalidInstruction_MissingTargetOperand,filepath,current_line);
        return INVALID_RETURN;
    }
    single_operand = my_strdup(word);
    /* try to get the next word in line, if we recieve a valid position in line, extraneous text found */
    if((*position = read_word_from_line(line, word, *position)) != INVALID_RETURN)
    {
        free(new_wf);
        new_wf = NULL;
        
        free(single_operand);
        single_operand = NULL;
        
        add_error_entry(ErrorType_ExtraneousText_Instruction,filepath,current_line);
        return INVALID_RETURN;
    } 

    switch (single_operand_type)
    {
    case OPERAND_TYPE_IMMEDIATE:
        handle_immediate_operand(binary_table,line,single_operand,TC,filepath,current_line,wf_instruction,new_wf);
        if(new_wf != NULL)
        {
            free(new_wf);
            new_wf = NULL;
        }
        break;
    case OPERAND_TYPE_DIRECT:
        flag = is_label(single_operand);
        if(flag == INVALID_RETURN)
        {
            add_error_entry(ErrorType_InvalidLabel_Name,filepath,current_line);
        }
        set_wordfield_dest(wf_instruction,OPERAND_TYPE_DIRECT,0);
        binary_node_add(binary_table,*TC,line,NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_instruction);
        (*TC)++;
        binary_node_add(binary_table,*TC,"Address of Label",single_operand);
        set_binary_node_wordfield(binary_table,*TC,new_wf);
        if(new_wf != NULL)
        {
            free(new_wf);
            new_wf = NULL;
        }
        (*TC)++;
        break;
    case OPERAND_TYPE_RELATIVE:
        /* 
            NOTE: This assembler supports both direct and relative addressing.
            If an operand is missing the '&' prefix, it will be treated as a direct addressing mode.
            To use relative addressing, ensure that the operand begins with an '&'.
        */

        set_wordfield_dest(wf_instruction,OPERAND_TYPE_RELATIVE,0);
        binary_node_add(binary_table,*TC,line,NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_instruction);
        (*TC)++;
        /*
        if(single_operand[0] == AMPERSAND)
            remove_first_character(single_operand);
        */
        binary_node_add(binary_table,*TC,"Distance to Label",single_operand);
        set_binary_node_wordfield(binary_table,*TC,new_wf);
        if(new_wf != NULL)
        {
            free(new_wf);
            new_wf = NULL;
        }
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
        handle_register_operand(binary_table,line,single_operand,OPERAND_TYPE_SINGLE,TC,filepath,current_line,wf_instruction);
        (*TC)++;
        wf_instruction = NULL;

        /* no use for an extra wordfield if 1st operand is a register */
        if(new_wf)
        {
            free(new_wf);
            new_wf = NULL;
        }
        break;
    default:
        break;
    }

    if(single_operand != NULL)
    {
        free(single_operand);
        single_operand = NULL;
    }

    return flag;
}

int handle_double_operand_instruction(BinaryTable* binary_table,char* line, char* word,int* position, unsigned int* TC,
    const char* filepath, int current_line,wordfield* wf_instruction)
{
    OperandType operand1_type, operand2_type;
    char* operand1; /* NOTE: make sure to free when necessary! */
    char* operand2; /* NOTE: make sure to free when necessary! */
    wordfield* new_wf1  = NULL; 
    wordfield* new_wf2  = NULL;
    int flag            = 0;
    

    get_double_operands(line,word,position,filepath,current_line,&operand1,&operand2);
    
    /* try to get the next word in line, in order to find extraneous text */
    if((*position = read_word_from_line(line, word, *position)) != INVALID_RETURN)
    {
        free(operand1);
        operand1 = NULL;
        free(operand2);
        operand2 = NULL;
        add_error_entry(ErrorType_ExtraneousText_Instruction,filepath,current_line);
        return INVALID_RETURN;
    } 

    operand1_type = get_operand_type(operand1);
    operand2_type = get_operand_type(operand2);
    
    if(check_src_operand(wf_instruction->opcode,operand1_type) == INVALID_RETURN)
    {
        free(operand1);
        operand1 = NULL;
        free(operand2);
        operand2 = NULL;
        add_error_entry(ErrorType_InvalidInstruction_WrongSrcOperand,filepath,current_line);
        return INVALID_RETURN;
    }
    if(check_target_operand(wf_instruction->opcode,operand2_type) == INVALID_RETURN)
    {
        free(operand1);
        operand1 = NULL;
        free(operand2);
        operand2 = NULL;
        add_error_entry(ErrorType_InvalidInstruction_WrongTargetOperand,filepath,current_line);
        return INVALID_RETURN;
    }

    switch (operand1_type)
    {
    case OPERAND_TYPE_IMMEDIATE: 
        set_wordfield_src(wf_instruction,OPERAND_TYPE_IMMEDIATE,0);
        new_wf1 = init_wordfield();
        handle_immediate_operand(binary_table,line,operand1,TC,filepath,current_line,wf_instruction,new_wf1);
                
        if(new_wf1 != NULL)
        {
            free(new_wf1);
            new_wf1 = NULL;
        }
        break;
    case OPERAND_TYPE_DIRECT:
        flag = is_label(operand1);
        if(flag == INVALID_RETURN)
        {
            add_error_entry(ErrorType_InvalidLabel_Name,filepath,current_line);
        }        
        set_wordfield_src(wf_instruction,OPERAND_TYPE_DIRECT,0);
        binary_node_add(binary_table,*TC,line,NULL);
        set_binary_node_wordfield(binary_table,*TC,wf_instruction);
        (*TC)++;
        new_wf1 = init_wordfield();
        binary_node_add(binary_table,*TC,"Address of Label",operand1);
        set_binary_node_wordfield(binary_table,*TC,new_wf1);
        if(new_wf1 != NULL)
        {
            free(new_wf1);
            new_wf1 = NULL;
        }
        (*TC)++;
        break;
    case OPERAND_TYPE_RELATIVE:
        /* 
        NOTE: This assembler supports both direct and relative addressing.
        If an operand is missing the '&' prefix, it will be treated as a direct addressing mode.
        To use relative addressing, ensure that the operand begins with an '&'.
        */

        (*TC)++;
        /* no use for this if 1st operand is relative - processed in 2nd Pass */
        if(new_wf1 != NULL)
        {
            free(new_wf1);
            new_wf1 = NULL;
        }
        if(operand1 != NULL)
        {
            free(operand1);
            new_wf1 = NULL;
        }
        break;
    case OPERAND_TYPE_REGISTER:
        handle_register_operand(binary_table,line, operand1,OPERAND_TYPE_FIRST,TC,filepath,current_line,wf_instruction);
        (*TC)++;
        /* no use for an extra wordfield if 1st operand is a register */
        
        if(new_wf1 != NULL)
        {
            free(new_wf1);
            new_wf1 = NULL;
        }
        if(operand1 != NULL)
        {
            free(operand1);
            operand1 = NULL;
        }
        break;
    default:
        break;
    }

    
    switch (operand2_type)
    {
    case OPERAND_TYPE_IMMEDIATE: /* only in the special case of an immediate value if the instruction is 'cmp' */
        new_wf2 = init_wordfield();
        handle_immediate_operand(binary_table,line,operand2,TC,filepath,current_line,NULL,new_wf2);
        
        if(new_wf2 != NULL)
        {
            free(new_wf2);
            new_wf2 = NULL;
        }
        break;
    case OPERAND_TYPE_DIRECT:
        flag = is_label(operand2);
        if(flag == INVALID_RETURN)
        {
            add_error_entry(ErrorType_InvalidLabel_Name,filepath,current_line);
        }
        set_wordfield_dest(wf_instruction,OPERAND_TYPE_DIRECT,0);
        if(operand1_type == OPERAND_TYPE_IMMEDIATE || operand1_type == OPERAND_TYPE_REGISTER || operand1_type == OPERAND_TYPE_RELATIVE)
        {
            set_binary_node_wordfield(binary_table,*TC-1,wf_instruction);
        }
        else if(operand1_type == OPERAND_TYPE_DIRECT)
        {
            set_binary_node_wordfield(binary_table,*TC-2,wf_instruction);
        }

        new_wf2 = init_wordfield();
        binary_node_add(binary_table,*TC,"Address of Label",operand2);
        set_binary_node_wordfield(binary_table,*TC,new_wf2);
        if(new_wf2 != NULL)
        {
            free(new_wf2);
            new_wf2 = NULL;
        }
        (*TC)++;
        break;
    case OPERAND_TYPE_RELATIVE:
        /* 
            NOTE: This assembler supports both direct and relative addressing.
            If an operand is missing the '&' prefix, it will be treated as a direct addressing mode.
            To use relative addressing, ensure that the operand begins with an '&'.
        */
        (*TC)++;
        /* no use for this if 2nd operand if its relative - processed in 2nd Pass */
        if(new_wf2 != NULL)
        {
            free(new_wf2);
            new_wf2 = NULL;
        }
        if(operand2 != NULL)
        {
            free(operand2);
            operand2 = NULL;
        }
        break;
    case OPERAND_TYPE_REGISTER:
        handle_register_operand(binary_table,line,operand2,operand1_type,TC,filepath,current_line,wf_instruction);
        /* no use for this if 2nd operand is a register */
        if(new_wf2 != NULL)
        {
            free(new_wf2);
            new_wf2 = NULL;
        }
        if(operand2 != NULL)
        {
            free(operand2);
            operand2 = NULL;
        }
        break;
    default:
        break;
    }

    if(new_wf1 != NULL)
    {
        free(new_wf1);
        new_wf1 = NULL;
    }
    if(operand1 != NULL)
    {
        free(operand1);
        operand1 = NULL;
    }
    if(new_wf2 != NULL)
    {
        free(new_wf2);
        new_wf2 = NULL;
    }
    if(operand2 != NULL)
    {
        free(operand2);
        operand2 = NULL;
    }
    return flag;
}

void get_double_operands(char* line, char* word, int* position, const char* filepath, int current_line,
    char** operand1, char** operand2)
{
    *position = read_word_from_line(line, word, *position); /* get operand1 */
    if(word[strlen(word)-1] == COMMA) /* if last character of src operand is a comma remove it */
    {
        remove_last_character(word);  
        *operand1 = my_strdup(word);
        *position = read_word_from_line(line, word, *position);
        if(*position == INVALID_RETURN) 
        {
            add_error_entry(ErrorType_InvalidInstruction_MissingTargetOperand,filepath,current_line);
            if(*operand1 != NULL)
                free(*operand1);
            return;
        }
        *operand2 = my_strdup(word);
    } 
    else /* otherwise check for comma to see if missing */
    {
        *operand1 = my_strdup(word);
        *position = read_word_from_line(line, word, *position);
        if(COMMA != word[0])
        {
            add_error_entry(ErrorType_InvalidInstruction_MissingComma,filepath,current_line);      
            if(*operand1 != NULL)
                free(*operand1);
            return;
        }
        if(word[0] == NULL_TERMINATOR || strlen(word) == 1) 
        {
            *position = read_word_from_line(line, word, *position);
        }    
        else if(word[0] == COMMA)
            remove_first_character(word);
        *operand2 = my_strdup(word);
    }  
}

static int valid_src[MAX_INSTRUCTIONS][ADDRESSING_MODES] = 
{
    /* 
        NOTE: 
        some entried are unused but most be defined.
        the order is as in the mmn's table of valid addressing modes:
        { IMMEDIATE, DIRECT, RELATIVE, REGISTER }     
    */

    { 1, 1, 0, 1 }, /* opcode 0: mov */    
    { 1, 1, 0, 1 }, /* opcode 1: cmp */
    { 1, 1, 0, 1 }, /* opcode 2: add/sub */
    { 0, 0, 0, 0 }, /* opcode 3 (UNUSED) */
    { 0, 1, 0, 0 }, /* opcode 4: lea */
    { 0, 0, 0, 0 }, /* opcode 5: clr/not/inc/dec */
    { 0, 0, 0, 0 }, /* opcode 6 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 7 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 8 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 9: jmp/bne/jsr */
    { 0, 0, 0, 0 }, /* opcode 10 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 11 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 12: red */
    { 0, 0, 0, 0 }, /* opcode 13: prn */
    { 0, 0, 0, 0 }, /* opcode 14: rts */
    { 0, 0, 0, 0 }, /* opcode 15: stop */
};

static int valid_dest[MAX_INSTRUCTIONS][ADDRESSING_MODES] =
{
    /* 
        NOTE: 
        some entried are unused but most be defined.
        the order is as in the mmn's table of valid addressing modes:
        { IMMEDIATE, DIRECT, RELATIVE, REGISTER }     
    */

    { 0, 1, 0, 1 }, /* opcode 0: mov */
    { 1, 1, 0, 1 }, /* opcode 1: cmp */
    { 0, 1, 0, 1 }, /* opcode 2: add/sub */
    { 0, 0, 0, 0 }, /* opcode 3 (UNUSED) */
    { 0, 1, 0, 1 }, /* opcode 4: lea */
    { 0, 1, 0, 1 }, /* opcode 5: clr/not/inc/dec */
    { 0, 0, 0, 0 }, /* opcode 6 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 7 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 8 (UNUSED) */
    { 0, 1, 1, 0 }, /* opcode 9: jmp/bne/jsr */
    { 0, 0, 0, 0 }, /* opcode 10 (UNUSED) */
    { 0, 0, 0, 0 }, /* opcode 11 (UNUSED) */
    { 0, 1, 0, 1 }, /* opcode 12: red */
    { 1, 1, 0, 1 }, /* opcode 13: prn */
    { 0, 0, 0, 0 }, /* opcode 14: rts */
    { 0, 0, 0, 0 }  /* opcode 15: stop */

};

int check_src_operand(int opcode, OperandType operand_type) 
{
    int flag = -1;
    if (opcode < 0 || opcode > 15) 
        return INVALID_RETURN;
    if (operand_type < 0 || operand_type > 3) 
        return INVALID_RETURN;

    flag = valid_src[opcode][operand_type];
    if(flag == 1)
        return VALID_RETURN;
    
    return INVALID_RETURN;
}

int check_target_operand(int opcode, OperandType operand_type) 
{
    int flag = -1;
    if (opcode < 0 || opcode > 15) 
        return INVALID_RETURN;
    if (operand_type < 0 || operand_type > 3) 
        return INVALID_RETURN;

    flag = valid_dest[opcode][operand_type];
    if(flag == 1)
        return VALID_RETURN;
    
    return INVALID_RETURN;
}

int handle_data_directive(BinaryTable* binary_table,unsigned int* TC, unsigned int* DC, char* line, 
    char* word, int* position,const char* filepath, int current_line)
{
    char last_num_str[MAX_WORD] = "Integer "; 
    wordfield* last_wf          = init_wordfield(); 
    int numbers_count           = 0;

    while ((*position = read_word_from_line(line, word, *position)) != INVALID_RETURN && word[strlen(word)-1] == COMMA)
    {
        char final_str[MAX_WORD] = "Integer ";
        wordfield num_wf = {0};
        remove_last_character(word);
        set_wordfield_by_num(&num_wf,(unsigned int)atoi(word));
        if(numbers_count == 0)
        {
            binary_node_add(binary_table,*TC,line, NULL);
            set_binary_node_wordfield(binary_table,*TC,&num_wf);
            numbers_count++;
            (*TC)++;
            continue;
        }
        
        strcat(final_str, word);
        binary_node_add(binary_table,*TC, final_str, NULL);
        set_binary_node_wordfield(binary_table,*TC,&num_wf);
        (*TC)++;
        numbers_count++;
    }
    if(*position == -1)
    {
        if(last_wf)
            free(last_wf);
        add_error_entry(ErrorType_InvalidDirective_Empty,filepath,current_line);
        return INVALID_RETURN;
    }
    if(numbers_count == 0) /* single number I.E: .data 100 */
    {
        set_wordfield_by_num(last_wf,(unsigned int)atoi(word));
        binary_node_add(binary_table,*TC,line, NULL);
        set_binary_node_wordfield(binary_table,*TC,last_wf);
        if(last_wf)
            free(last_wf);
        (*TC)++;
        numbers_count++; 
        *DC += numbers_count;
        return VALID_RETURN;
    }
    set_wordfield_by_num(last_wf,(unsigned int)atoi(word));
    strcat(last_num_str, word);
    binary_node_add(binary_table,*TC, last_num_str, NULL);
    set_binary_node_wordfield(binary_table,*TC,last_wf);
    if(last_wf)
        free(last_wf);
    (*TC)++;
    numbers_count++; 
    /* NOTE: numbers count should be valid here */
    *DC += numbers_count;

    return VALID_RETURN; 
}

int handle_string_directive(BinaryTable* binary_table, unsigned int* TC, unsigned int* DC, char* line, 
    char* word, int* position,const char* filepath, int current_line)
{
    int i               = 0; 
    int str_length      = -1;
    int char_count      = 0;
    wordfield* last_wf  = init_wordfield(); 

    *position = read_word_from_line(line, word, *position);
    str_length = strlen(word);
    if((*position == -1 || word[0] != DOUBLE_QUOTE || word[str_length] != DOUBLE_QUOTE) && 
        (word[0] != DOUBLE_QUOTE && word[str_length] != DOUBLE_QUOTE))
    {
        if(last_wf)
            free(last_wf);
        add_error_entry(ErrorType_InvalidDirective_MissingQuotes,filepath,current_line);
        return INVALID_RETURN;
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
            binary_node_add(binary_table,*TC,line, NULL);
            set_binary_node_wordfield(binary_table,*TC,&char_wf);
            (*TC)++;
            char_count++;
            continue;
        }
        strncat(ascii_str, character,1);
        binary_node_add(binary_table,*TC,ascii_str, NULL);
        set_binary_node_wordfield(binary_table,*TC,&char_wf);
        (*TC)++;
        char_count++;
    }
    binary_node_add(binary_table,*TC,"Ascii code \'\\0\'", NULL);
    set_binary_node_wordfield(binary_table,*TC,last_wf);
    if(last_wf)
        free(last_wf);
    (*TC)++;

    return VALID_RETURN;
}

int check_directive_label(LabelTable* label_table,char* line, char* word, int* position,
    const char* filepath, int current_line)
{
    char directive_name[MAX_WORD];
    strcpy(directive_name,word);

    *position = read_word_from_line(line, word, *position);
    is_label(word);        
    if(label_table_search(label_table,word) != INVALID_RETURN && strcmp(directive_name,".entry") != VALID_RETURN)
    {
        add_error_entry(ErrorType_InvalidLabel_Redefinition,filepath,current_line);
        return INVALID_RETURN;
    }
    if(*position == -1)
    {
        add_error_entry(ErrorType_InvalidDirective_Empty,filepath,current_line);
        return INVALID_RETURN;
    }
    return VALID_RETURN;
}