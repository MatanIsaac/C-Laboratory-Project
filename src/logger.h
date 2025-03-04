#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Logs a message to stdout with file and line info.
 * @param file The file name.
 * @param line The line number.
 * @param fmt The format string.
 */
void log_out(const char *file, int line, const char *fmt, ...);

/**
 * @brief Logs a message to stderr with file and line info.
 * @param file The file name.
 * @param line The line number.
 * @param fmt The format string.
 */
void log_error(const char *file, int line, const char *fmt, ...);


/**
 * @brief Logs a message to the given file pointer with file and line info.
 * @param fp The file pointer to write to.
 * @param file The source file name.
 * @param line The line number.
 * @param fmt The format string.
 */
void log_file(FILE *fp, const char *file, int line, const char *fmt, ...);



#endif
