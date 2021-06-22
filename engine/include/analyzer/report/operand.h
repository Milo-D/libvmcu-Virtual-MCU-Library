/* Operand Structure Header */

#ifndef VMCU_OPERAND_H
#define VMCU_OPERAND_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/registerpair.h"

// Project Headers (engine utilities)
#include "engine/include/arch/enum/optype.h"
#include "engine/include/arch/enum/register.h"

typedef struct vmcu_operand {

    union {

        uint8_t k;
        uint8_t b;
        uint8_t io;

        uint16_t d;
        uint32_t p;
        int16_t  s;

        VMCU_REGISTER r;
        vmcu_registerpair_t rp;
    };

    VMCU_OPTYPE type;
    
} vmcu_operand_t;

#endif
