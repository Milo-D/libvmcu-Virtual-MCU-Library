/* Instruction (Datastructure) Header */

#ifndef VMCU_INSTR_H
#define VMCU_INSTR_H

// C Headers
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/operand.h"
#include "engine/include/analyzer/report/mnemonic.h"
#include "engine/include/analyzer/report/access.h"

// Project Headers (engine utilities)
#include "engine/include/arch/enum/ikey.h"
#include "engine/include/arch/enum/group.h"

typedef struct vmcu_instr {

    struct {

        VMCU_IKEY key;

        int opcode;
        int addr;

        bool exec;
        bool dword;
    };

    struct {

        VMCU_GROUP group;

        vmcu_access_t writes;
        vmcu_access_t reads;
    };

    struct {

        vmcu_operand_t src;
        vmcu_operand_t dest;
    };

    struct {

        vmcu_mnemonic_t mnem;
    };

} vmcu_instr_t;

#endif
