/* Instruction Decomposer Header */

#ifndef VMCU_DECOMPOSER_H
#define VMCU_DECOMPOSER_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_instr vmcu_instr_t;
typedef struct vmcu_model vmcu_model_t;

extern int vmcu_decompose_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);
extern vmcu_instr_t* vmcu_decompose_file(const char *file, uint32_t *size, vmcu_model_t *mcu);

#endif
