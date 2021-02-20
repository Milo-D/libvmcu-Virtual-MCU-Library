/* Disassembler Header */

#ifndef VMCU_DISASSEMBLER_H
#define VMCU_DISASSEMBLER_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_plain vmcu_plain_t;

extern int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_plain_t *p);
extern vmcu_plain_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size);

#endif