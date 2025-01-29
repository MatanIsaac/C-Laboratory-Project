#ifndef BINARY_TABLE_H
#define BINARY_TABLE_H
#include "wordfield.h"

typedef struct 
{
    unsigned int    address;
    wordfield       word;
} BinaryNode;

void print_binary_node(BinaryNode* node);



#endif