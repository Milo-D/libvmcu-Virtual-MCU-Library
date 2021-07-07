/* Analyzer Utilities Implementation */

// Project Headers (engine)
#include "engine/include/analyzer/util/util.h"
#include "engine/include/analyzer/report/instr.h"

int vmcu_resolve_flow(const vmcu_instr_t *instr) {

    /* this function only accepts branches, calls/jumps */

    if(instr->key == VMCU_IKEY_CALL || instr->key == VMCU_IKEY_JMP)
        return instr->src.p;

    if(instr->src.s < 0) {

        if((instr->src.s * -1) > instr->addr)
            return -1;
    }

    return (instr->addr + instr->src.s + 1);
}