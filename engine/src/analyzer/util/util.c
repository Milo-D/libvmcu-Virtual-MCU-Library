/* Analyzer Utilities Implementation */

// Project Headers (engine)
#include "engine/include/analyzer/util/util.h"
#include "engine/include/analyzer/report/instr.h"

int vmcu_get_abs_addr(const vmcu_instr_t *instr) {

    if(instr->key == VMCU_IKEY_CALL)
        return instr->src.value;

    if(instr->key == VMCU_IKEY_JMP)
        return instr->src.value;

    if(instr->src.value < 0) {

        if((instr->src.value * -1) > instr->addr)
            return -1;
    }

    return (instr->addr + instr->src.value + 1);
}