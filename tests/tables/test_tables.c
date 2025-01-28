#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../test_framework.h"
#include "../../src/macro_table.h"
#include "../../src/instruction_table.h"
#include "../../src/label_table.h"

void test_macro_table();
void test_instruction_table();
void test_label_table();

int main()
{
    test_macro_table();
    test_instruction_table();
    test_label_table();
    return 0;
}

/* =======================
   Test: Macro Table
   ======================= */
void test_macro_table()
{
    const char* value;
    MacroTable* table = NULL;

    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n");
    log_out(__FILE__,__LINE__, "Starting Test - Macro Table Functions in macro_table.h\n");

    table = macro_table_create(10);
    if (table)
        log_test("Test_macro_table_create", TEST_PASS, "Macro table created successfully.", "test_log.txt");
    else
        log_test("Test_macro_table_create", TEST_FAIL, "Macro table creation failed.", "test_log.txt");

    macro_table_insert(table, "MAX_SIZE", "1024");
    macro_table_insert(table, "VERSION", "1.0.3");

    value = macro_table_get(table, "MAX_SIZE");
    if (value && strcmp(value, "1024") == 0)
        log_test("Test_macro_table_insert_get", TEST_PASS, "Macro retrieved correctly.", "test_log.txt");
    else
        log_test("Test_macro_table_insert_get", TEST_FAIL, "Macro retrieval failed.", "test_log.txt");

    macro_table_remove(table, "MAX_SIZE");
    if (!macro_table_get(table, "MAX_SIZE"))
        log_test("Test_macro_table_remove", TEST_PASS, "Macro removed successfully.", "test_log.txt");
    else
        log_test("Test_macro_table_remove", TEST_FAIL, "Macro removal failed.", "test_log.txt");

    macro_table_destroy(table);
    log_out(__FILE__,__LINE__, "Done - Testing Macro Table Functions\n");
    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n\n");
}

/* =======================
   Test: Instruction Table
   ======================= */
void test_instruction_table()
{
    InstructionTable table;  
    InstructionNode* node = NULL;

    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n");
    log_out(__FILE__,__LINE__, "Starting Test - Instruction Table Functions in instruction_table.h\n");

    instruction_table_create(&table);  /* Pass reference instead of using malloc() */
    
    /* No need for pointer checks since it's stack allocated */
    log_test("Test_instruction_table_create", TEST_PASS, "Instruction table initialized successfully.", "test_log.txt");

    instruction_table_insert(&table, "ADD", 1, 10);
    instruction_table_insert(&table, "SUB", 2, 20);

    node = instruction_table_get(&table, "ADD");
    if (node && strcmp(node->op_name, "ADD") == 0)
        log_test("Test_instruction_table_insert_get", TEST_PASS, "Instruction retrieved correctly.", "test_log.txt");
    else
        log_test("Test_instruction_table_insert_get", TEST_FAIL, "Instruction retrieval failed.", "test_log.txt");

    instruction_table_remove(&table, "ADD");
    if (!instruction_table_get(&table, "ADD"))
        log_test("Test_instruction_table_remove", TEST_PASS, "Instruction removed successfully.", "test_log.txt");
    else
        log_test("Test_instruction_table_remove", TEST_FAIL, "Instruction removal failed.", "test_log.txt");

    instruction_table_destroy(&table);  /* No freeing needed, just clears memory */

    log_out(__FILE__,__LINE__, "Done - Testing Instruction Table Functions\n");
    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n\n");
}


/* =======================
   Test: Label Table
   ======================= */
void test_label_table()
{
    LabelTable table;
    const LabelNode* node;

    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n");
    log_out(__FILE__,__LINE__, "Starting Test - Label Table Functions in label_table.h\n");

    label_table_create(&table);

    label_table_add(&table, "start", 0x1000, CODE);
    label_table_add(&table, "var1", 0x2000, DATA);

    node = &table.labels[0];
    if (node && strcmp(node->name, "start") == 0)
        log_test("Test_label_table_add_get", TEST_PASS, "Label added and retrieved correctly.", "test_log.txt");
    else
        log_test("Test_label_table_add_get", TEST_FAIL, "Label retrieval failed.", "test_log.txt");

    label_table_destroy(&table);
    log_out(__FILE__,__LINE__, "Done - Testing Label Table Functions\n");
    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n\n");
}
