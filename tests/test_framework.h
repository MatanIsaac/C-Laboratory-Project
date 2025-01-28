#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <time.h>
#include "isaac_logger.h"

typedef enum 
{
    TEST_PASS,    /* The test passed successfully */
    TEST_FAIL,    /* The test failed  */
    TEST_OTHER    /* Any other non-pass/fail result (e.g., skipped, error, timeout, crash)  */
} TestResultType;

/* Function to convert TestResultType to a string */
const char* test_result_to_string(TestResultType result) 
{
    switch (result) 
    {
        case TEST_PASS:  return "PASS";
        case TEST_FAIL:  return "FAIL";
        case TEST_OTHER: return "OTHER";
        default:         return "UNKNOWN";
    }
}

/* Function to log test results */
void log_test(const char* test_name, TestResultType result, const char* details, const char* log_file) 
{
    /* Get current timestamp */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    /* Print to console with improved alignment */
    log_error(__FILE__,__LINE__,"[%s] Test:     [%s]\n"
           "                      Result:   [%s]\n"
           "                      Details:  [%s]\n\n",
           timestamp, test_name, test_result_to_string(result), details ? details : "N/A");

    /* Write to log file if specified */
    if (log_file) 
    {
        FILE *file = fopen(log_file, "a");
        if (file) 
        {
            log_error(__FILE__,__LINE__, "[%s] Test:     [%s]\n"
                          "              Result:   [%s]\n"
                          "              Details:  [%s]\n\n",
                    timestamp, test_name, test_result_to_string(result), details ? details : "N/A");
            fclose(file);
        }
        else 
        {
            log_error(__FILE__,__LINE__,"Warning: Could not open log file %s for writing.\n", log_file);
        }
    }
}


#endif /* TEST_FRAMEWORK_H */
