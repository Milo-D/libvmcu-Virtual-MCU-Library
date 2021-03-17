/* xref (from, to) Structure Header */

#ifndef VMCU_XREF_H
#define VMCU_XREF_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_instr vmcu_instr_t;

typedef struct vmcu_xref {

    vmcu_instr_t *i;

} vmcu_xref_t;

#endif