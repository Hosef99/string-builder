#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define HANDLE_ERROR(message) handle_error(message, __FILE__, __LINE__, __func__)

static inline void handle_error(char *message, char *file, int line, const char *func) {
    fprintf(stderr, "Error: %s\n", message);
#ifdef SB_DEBUG
    fprintf(stderr, "in file: %s, line %d, function: %s\n", file, line, func);
#endif
    abort();
}

#endif
