/* Analyzer Utilities Header */

#ifndef VMCU_UTIL_H
#define VMCU_UTIL_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_instr vmcu_instr_t;

extern int64_t vmcu_resolve_flow(const vmcu_instr_t *instr);

#endif
