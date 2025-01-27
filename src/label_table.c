#include "label_table.h"

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
        fprintf(stderr, "Failed to allocate memory for the label table!\n");
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
    if (table->size >= table->capacity) 
    {
        /* Grow the array dynamically (double the capacity) */
        table->capacity *= 2;
        table->labels = (LabelNode*)realloc(table->labels, table->capacity * sizeof(LabelNode));
        if (!table->labels) 
        {
            fprintf(stderr, "Failed to reallocate memory for the label table!\n");
            exit(EXIT_FAILURE);
        }
    }

    table->labels[table->size].name = name;
    table->labels[table->size].address = address;
    table->labels[table->size].type = type;
    table->size++;
}


void label_node_print(LabelNode* node)
{
    if (!node) 
    {
        fprintf(stderr, "Error: NULL label node\n");
        return;
    }

    fprintf(stdout, "[name: %s] - [address: %u, type: %s]\n",
            node->name, node->address, labeltype_to_string(node->type));
}


void label_table_print(LabelTable* table) 
{
    unsigned int i = 0;

    printf("| %-6s | %-10s | %-6s |\n", "Label", "Address", "Type");
    printf("---------------------------------\n");

    for (; i < table->size; i++) 
    {
        printf("| %-6s | %-10u | %-6s |\n",
               table->labels[i].name,
               table->labels[i].address,
               labeltype_to_string(table->labels[i].type));
    }
}
