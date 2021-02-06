/* Instruction Decomposer Header */

#ifndef DECOMPOSER_H
#define DECOMPOSER_H

typedef struct _array array_t;

extern int decompose(const char *hex_file, array_t *buffer);

#endif
