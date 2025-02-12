#include "label_table.h"
#include "isaac_logger.h"

const char* labeltype_to_string(enum LabelType type) 
{
    static const char* labels[] = { "code", "data" };

    if (type < 0 || type > 1) return "UNKNOWN";
    return labels[type];
}


void label_table_create(LabelTable* table) 
{
    table->labels = (LabelNode*)malloc(LABEL_TABLE_DEFAULT_SIZE * sizeof(LabelNode));
    if (!table->labels) 
    {
        log_error(__FILE__,__LINE__,  "Failed to allocate memory for the label table!\n");
        exit(EXIT_FAILURE);
    }
    table->size = 0;
    table->capacity = LABEL_TABLE_DEFAULT_SIZE;
}

void label_table_destroy(LabelTable* table) 
{
    if (table->labels) 
    {
        free(table->labels);
        table->labels = NULL;
    }
    table->size = 0;
    table->capacity = 0;
}

/* Dynamically adds a label to the table */
void label_table_add(LabelTable* table, const char* name, unsigned int address, enum LabelType type) 
{
    if(label_table_search(table,name) > 1)
    {
        log_error(__FILE__,__LINE__, "Error, label already exists in the label table!\n");
        return;
    }

    if (table->size >= table->capacity) 
    {
        /* Grow the array dynamically (double the capacity) */
        table->capacity *= 2;
        table->labels = (LabelNode*)realloc(table->labels, table->capacity * sizeof(LabelNode));
        if (!table->labels) 
        {
            log_error(__FILE__,__LINE__, "Failed to reallocate memory for the label table!\n");
            exit(EXIT_FAILURE);
        }
    }

    table->labels[table->size].name = name;
    table->labels[table->size].address = address;
    table->labels[table->size].type = type;
    table->size++;
}

void label_table_print(LabelTable* table) 
{
    unsigned int i = 0;

    if(table->size == 0)
    {
        log_out(__FILE__,__LINE__, "Label Table is empty.\n");    
    }

    log_out(__FILE__,__LINE__, "| %-6s | %-10s | %-6s |\n", "Label", "Address", "Type");
    log_out(__FILE__,__LINE__, "---------------------------------\n");

    for (; i < table->size; i++) 
    {
        fprintf(stdout, "| %-6s | %-10u | %-6s |\n",
               table->labels[i].name,
               table->labels[i].address,
               labeltype_to_string(table->labels[i].type));
    }
}

void label_node_print(LabelNode* node)
{
    if (!node) 
    {
        log_error(__FILE__,__LINE__, "Error: NULL label node\n");
        return;
    }

    log_error(__FILE__,__LINE__, "[name: %s] - [address: %u, type: %s]\n",
            node->name, node->address, labeltype_to_string(node->type));
}

int label_table_search(LabelTable* table, const char* name)
{
    unsigned int i = 0;
    for (; i < table->size; i++)
    {
        if(strcmp(name,table->labels[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int label_table_search_by_address(LabelTable* table, unsigned int address)
{
    unsigned int i = 0;
    for (; i < table->size; i++)
    {
        if(table->labels[i].address == address)
        {
            return i;
        }
    }
    return -1;
}

int label_table_set_node_by_name(LabelTable* table,const char* name, unsigned int address, enum LabelType type)
{
    int node_index = -1;
    if(table == NULL || name == NULL)
    {
        log_out(__FILE__,__LINE__, "Error setting label node.\n");    
        return -1;
    }
    if((node_index = label_table_search(table,name)) > 1)
    {
        table->labels[node_index].type      = type;
        table->labels[node_index].address   = address;
        return 1;
    }   
    return -1; 
}

int label_table_set_label_type(LabelTable* table,unsigned int address, enum LabelType type)
{
    int node_index = -1;
    if((node_index = label_table_search_by_address(table,address)) > 1)
    {
        table->labels[node_index].type      = type;
        return 1;
    }
    return -1; 
}