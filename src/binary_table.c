#include "binary_table.h"
#include "isaac_logger.h"

void print_binary_node(BinaryNode* node)
{
    if (node)
    {
        /* Found a valid entry */
        log_out(__FILE__,__LINE__,  "[address: %u] - \n", node->address);
        print_wordfield(&node->word);
    }
    else
    {
        /* This slot is empty or invalid */
        log_error(__FILE__,__LINE__,  "(empty)\n");
    }
}