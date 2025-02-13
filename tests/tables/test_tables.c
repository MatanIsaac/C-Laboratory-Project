#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../test_framework.h"
#include "../../src/isaac_logger.h"
#include "../../src/macro_table.h"
#include "../../src/instruction_table.h"
#include "../../src/label_table.h"

void test_macro_table();
void test_macro_table_advanced();
void test_instruction_table();
void test_label_table();

int main()
{
    test_macro_table();
    test_macro_table_advanced();
    /*test_instruction_table();
    test_label_table();*/
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
        log_test("Test_macro_table_create", TEST_PASS, "Macro table created successfully.");
    else
        log_test("Test_macro_table_create", TEST_FAIL, "Macro table creation failed.");

    macro_table_insert(table, "MAX_SIZE", "1024");
    macro_table_insert(table, "VERSION", "1.0.3");

    value = macro_table_get(table, "MAX_SIZE");
    if (value && strcmp(value, "1024") == 0)
        log_test("Test_macro_table_insert_get", TEST_PASS, "Macro retrieved correctly.");
    else
        log_test("Test_macro_table_insert_get", TEST_FAIL, "Macro retrieval failed.");

    macro_table_remove(table, "MAX_SIZE");
    if (!macro_table_get(table, "MAX_SIZE"))
        log_test("Test_macro_table_remove", TEST_PASS, "Macro removed successfully.");
    else
        log_test("Test_macro_table_remove", TEST_FAIL, "Macro removal failed.");

    macro_table_destroy(table);
    log_out(__FILE__,__LINE__, "Done - Testing Macro Table Functions\n");
    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n\n");
}

/* 
 * Tests your MacroTable with more complex scenarios:
 * - Multiple inserts
 * - Duplicate macro names
 * - Removals
 * - Resetting the table
 * - Checking invalid gets
 */
void test_macro_table_advanced()
{
    const char* value;
    MacroTable* table = NULL;

    log_out(__FILE__, __LINE__, "#---------------------------------------------------------#\n");
    log_out(__FILE__, __LINE__, "Starting Advanced Test - Macro Table Functions\n\n");

    /* 1) Create a fresh table */
    table = macro_table_create(DEFAULT_MACRO_TABLE_SIZE);
    if (table) 
    {
        log_test("Advanced_macro_table_create", TEST_PASS, "Macro table created successfully.");
    } 
    else 
    {
        log_test("Advanced_macro_table_create", TEST_FAIL, "Macro table creation failed.");
        return; /* If we can’t create, no point continuing! */
    }

    /* 2) Insert some macros typically seen in an assembler scenario */
    macro_table_insert(table, "BEGIN", "start_of_code_segment");
    macro_table_insert(table, "LOOP_MACRO", "loop_body_goes_here");
    macro_table_insert(table, "PRINT_INT", "print integer routine");
    macro_table_insert(table, "PRINT_STR", "print string routine");
    macro_table_insert(table, "END", "end_of_code_segment");

    /* 3) Verify we can retrieve them */
    value = macro_table_get(table, "BEGIN");
    if (value && strcmp(value, "start_of_code_segment") == 0) 
    {
        log_test("Advanced_macro_table_insert_get_1", TEST_PASS, 
                 "Retrieved macro 'BEGIN' correctly.");
    } 
    else 
    {
        log_test("Advanced_macro_table_insert_get_1", TEST_FAIL, 
                 "Macro retrieval for 'BEGIN' failed.");
    }

    value = macro_table_get(table, "PRINT_STR");
    if (value && strcmp(value, "print string routine") == 0) 
    {
        log_test("Advanced_macro_table_insert_get_2", TEST_PASS,
                 "Retrieved macro 'PRINT_STR' correctly.");
    } 
    else 
    {
        log_test("Advanced_macro_table_insert_get_2", TEST_FAIL, 
                 "Macro retrieval for 'PRINT_STR' failed.");
    }

    /* 4) Try removing a macro and verifying it’s gone */
    macro_table_remove(table, "LOOP_MACRO");
    if (!macro_table_get(table, "LOOP_MACRO")) 
    {
        log_test("Advanced_macro_table_remove_1", TEST_PASS, 
                 "Removed macro 'LOOP_MACRO' successfully.");
    } 
    else 
    {
        log_test("Advanced_macro_table_remove_1", TEST_FAIL, 
                 "Macro removal for 'LOOP_MACRO' failed.");
    }

    /* 5) Insert a duplicate macro name to see if it’s overwritten or logged */
    macro_table_insert(table, "BEGIN", "redefined_code_segment");
    value = macro_table_get(table, "BEGIN");
    /* In your current code, if the macro already exists, you log 
       'Macro Node index not empty.' and do not overwrite. 
       So let's see if it changed or not. */
    if (value && strcmp(value, "start_of_code_segment") == 0) 
    {
        log_test("Advanced_macro_table_duplicate_insert", TEST_PASS, 
                 "Duplicate insert didn't overwrite. index not empty.");
    } 
    else 
    {
        log_test("Advanced_macro_table_duplicate_insert", TEST_FAIL, 
                 "Duplicate insert logic is broken or macro is missing.");
    }

    /* 6) Try retrieving a macro that doesn’t exist */
    value = macro_table_get(table, "NOT_DEFINED");
    if (!value) 
    {
        log_test("Advanced_macro_table_get_invalid", TEST_PASS, 
                 "Fetching 'NOT_DEFINED' correctly returned NULL.");
    } 
    else 
    {
        log_test("Advanced_macro_table_get_invalid", TEST_FAIL, 
                 "Should have returned NULL for undefined macro, but got something else.");
    }

    /* 7) Now let’s test macro_table_reset (destroys then recreates) */
    macro_table_reset(&table);  /* pointer-to-pointer version */
    if (table != NULL) 
    {
        log_test("Advanced_macro_table_reset_1", TEST_PASS,
                 "macro_table_reset succeeded, got a fresh table.");
    } 
    else 
    {
        log_test("Advanced_macro_table_reset_1", TEST_FAIL,
                 "macro_table_reset returned NULL table.");
        return;
    }

    /* Right after reset, everything should be empty */
    value = macro_table_get(table, "BEGIN");
    if (!value) 
    {
        log_test("Advanced_macro_table_reset_2", TEST_PASS,
                 "After reset, 'BEGIN' is indeed gone.");
    } 
    else 
    {
        log_test("Advanced_macro_table_reset_2", TEST_FAIL,
                 "After reset, 'BEGIN' somehow still exists. Did we free everything?");
    }

    /* Insert again to confirm new table is fully operational */
    macro_table_insert(table, "ERR_CHECK", "some error macro definition");
    if (macro_table_get(table, "ERR_CHECK")) 
    {
        log_test("Advanced_macro_table_insert_after_reset", TEST_PASS,
                 "Inserted new macro after reset successfully.");
    } 
    else 
    {
        log_test("Advanced_macro_table_insert_after_reset", TEST_FAIL,
                 "Could not retrieve macro after reset insertion.");
    }

    /* 8) Destroy the table to free everything */
    macro_table_destroy(table);
    table = NULL;

    /* We can’t test retrieval anymore—table is destroyed—so we’re done. */
    log_out(__FILE__, __LINE__, 
            "Done - Advanced Macro Table Testing\n");
    log_out(__FILE__, __LINE__, 
            "#---------------------------------------------------------#\n\n");
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
    log_test("Test_instruction_table_create", TEST_PASS, "Instruction table initialized successfully.");

    instruction_table_insert(&table, "ADD", 1, 10);
    instruction_table_insert(&table, "SUB", 2, 20);

    node = instruction_table_get(&table, "ADD");
    if (node && strcmp(node->op_name, "ADD") == 0)
        log_test("Test_instruction_table_insert_get", TEST_PASS, "Instruction retrieved correctly.");
    else
        log_test("Test_instruction_table_insert_get", TEST_FAIL, "Instruction retrieval failed.");

    instruction_table_remove(&table, "ADD");
    if (!instruction_table_get(&table, "ADD"))
        log_test("Test_instruction_table_remove", TEST_PASS, "Instruction removed successfully.");
    else
        log_test("Test_instruction_table_remove", TEST_FAIL, "Instruction removal failed.");

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

    label_table_add(&table, "start", 0x1000, LABELTYPE_CODE);
    label_table_add(&table, "var1", 0x2000, LABELTYPE_DATA);

    node = &table.labels[0];
    if (node && strcmp(node->name, "start") == 0)
        log_test("Test_label_table_add_get", TEST_PASS, "Label added and retrieved correctly.");
    else
        log_test("Test_label_table_add_get", TEST_FAIL, "Label retrieval failed.");

    label_table_destroy(&table);
    log_out(__FILE__,__LINE__, "Done - Testing Label Table Functions\n");
    log_out(__FILE__,__LINE__, "#---------------------------------------------------------#\n\n");
}
