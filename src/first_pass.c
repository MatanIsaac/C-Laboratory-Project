#include "first_pass.h"
#include "input.h"
#include "common.h"
#include "utility.h"
#include "isaac_logger.h"
#include "binary_table.h"

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
    
    /*instruction_table_print(&instruction_table);*/
    
    label_table_create(&label_table);
    /*label_table_print(&label_table);*/

    log_out(__FILE__,__LINE__, "firstpass: opening filename: %s\n", filepath);
    fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        log_error(__FILE__,__LINE__, "Failed to open [%s] for first pass\n.", filepath);
    }

    if(execute_first_pass(fp,&label_table,&instruction_table, macro_table) >= 0) /* success */
    {
        log_out(__FILE__,__LINE__, "Done First-Pass for [%s]\n.", filepath);
    }
    else /* first pass failed */
    {
        log_error(__FILE__,__LINE__, "Failed First-Pass for [%s]\n.", filepath);
    }
}   


int execute_first_pass(FILE* fp, LabelTable* label_table, InstructionTable* instruction_table, MacroTable* macro_table)
{
    int flag                    = 0;    /* flag is used to signal if we encounted errors while executing first pass */
    unsigned int DC             = 0;    /* Data Counter */
    unsigned int IC             = 100;  /* Instruction Counter */
    char* line                  = string_calloc(MAX_LINE, sizeof(char)); 
    char* word                  = string_calloc(MAX_WORD, sizeof(char));
    BinaryTable* binary_table   = init_binary_table(5);

    while(read_line(fp,line) != -1)
    {
        int position = 0;
        if(line[0] == SEMICOLON || is_line_empty(line))
        {
            /* ignore comments and empty lines */
            continue;
        }

        add_binary_node(binary_table,IC,line);

        while ((position = read_word_from_line(line, word, position)) != -1) 
        {
            if(is_instruction(word))
            {
                int operands_count = 0;
                wordfield* wf = init_wordfield();
                wf = create_wordfield_by_opname(word, instruction_table);
                set_wordfield_are(wf,4);

                log_out(__FILE__,__LINE__, 
                    "instruction line: [%s]\n\tinstruction word: [%s]\n\taddress [%u]\n\twordfield: "
                    ,line, word,IC);
                print_wordfield(wf);
                operands_count = get_operands_count(word);
                if(operands_count == -1)
                {
                    log_error(__FILE__,__LINE__, "Failed to get operands count!\n");
                }
                else if(operands_count == 0)
                {
                    
                    break;
                }
                else if (operands_count == 1)
                {
                    OperandType operand1_type;
                    wordfield* new_wf = init_wordfield();
                    position = read_word_from_line(line, word, position);
                    operand1_type = get_operand_type(word);
                    switch (operand1_type)
                    {
                    case OPERAND_TYPE_IMMEDIATE:
                        set_binary_node_wordfield(binary_table,IC,wf);
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"word: [%s]\n",word);
                        new_wf = init_wordfield();
                        set_wordfield_are_num(new_wf,atoi(word),4);
                        print_wordfield(new_wf);
                        IC++;
                        add_binary_node(binary_table,IC,"Immediate value");
                        set_binary_node_wordfield(binary_table,IC,new_wf);
                        print_binary_table(binary_table);
                        break;
                    case OPERAND_TYPE_DIRECT:
                        log_out(__FILE__,__LINE__,"operand_type_direct operand: [%s]\n",word);
                        set_wordfield_dest(wf,1,0);
                        print_wordfield(wf);
                        set_binary_node_wordfield(binary_table,IC,wf);
                        IC++;
                        add_binary_node(binary_table,IC,"Address of Label");
                        set_binary_node_wordfield(binary_table,IC,new_wf);
                        print_binary_table(binary_table);

                        /* TODO: 
                            1. check if its a jump instruction or else
                            2. if jump type instruction - calculate distance between addresses etc.. 
                            3. else - check instruction type, I.E: 
                                x: .data 23 \n dec x
                                or 
                                jmp next
                                or 
                                .entry / .extern
                                etc..
                                multiple option can be here.
                                make sure to find them all.
                        */
                        break;
                    case OPERAND_TYPE_RELATIVE:
                        log_out(__FILE__,__LINE__,"word: [%s]\n",word);
                        if(word[0] != AMPERSAND)
                        {
                            log_error(__FILE__,__LINE__, "Error missing '&' before label for relative type addressing!.\n");
                            flag = -1;
                        }
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"label: [%s]\n",word);
                        IC++;
                        /* 
                            IMPORTANT: this is relevant only for: jmp, jsr, bne
                            TODO:
                            1. check if label already exists in label table and declared before
                            2. if not - 
                            add an empty extra line with IC to the binary table 
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
                        log_out(__FILE__,__LINE__,"register: [%s]\n",word);
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"word: [%s]\n",word);
                        
                        set_wordfield_dest(wf,3,atoi(word));
                        print_wordfield(wf);
                        set_binary_node_wordfield(binary_table,IC,wf);
                        print_binary_table(binary_table);

                        break;
                    default:
                        break;
                    }

                }
                else /* operands count is 2 */
                {
                    OperandType operand1_type, operand2_type;
                    wordfield* new_wf1 = init_wordfield();
                    wordfield* new_wf2 = init_wordfield();

                    position = read_word_from_line(line, word, position);
                    if(word[strlen(word)-1] != COMMA)
                    {
                        log_error(__FILE__,__LINE__,"Missing comma after first operand!.\n");
                        flag = -1;
                    }
                    word = remove_last_character(word);
                    log_out(__FILE__,__LINE__, "instruction operand1: [%s]\n",word);    

                    
                    operand1_type = get_operand_type(word);
                    switch (operand1_type)
                    {
                    case OPERAND_TYPE_IMMEDIATE:
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"value: [%s]\n",word);
                        new_wf1 = init_wordfield();
                        set_wordfield_are_num(new_wf1,atoi(word),4);
                        print_wordfield(new_wf1);
                        IC++;
                        break;
                    case OPERAND_TYPE_DIRECT:
                        log_out(__FILE__,__LINE__,"label: [%s]\n",word);
                        set_wordfield_src(wf,1,0);
                        print_wordfield(wf);
                        set_binary_node_wordfield(binary_table,IC,wf); /* if theres another label we set this again in the operand2 switch case */
                        IC++;
                        add_binary_node(binary_table,IC,"Address of Label");
                        set_binary_node_wordfield(binary_table,IC,new_wf1);
                        break;
                    case OPERAND_TYPE_RELATIVE:
                        log_out(__FILE__,__LINE__,"word: [%s]\n",word);
                        IC++;
                        break;
                    case OPERAND_TYPE_REGISTER:
                        log_out(__FILE__,__LINE__,"register: [%s]\n",word);
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"register number: [%s]\n",word);
                        set_wordfield_src(wf,3,atoi(word));
                        print_wordfield(wf);
                        /* 
                            if theres another register we set this again in the operand2 switch case 
                        */
                        set_binary_node_wordfield(binary_table,IC,wf); 

                        break;
                    default:
                        break;
                    }

                    /*
                    if(operand1_type != OPERAND_TYPE_RELATIVE && operand1_type != OPERAND_TYPE_REGISTER && 
                    operand2_type != OPERAND_TYPE_RELATIVE && operand2_type != OPERAND_TYPE_REGISTER)
                    {
                    }
                    */
                    position = read_word_from_line(line, word, position);
                    log_out(__FILE__,__LINE__, "instruction operand2: [%s]\n",word);    
                    operand2_type = get_operand_type(word);                    
                    switch (operand2_type)
                    {
                    case OPERAND_TYPE_IMMEDIATE:
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"value: [%s]\n",word);
                        new_wf2 = init_wordfield();
                        set_wordfield_are_num(new_wf2,atoi(word),4);
                        print_wordfield(new_wf2);
                        IC++;
                        add_binary_node(binary_table,IC,"Immediate value");
                        set_binary_node_wordfield(binary_table,IC,new_wf2);
                        print_binary_table(binary_table);
                        break;
                    case OPERAND_TYPE_DIRECT:
                        log_out(__FILE__,__LINE__,"word: [%s]\n",word);
                        set_wordfield_dest(wf,1,0);
                        print_wordfield(wf);
                        set_binary_node_wordfield(binary_table,IC,wf);
                        IC++;
                        add_binary_node(binary_table,IC,"Address of Label");
                        set_binary_node_wordfield(binary_table,IC,new_wf2);
                        print_binary_table(binary_table);
                        break;
                    case OPERAND_TYPE_RELATIVE:
                        log_out(__FILE__,__LINE__,"word: [%s]\n",word);
                        IC++;
                        break;
                    case OPERAND_TYPE_REGISTER:
                        log_out(__FILE__,__LINE__,"register: [%s]\n",word);
                        word = remove_first_character(word);
                        log_out(__FILE__,__LINE__,"register number: [%s]\n",word);
                        
                        set_wordfield_dest(wf,3,atoi(word));
                        print_wordfield(wf);
                        if(operand1_type != OPERAND_TYPE_REGISTER)
                            set_binary_node_wordfield(binary_table,IC-1,wf);
                        else /* we did not update IC if operand1 is register, reset node wordfield to updated one */
                            set_binary_node_wordfield(binary_table,IC,wf); 
                        break;
                    default:
                        break;
                    }

                }
            }
            else if(is_directive(word))
            {
                DirectiveType directive_type;
                int str_length = -1, i = 0;
                wordfield* null_wf = init_wordfield();
                wordfield* last_num_wf = init_wordfield(); 
                log_out(__FILE__,__LINE__, 
                    "directive line: [%s]\n\tdirective word: [%s]\n\taddress [%u]\n"
                    ,line, word,IC);
                /*
                    IMPORTANT: .entry is handled in the 2nd pass. 
                    TODO: 
                    1. switch on directive type .data / .string / .extern ? 
                */
                directive_type = get_directive_type(word);
                switch (directive_type)
                {
                case DIRECTIVE_TYPE_STRING:
                    position = read_word_from_line(line, word, position);
                    log_out(__FILE__,__LINE__,  "directive string: [%s] | [%c]\n",word);
                    str_length = strlen(word);
                    if((word[0] != DOUBLE_QUOTE || word[str_length] != DOUBLE_QUOTE) && (word[0] != DOUBLE_QUOTE && word[str_length] != DOUBLE_QUOTE))
                    {
                        log_error(__FILE__,__LINE__,"Error reading string data, missing quotes.\n");
                        flag = -1;
                    }
                    word = remove_first_character(word);
                    word = remove_last_character(word);
                    str_length -= 2; /* not including 2 double quotes */
                    log_out(__FILE__,__LINE__,  "directive string length: [%d]\n",str_length);
    
                    /* 
                        TODO:
                        make sure to handle DC and all that is relevant to that . 
                    */
                    for( ; i < str_length; i++)
                    {
                        wordfield* char_wf = get_char_wordfield(&word[i]);
                        IC++;
                    }
                    log_out(__FILE__,__LINE__,"null -> ");
                    print_wordfield(null_wf);
                    log_out(__FILE__,__LINE__,"\n");
                    break;
                case DIRECTIVE_TYPE_DATA:
                    while ((position = read_word_from_line(line, word, position)) != -1 && word[strlen(word)-1] == COMMA)
                    {
                        wordfield* num_wf = init_wordfield();
                        const char* num_char = remove_last_character(word);
                        log_out(__FILE__,__LINE__,"directive word: [%s]\n",num_char);
                        set_wordfield_by_num(num_wf,(unsigned int)atoi(num_char));
                        print_wordfield(num_wf);
                        IC++;
                    }
                    log_out(__FILE__,__LINE__,"last directive word: [%s]\n",word);
                    set_wordfield_by_num(last_num_wf,(unsigned int)atoi(word));
                    print_wordfield(last_num_wf);
                    break;
                case DIRECTIVE_TYPE_EXTERN:
                    break;
                case DIRECTIVE_TYPE_ENTRY:
                    break;
                default:
                    break;
                }
            }
            else
            {
                /*
                    TODO: 
                    1. make sure this symbol check is correct
                    2. make sure to check if symbol already exists or
                       if the name is valid (no macro names with the same name)- handle errors
                    3. replace this with a function (?) 
                */
                if(strrchr(word,':')) 
                {
                    
                    char* name;
                    name = remove_last_character(word);
                    label_table_add(label_table,name,IC,LABELTYPE_CODE); /* check if already exists in inside label_table_add() */

                }
            }
        }
        IC++;
    }
    printf("\n\n");
    label_table_print(label_table);
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
    else /* addressing mode: 3 */
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
    wordfield* wf = init_wordfield();
    set_wordfield_by_num(wf,(int)*character);
    log_out(__FILE__,__LINE__,"char: [%c] | ascii_code: [%d] | ",*character,(int)*character);
    print_wordfield(wf);
    return wf;
}

