#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @brief Default capacity of the label table. */
#define LABEL_TABLE_DEFAULT_SIZE 10

/**
 * @brief Possible types of labels.
 */
enum LabelType 
{
    LABELTYPE_CODE,
    LABELTYPE_DATA,
    LABELTYPE_EXTERN,
    LABELTYPE_ENTRY,
    LABELTYPE_CODE_ENTRY,
    LABELTYPE_DATA_ENTRY
};

/**
 * @brief Converts a LabelType to its string representation.
 * @param type The label type.
 * @return A string representing the type.
 */
const char* labeltype_to_string(enum LabelType type);

/**
 * @brief Represents a single label entry.
 */
typedef struct LabelNode
{
    char* name;              /* Label name */
    unsigned int address;    /* Label address */
    enum LabelType type;     /* Label type */

} LabelNode;

/**
 * @brief Manages a dynamic array of LabelNode entries.
 */
typedef struct LabelTable
{
    LabelNode* labels;       /* Array of labels */
    unsigned int size;       /* Current number of labels */
    unsigned int capacity;   /* Allocated capacity */

} LabelTable;

/**
 * @brief Initializes a label table.
 * @param table Pointer to the LabelTable to initialize.
 */
void label_table_create(LabelTable* table);

/**
 * @brief Frees resources used by a label table.
 * @param table Pointer to the LabelTable to destroy.
 */
void label_table_destroy(LabelTable* table);

/**
 * @brief Adds a label entry to the table.
 * @param table Pointer to the LabelTable.
 * @param name The label name.
 * @param address The label address.
 * @param type The label type.
 */
void label_table_add(LabelTable* table, char* name, unsigned int address, enum LabelType type);

/**
 * @brief Prints all labels in the table.
 * @param table Pointer to the LabelTable.
 */
void label_table_print(LabelTable* table);

/**
 * @brief Searches for a label by name.
 * @param table Pointer to the LabelTable.
 * @param name The label name to search for.
 * @return The index of the label if found, otherwise -1.
 */
int label_table_search(LabelTable* table, char* name);

/**
 * @brief Searches for a label by address.
 * @param table Pointer to the LabelTable.
 * @param address The label address to search for.
 * @return The index of the label if found, otherwise -1.
 */
int label_table_search_by_address(LabelTable* table, unsigned int address);

/**
 * @brief Updates a label's address and type by name.
 * @param table Pointer to the LabelTable.
 * @param name The label name.
 * @param address The new address.
 * @param type The new type.
 * @return The index if found and updated, otherwise -1.
 */
int label_table_set_node_by_name(LabelTable* table,char* name, unsigned int address, enum LabelType type);


/**
 * @brief Sets a label's type by its address.
 * @param table Pointer to the LabelTable.
 * @param address The label address.
 * @param type The new type.
 * @return The index if found and updated, otherwise -1.
 */
int label_table_set_label_type(LabelTable* table,unsigned int address, enum LabelType type);

/**
 * @brief Prints a single label node.
 * @param node Pointer to the LabelNode to print.
 */
void label_node_print(LabelNode* node);

#endif /* LABEL_TABLE_H */
