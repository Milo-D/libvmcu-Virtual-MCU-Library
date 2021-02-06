/* File Manipulation Header */

#ifndef FILEMANIP_H
#define FILEMANIP_H

// C Headers
#include <stdbool.h>

typedef struct _array array_t;

extern long fbytes(const char *file);
extern bool file_exists(const char *file);
extern bool is_hex_file(const char *file);
extern bool check_files(const int argc, char **argv);
extern void get_files(const int argc, char **argv, array_t *buffer);

#endif