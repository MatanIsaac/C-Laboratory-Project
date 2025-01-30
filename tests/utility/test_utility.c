#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../test_framework.h"
#include "../../src/utility.h"
#include "../../src/isaac_logger.h"

/* Function prototypes */
void test_filename();
void test_string_functions();
void test_instruction_functions();
void test_directive_functions();
void test_hash_function();

int main()
{
    /*
    test_filename();
    test_instruction_functions();
    test_directive_functions();
    test_hash_function();
    */
    test_string_functions();
    
    return 0;
}

/* =======================
   Test: get_filename()
   ======================= */
void test_filename()
{
    char* name1 = NULL;
    char*  name2 = NULL;
    char* name3 = NULL;
    char* name4 = NULL;

    log_out(__FILE__,__LINE__, "Starting Test - get_filename function in utility.h\n");

    /* Test case 1: Valid file path */
    name1 = get_filename("input_files/preproccessor/preproc_invalid1.as");
    if (strcmp(name1, "preproc_invalid1") == 0)
    {
        log_test("Test_get_filename_ValidPath", TEST_PASS, "Extracted filename correctly.");
    }
    else
    {
        log_test("Test_get_filename_ValidPath", TEST_FAIL, "Filename extraction failed.");
    }
    free(name1);

    /* Test case 2: File without extension */
    name2 = get_filename("input_files/preproccessor/preproc_valid1");
    if (strcmp(name2, "preproc_valid1") == 0)
    {
        log_test("Test_get_filename_NoExtension", TEST_PASS, "Handled filename without extension.");
    }
    else
    {
        log_test("Test_get_filename_NoExtension", TEST_FAIL, "Incorrect filename extraction.");
    }
    free(name2);

    /* Test case 3: File with multiple dots */
    name3 = get_filename("input_files/preproccessor/pre.proc_valid.2..as");
    if (strcmp(name3, "preproc_valid2") != 0)
    {
        log_test("Test_get_filename_MultipleDots", TEST_PASS, "Handled multiple dots correctly.");
    }
    else
    {
        log_test("Test_get_filename_MultipleDots", TEST_FAIL, "Failed to handle multiple dots.");
    }
    free(name3);

    /* Test case 4: NULL input */
    name4 = get_filename(NULL);
    if (name4 == NULL)
    {
        log_test("Test_get_filename_NullInput", TEST_PASS, "Handled NULL input safely.");
    }
    else
    {
        log_test("Test_get_filename_NullInput", TEST_FAIL, "NULL input handling failed.");
        free(name4);
    }

    log_out(__FILE__,__LINE__, "Done Testing - get_filename function in utility.h\n");
}

/* =======================
   Test: String Allocation Functions
   ======================= */
void test_string_functions()
{
    char* str = NULL;
    char*  dup = NULL;

    log_out(__FILE__,__LINE__, "Starting Test - string memory allocation functions in utility.h\n");

    str = string_malloc(10);
    if (str)
    {
        log_test("Test_string_malloc", TEST_PASS, "Allocated memory successfully.");
        free(str);
    }
    else
    {
        log_test("Test_string_malloc", TEST_FAIL, "Memory allocation failed.");
    }

    str = string_calloc(5, sizeof(char));
    if (str)
    {
        log_test("Test_string_calloc", TEST_PASS, "Allocated and zero-initialized memory successfully.");
        free(str);
    }
    else
    {
        log_test("Test_string_calloc", TEST_FAIL, "Memory allocation failed.");
    }

    dup = my_strdup("Hello");
    if (dup && strcmp(dup, "Hello") == 0)
    {
        log_test("Test_my_strdup", TEST_PASS, "Duplicated string correctly.");
        free(dup);
    }
    else
    {
        log_test("Test_my_strdup", TEST_FAIL, "String duplication failed.");
    }


    str = remove_last_character("MAIN:");
    if (str && strcmp(str, "MAIN") == 0)
    {
        
        log_test("Test_remove_last_character", TEST_PASS, "Removing string's last character correctly.");
        free(str);
    }
    else
    {
        log_test("Test_remove_last_character", TEST_FAIL, "Removing string's last character failed.");
    } 

    str = remove_last_character("r3,");
    if (str && strcmp(str, "r3") == 0)
    {
        log_test("Test_remove_last_character", TEST_PASS, "Removing string's last character correctly.");
        free(str);
    }
    else
    {
        log_test("Test_remove_last_character", TEST_FAIL, "Removing string's last character failed.");
    } 

    str = strncpy_from_pos("hello, world",5);
    if (str && strcmp(str, ", world") == 0)
    {
        log_test("Test_strncpy_from_pos", TEST_PASS, "Copying n characters from position correctly.");
        free(str);
    }
    else
    {
        log_test("Test_strncpy_from_pos", TEST_FAIL, "Copying n characters from position failed.");
    } 

    str = remove_first_character("#37");
    if (str && strcmp(str, "37") == 0)
    {
        log_test("Test_remove_first_character", TEST_PASS, "Removing string's first character correctly.");
        free(str);
    }
    else
    {
        log_test("Test_remove_first_character", TEST_FAIL, "Removing string's first character failed.");
    }

    log_out(__FILE__,__LINE__, "Done Testing - string memory allocation functions in utility.h\n");
}

/* =======================
   Test: is_instruction()
   ======================= */
void test_instruction_functions()
{
    log_out(__FILE__,__LINE__, "Starting Test - is_instruction function in utility.h\n");

    if (is_instruction("mov"))
        log_test("Test_is_instruction_valid", TEST_PASS, "Recognized valid instruction.");
    else
        log_test("Test_is_instruction_valid", TEST_FAIL, "Failed to recognize instruction.");

    if (!is_instruction("invalid_op"))
        log_test("Test_is_instruction_invalid", TEST_PASS, "Correctly rejected invalid instruction.");
    else
        log_test("Test_is_instruction_invalid", TEST_FAIL, "Incorrectly recognized invalid instruction.");

    log_out(__FILE__,__LINE__, "Done Testing - is_instruction function in utility.h\n");
}

/* =======================
   Test: is_directive()
   ======================= */
void test_directive_functions()
{
    log_out(__FILE__,__LINE__, "Starting Test - is_directive function in utility.h\n");

    if (is_directive(".data"))
        log_test("Test_is_directive_valid", TEST_PASS, "Recognized valid directive.");
    else
        log_test("Test_is_directive_valid", TEST_FAIL, "Failed to recognize directive.");

    if (!is_directive(".unknown"))
        log_test("Test_is_directive_invalid", TEST_PASS, "Correctly rejected invalid directive.");
    else
        log_test("Test_is_directive_invalid", TEST_FAIL, "Incorrectly recognized invalid directive.");

    log_out(__FILE__,__LINE__, "Done Testing - is_directive function in utility.h\n");
}

/* =======================
   Test: hash() Function
   ======================= */
void test_hash_function()
{
    unsigned long hash1,hash2,hash3;

    log_out(__FILE__,__LINE__, "Starting Test - hash function in utility.h\n");

    hash1 = hash("test_string");
    hash2 = hash("test_string");

    if (hash1 == hash2)
        log_test("Test_hash_deterministic", TEST_PASS, "Hash function produced consistent results.");
    else
        log_test("Test_hash_deterministic", TEST_FAIL, "Hash function produced inconsistent results.");

    hash3 = hash("different_string");
    if (hash1 != hash3)
        log_test("Test_hash_unique", TEST_PASS, "Hash function produced unique values.");
    else
        log_test("Test_hash_unique", TEST_FAIL, "Hash function produced duplicate values.");

    log_out(__FILE__,__LINE__, "Done Testing - hash function in utility.h\n");
}
