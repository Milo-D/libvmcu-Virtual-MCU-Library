/* Instruction (Datastructure) Header */

#ifndef VMCU_INSTR_H
#define VMCU_INSTR_H

// C Headers
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/operand.h"

// Project Headers (engine utilities)
#include "engine/include/arch/enum/ikey.h"
#include "engine/include/arch/enum/group.h"

typedef struct vmcu_instr {

    struct {

        VMCU_IKEY key;
        VMCU_GROUP group;
    };

    int opcode;
    int addr;

    bool exec;
    bool dword;

    char *mnem;

    vmcu_operand_t src;
    vmcu_operand_t dest;

} vmcu_instr_t;

#endif
