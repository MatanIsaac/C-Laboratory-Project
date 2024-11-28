#include "isaac_hashtable.h"

int main()
{
    HashTable* symbol_table = NULL;
    const char* labels[] = {"START", "LOOP", "END", "FUNC", "VAR"};
    const char* addresses[] = {"0x0000", "0x0010", "0x0020", "0x0030", "0x0040"};
    size_t num_labels = 5;
    size_t i;

    symbol_table = isaac_hashtable_create(10);

    /* Insert labels and addresses into the symbol table */
    for (i = 0; i < num_labels; i++)
    {
        isaac_hashtable_insert(symbol_table, labels[i], addresses[i]);
    }

    /* Print the symbol table */
    isaac_hashtable_print(symbol_table);

    /* Clean up */
    isaac_hashtable_destroy(symbol_table);
    
    return 0;
}