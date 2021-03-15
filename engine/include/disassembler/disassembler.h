/* Disassembler Header */

#ifndef VMCU_DISASSEMBLER_H
#define VMCU_DISASSEMBLER_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_instr vmcu_instr_t;
typedef struct vmcu_model vmcu_model_t;

extern int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_instr_t *p, vmcu_model_t *mcu);
extern vmcu_instr_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu);

#endif