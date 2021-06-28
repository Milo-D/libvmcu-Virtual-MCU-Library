/* Instruction Annotator Header */

#ifndef VMCU_ANNOTATOR_H
#define VMCU_ANNOTATOR_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_instr vmcu_instr_t;
typedef struct vmcu_model vmcu_model_t;

extern int vmcu_annotate_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);
extern vmcu_instr_t* vmcu_annotate_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu);

#endif
