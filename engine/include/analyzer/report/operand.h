/* Operand Structure Header */

#ifndef VMCU_OPERAND_H
#define VMCU_OPERAND_H

// C Headers
#include <inttypes.h>

// Project Headers (engine utilities)
#include "engine/include/arch/enum/op.h"

typedef struct vmcu_operand {
    
    int32_t value;
    VMCU_OP type;
    
} vmcu_operand_t;

#endif
