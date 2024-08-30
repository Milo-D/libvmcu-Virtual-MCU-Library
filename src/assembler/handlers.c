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

vmcu_rc_t pack_and_assemble_movw(uint16_t* write_ptr, const va_list operands) {

    /*
     * todo: rework vmcu_operand_t
     * */

    vmcu_register_t rd, rr;

    rd = va_arg(operands, vmcu_register_t);
    rr = va_arg(operands, vmcu_register_t);

    vmcu_operand_t dest = {

        .type = VMCU_OPTYPE_RP,
        .rp   = {

            .low  = rd,
            .high = rd + 1
        }
    };

    vmcu_operand_t src = {

        .type = VMCU_OPTYPE_RP,
        .rp   = {

            .low  = rr,
            .high = rr + 1
        }
    };

    return assemble_movw(write_ptr, &dest, &src);
}

vmcu_rc_t assemble_movw(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src) {

    vmcu_register_t rd, rr;

    rd = dest->rp.low;
    rr = src->rp.low;

    if (rd < VMCU_REGISTER_R0 || rd > VMCU_REGISTER_R30 || rd % 2 == 1)
        return VMCU_RC_ERR_OP_DEST;

    if (rr < VMCU_REGISTER_R0 || rr > VMCU_REGISTER_R30 || rr % 2 == 1)
        return VMCU_RC_ERR_OP_SRC;

    write_word(write_ptr, (0x100 | (rr / 2)) | ((rd / 2) << 4));
    return VMCU_RC_OK;
}

vmcu_rc_t pack_and_assemble_mul(uint16_t* write_ptr, const va_list operands) {

    vmcu_operand_t dest = {

        .type = VMCU_OPTYPE_R,
        .r    = va_arg(operands, vmcu_register_t)
    };

    vmcu_operand_t src = {

        .type = VMCU_OPTYPE_R,
        .r    = va_arg(operands, vmcu_register_t)
    };

    return assemble_mul(write_ptr, &dest, &src);
}

vmcu_rc_t assemble_mul(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src) {

    uint16_t opcode;

    if (dest->r < VMCU_REGISTER_R0 || dest->r > VMCU_REGISTER_R31)
        return VMCU_RC_ERR_OP_DEST;

    if (src->r < VMCU_REGISTER_R0 || src->r > VMCU_REGISTER_R31)
        return VMCU_RC_ERR_OP_SRC;

    opcode = ((0x9c00 | (src->r & 0xf)) | ((src->r & 0x10) << 5));
    opcode |= (dest->r << 4);

    write_word(write_ptr, opcode);
    return VMCU_RC_OK;
}

vmcu_rc_t pack_and_assemble_muls(uint16_t* write_ptr, const va_list operands) {

    vmcu_operand_t dest = {

        .type = VMCU_OPTYPE_R,
        .r    = va_arg(operands, vmcu_register_t)
    };

    vmcu_operand_t src = {

        .type = VMCU_OPTYPE_R,
        .r    = va_arg(operands, vmcu_register_t)
    };

    return assemble_muls(write_ptr, &dest, &src);
}

vmcu_rc_t assemble_muls(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src) {

    if (dest->r < VMCU_REGISTER_R16 || dest->r > VMCU_REGISTER_R31)
        return VMCU_RC_ERR_OP_DEST;

    if (src->r < VMCU_REGISTER_R16 || src->r > VMCU_REGISTER_R31)
        return VMCU_RC_ERR_OP_SRC;

    write_word(write_ptr, ((0x200 | (src->r - 16)) | ((dest->r - 16) << 4)));
    return VMCU_RC_OK;
}
