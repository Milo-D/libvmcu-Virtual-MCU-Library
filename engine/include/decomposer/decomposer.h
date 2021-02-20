/* Instruction Decomposer Header */

#ifndef VMCU_DECOMPOSER_H
#define VMCU_DECOMPOSER_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_plain vmcu_plain_t;

extern int vmcu_decompose_bytes(const uint32_t bytes, vmcu_plain_t *p);
extern vmcu_plain_t* vmcu_decompose_ihex(const char *hex_file, int32_t *size);

#endif
