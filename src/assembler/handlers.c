#include <stddef.h>
#include "handlers.h"

static inline void write_word(uint16_t* write_ptr, const uint16_t opcode) {

    *write_ptr = opcode;
}

/* Exported */

vmcu_rc_t pack_and_assemble_nop(uint16_t* write_ptr, const va_list operands) {

    return assemble_nop(write_ptr, NULL, NULL);
}

vmcu_rc_t assemble_nop(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src) {

    write_word(write_ptr, 0x0000);
    return VMCU_RC_OK;
}

/*
 
void pack_and_assemble_ldi(uint16_t* write_ptr, const va_list operands) {

    vmcu_operand_t dest = {

        .type = VMCU_OPTYPE_R,
        .r    = va_arg(operands, vmcu_register_t)
    };

    vmcu_operand_t src = {

        .type = VMCU_OPTYPE_K8,
        .imm8 = va_arg(operands, uint8_t)
    };

    return assemble_ldi(write_ptr, addr, &dest, &src);
}

 */
