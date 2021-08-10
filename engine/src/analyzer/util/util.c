/* Analyzer Utilities Implementation */

// Project Headers (engine)
#include "engine/include/analyzer/util/util.h"
#include "engine/include/analyzer/report/instr.h"

int64_t vmcu_resolve_flow(const vmcu_instr_t *instr) {

    /* this function only accepts branches, calls/jumps
     * todo: handle flash wrap arounds
     * */

    if(instr->key == VMCU_IKEY_CALL || instr->key == VMCU_IKEY_JMP)
        return instr->src.p;

    if(instr->src.s < 0) {

        if((uint32_t) (instr->src.s * -1) > instr->addr)
            return -1;
    }

    return (instr->addr + instr->src.s + 1);
}