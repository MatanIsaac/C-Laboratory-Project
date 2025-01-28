#ifndef ISAAC_LOGGER_H
#define ISAAC_LOGGER_H

#include <stdio.h>
#include <stdarg.h>

/* Workaround: Use a separate function for formatted output */
void log_out(const char *file, int line, const char *fmt, ...);
void log_error(const char *file, int line, const char *fmt, ...);


#endif
