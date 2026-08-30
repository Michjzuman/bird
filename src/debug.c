#include <stdio.h>
#include <stdarg.h>

const char path[] = "debug.log";

void clear_debug_log(void) {
    FILE *file = fopen(path, "w");
    fprintf(file, "");
    fclose(file);
}

void debug_log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    FILE *file = fopen(path, "a");
    vfprintf(file, format, args);
    fclose(file);
    va_end(args);
}