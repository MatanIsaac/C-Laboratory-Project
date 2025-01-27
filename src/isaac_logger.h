#ifndef ISAAC_LOGGER_H
#define ISAAC_LOGGER_H

#include <stdio.h>
#include <stdarg.h>

/* Logs a message to stdout with file and line info */
void log_info(const char *msg, const char *file, int line)
{
    fprintf(stdout, "[LOG] File: %s | Line: %d | Date: %s | Time: %s\n",
            file, line, __DATE__, __TIME__);
    fprintf(stdout, "\t\tINFO: %s\n\n", msg);
}

/* Logs a message to stdout with file and line info */
void log_err(const char *msg, const char *file, int line)
{
    fprintf(stderr, "[LOG] File: %s | Line: %d | Date: %s | Time: %s\n",
            file, line, __DATE__, __TIME__);
    fprintf(stderr, "\t\tERROR: %s\n\n", msg);
}

/* Logs a message to a file at a specified location */
void log_to_file(const char *filepath, const char *msg, const char *file, int line)
{
    FILE *fp = fopen(filepath, "a"); /* Open in append mode */
    if (fp != NULL)
    {
        fprintf(fp, "[LOG] File: %s | Line: %d | Date: %s | Time: %s\n",
                file, line, __DATE__, __TIME__);
        fprintf(fp, "\t\tMessage: %s\n\n", msg);
        fclose(fp);
    }
}

/* Workaround: Use a separate function for formatted output */
void fmt_log_out(const char *file, int line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stdout, "[LOG] File: %s | Line: %d | Date: %s | Time: %s\n",
            file, line, __DATE__, __TIME__);
    fprintf(stdout, "\tINFO: ");
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n\n");

    va_end(args);
}

void fmt_log_error(const char *file, int line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[LOG] File: %s | Line: %d | Date: %s | Time: %s\n",
            file, line, __DATE__, __TIME__);
    fprintf(stderr, "\tERROR: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n\n");

    va_end(args);
}
#define ISAAC_LOG_OUT(msg) log_out(msg, __FILE__, __LINE__)
#define ISAAC_LOG_ERR(msg) log_err(msg, __FILE__, __LINE__)
#define ISAAC_LOG_FILE(filepath, msg) log_to_file(filepath, msg, __FILE__, __LINE__)

#endif
