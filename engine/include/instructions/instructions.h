/* Assembly Instruction Set Header */

#ifndef VMCU_INSTRUCTIONS_H
#define VMCU_INSTRUCTIONS_H

#define INSTR_MAX 131

// C Headers
#include <inttypes.h>

typedef struct vmcu_system vmcu_system_t;

extern void (*vmcu_execute[INSTR_MAX]) (vmcu_system_t *sys, const uint32_t opcode);

#endif
