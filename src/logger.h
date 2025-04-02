#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Logs a general informational message to the standard output.
 *
 * This function provides a formatted log message that includes file name, line number,
 * compilation date, and time. The actual message is formatted using printf-style arguments.
 *
 * @param file   The file where the log message is triggered (typically use __FILE__).
 * @param line   The line number where the log message is triggered (typically use __LINE__).
 * @param fmt    Format string for the log message (like printf).
 * @param ...    Additional arguments for the format string.
 */
void log_out(const char *file, int line, const char *fmt, ...);

/**
 * @brief Logs an error message to the standard error stream.
 *
 * Outputs a formatted error message along with metadata including file name,
 * line number, and compilation timestamp. This is intended for critical or failure-related logs.
 *
 * @param file   The file where the error occurred (typically use __FILE__).
 * @param line   The line number where the error occurred (typically use __LINE__).
 * @param fmt    Format string for the error message (like printf).
 * @param ...    Additional arguments for the format string.
 */
void log_error(const char *file, int line, const char *fmt, ...);


/**
 * @brief Logs a formatted message to the given file stream.
 *
 * Outputs a message formatted similarly to log_error, but writes to a custom file stream
 * (e.g., log file or external file), instead of standard output or error.
 *
 * @param fp     The file stream to write the log into.
 * @param file   The file where the log is triggered (typically use __FILE__).
 * @param line   The line number where the log is triggered (typically use __LINE__).
 * @param fmt    Format string for the message.
 * @param ...    Additional arguments for the format string.
 */
void log_file(FILE *fp, const char *file, int line, const char *fmt, ...);

#endif
