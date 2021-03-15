/* Instruction Decomposer Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/analyzer/report/instr.h"

// Project Headers (engine utilities)
#include "engine/include/misc/bitmanip.h"

/* Forward Declaration of static Functions */

static void decompose_dw(vmcu_instr_t *instr);

/* Forward Declaration of static Members */

static void (*decompose_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr);

/* --- Extern --- */

int vmcu_decompose_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu) {

    if(vmcu_decode_bytes(bytes, instr, mcu) < 0)
        return -1;

    if(instr->exec == false) {

        decompose_dw(instr);
        return 0;
    }

    (*decompose_opcode[ instr->key ])(instr);
    return 0;
}

vmcu_instr_t* vmcu_decompose_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *instr_list;

    if((instr_list = vmcu_decode_ihex(hex_file, size, mcu)) == NULL)
        return NULL;

    for(int32_t i = 0; i < *size; i++) {

        vmcu_instr_t *instr = &(instr_list[i]);

        if(instr->exec == false) {

            decompose_dw(instr);
            continue;
        }

        (*decompose_opcode[ instr->key ])(instr);
    }

    return instr_list;
}

/* --- Static --- */

static void decompose_dw(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_NONE;

    instr->src.type   = VMCU_UIMM16;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = instr->opcode;
}

static void decompose_nop(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_SYS_CTRL;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_movw(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 4, 0) * 2;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 8, 0) * 2;
}

static void decompose_mul(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_muls(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 4, 0) + 16;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 8, 0) + 16;
}

static void decompose_mulsu(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_fmul(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_fmuls(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_fmulsu(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_ldi(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_IMM8;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_rjmp(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 12, 0);

    if(((0x01 << 11) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 12) * -1;
}

static void decompose_jmp(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = (opc & 0x1ffff) + ((opc & 0x1f00000) >> 3);
}

static void decompose_ijmp(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_mov(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_dec(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_inc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_add(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_adc(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_adiw(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_UIMM8;
    instr->dest.type  = VMCU_REGISTERPAIR;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 6, 8, 4);
    instr->dest.value = (2 * vmcu_extr(opc, 4, 6, 0)) + 24;
}

static void decompose_sub(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_subi(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_IMM8;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_sbc(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_sbci(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_IMM8;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_sbiw(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_UIMM8;
    instr->dest.type  = VMCU_REGISTERPAIR;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 6, 8, 4);
    instr->dest.value = (2 * vmcu_extr(opc, 4, 6, 0)) + 24;
}

static void decompose_push(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_pop(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_in(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_IODIRECT;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 11, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_out(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_IODIRECT;

    instr->src.value  = vmcu_extr(opc, 4, 9, 0);
    instr->dest.value = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 11, 4);
}

static void decompose_sbis(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_IODIRECT;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_sbic(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_IODIRECT;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_sbrc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_sbrs(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_cpse(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_eor(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ld_x(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_XPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 26;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_xi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_XPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 26;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_dx(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_XPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 26;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_y(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_YPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 28;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_yi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_YPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 28;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_dy(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_YPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 28;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ldd_yq(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 3, 0)
                      + vmcu_extr(opc, 10, 12, 3)
                      + vmcu_extr(opc, 13, 14, 5);

    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ldd_zq(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 3, 0)
                      + vmcu_extr(opc, 10, 12, 3)
                      + vmcu_extr(opc, 13, 14, 5);

    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ld_z(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_zi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_dz(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_st_x(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_XPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 26;
}

static void decompose_st_xi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_XPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 26;
}

static void decompose_st_dx(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_XPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 26;
}

static void decompose_st_y(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_YPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 28;
}

static void decompose_st_yi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_YPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 28;
}

static void decompose_st_dy(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_YPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 28;
}

static void decompose_std_yq(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_IMM;

    instr->src.value  = vmcu_extr(opc, 4, 9, 0);

    instr->dest.value = vmcu_extr(opc, 0, 3, 0)
                      + vmcu_extr(opc, 10, 12, 3)
                      + vmcu_extr(opc, 13, 14, 5);
}

static void decompose_st_z(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_st_zi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_st_dz(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_std_zq(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_IMM;

    instr->src.value  = vmcu_extr(opc, 4, 9, 0);

    instr->dest.value = vmcu_extr(opc, 0, 3, 0)
                      + vmcu_extr(opc, 10, 12, 3)
                      + vmcu_extr(opc, 13, 14, 5);
}

static void decompose_sts(vmcu_instr_t *instr) {

    const int opc = instr->opcode;
    const int tmp = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 11, 4);

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_IMM;

    instr->src.value  = vmcu_extr(opc, 4, 8, 0) + 16;

    instr->dest.value = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) |
                         ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                         ((0x0f & opc));
}

static void decompose_sts32(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_UIMM16;

    instr->src.value  = vmcu_extr(instr->opcode, 20, 25, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 0, 16, 0);
}

static void decompose_lds(vmcu_instr_t *instr) {

    const int opc = instr->opcode;
    const int tmp = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 11, 4);

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) |
                         ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                         ((0x0f & opc));

    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_lds32(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_UIMM16;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 16, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 20, 25, 0);
}

static void decompose_xch(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_brne(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_breq(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brge(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brpl(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brlo(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brlt(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brcc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brcs(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brvs(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brts(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brtc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brmi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brhc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brhs(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brid(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brie(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_brvc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 7) * -1;
}

static void decompose_rcall(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 12, 0);

    if(((0x01 << 11) & instr->src.value) != 0x00)
        instr->src.value = comp(instr->src.value, 12) * -1;
}

static void decompose_ret(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_reti(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_icall(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_call(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = (instr->opcode & 0x1ffff)
                      + (instr->opcode & 0x1f00000);
}

static void decompose_cp(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_cpi(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_UIMM8;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_cpc(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_lsr(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_asr(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ror(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_swap(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ori(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_UIMM8;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_or_asm(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_and_asm(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_andi(vmcu_instr_t *instr) {

    const int opc = instr->opcode;

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_UIMM8;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_las(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_lac(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_lat(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_ZPTR;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
    instr->dest.value = 30;
}

static void decompose_com(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_neg(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_REGISTER;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_bld(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_bst(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_sbi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_IODIRECT;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_cbi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_IODIRECT;

    instr->src.value  = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.value = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_lpm(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_lpm_z(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_lpm_zi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_eicall(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_eijmp(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_elpm(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_elpm_z(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_elpm_zi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_FLOW;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_REGISTER;

    instr->src.value  = 30;
    instr->dest.value = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_des(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_MATH_LOGIC;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 8, 0);
}

static void decompose_sleep(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_SYS_CTRL;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_wdr(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_SYS_CTRL;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_break_asm(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_SYS_CTRL;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_spm(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_spm_zi(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_TRANSFER;

    instr->src.type   = VMCU_ZPTR;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = 30;
}

static void decompose_ses(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_set(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_sev(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_sez(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_seh(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_sec(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_sei(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_sen(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_cls(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_clt(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_clv(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_clz(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_clh(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_clc(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_cli(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_cln(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_EMPTY;
    instr->dest.type  = VMCU_EMPTY;
}

static void decompose_bclr(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 7, 0);
}

static void decompose_bset(vmcu_instr_t *instr) {

    instr->group      = VMCU_GROUP_BIT;

    instr->src.type   = VMCU_IMM;
    instr->dest.type  = VMCU_EMPTY;

    instr->src.value  = vmcu_extr(instr->opcode, 4, 7, 0);
}

static void (*decompose_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr) = {

    decompose_nop,
    decompose_movw,
    decompose_mul,
    decompose_muls,
    decompose_mulsu,
    decompose_fmul,
    decompose_fmuls,
    decompose_fmulsu,
    decompose_ldi,
    decompose_rjmp,
    decompose_jmp,
    decompose_ijmp,
    decompose_mov,
    decompose_dec,
    decompose_inc,
    decompose_add,
    decompose_adc,
    decompose_adiw,
    decompose_sub,
    decompose_subi,
    decompose_sbc,
    decompose_sbci,
    decompose_sbiw,
    decompose_push,
    decompose_pop,
    decompose_in,
    decompose_out,
    decompose_sbis,
    decompose_sbic,
    decompose_sbrc,
    decompose_sbrs,
    decompose_cpse,
    decompose_eor,
    decompose_ld_x,
    decompose_ld_xi,
    decompose_ld_dx,
    decompose_ld_y,
    decompose_ld_yi,
    decompose_ld_dy,
    decompose_ldd_yq,
    decompose_ldd_zq,
    decompose_ld_z,
    decompose_ld_zi,
    decompose_ld_dz,
    decompose_st_x,
    decompose_st_xi,
    decompose_st_dx,
    decompose_st_y,
    decompose_st_yi,
    decompose_st_dy,
    decompose_std_yq,
    decompose_st_z,
    decompose_st_zi,
    decompose_st_dz,
    decompose_std_zq,
    decompose_sts,
    decompose_sts32,
    decompose_lds,
    decompose_lds32,
    decompose_xch,
    decompose_brne,
    decompose_breq,
    decompose_brge,
    decompose_brpl,
    decompose_brlo,
    decompose_brlt,
    decompose_brcc,
    decompose_brcs,
    decompose_brvs,
    decompose_brts,
    decompose_brtc,
    decompose_brmi,
    decompose_brhc,
    decompose_brhs,
    decompose_brid,
    decompose_brie,
    decompose_brvc,
    decompose_rcall,
    decompose_ret,
    decompose_reti,
    decompose_icall,
    decompose_call,
    decompose_cp,
    decompose_cpi,
    decompose_cpc,
    decompose_lsr,
    decompose_asr,
    decompose_ror,
    decompose_swap,
    decompose_ori,
    decompose_or_asm,
    decompose_and_asm,
    decompose_andi,
    decompose_las,
    decompose_lac,
    decompose_lat,
    decompose_com,
    decompose_neg,
    decompose_bld,
    decompose_bst,
    decompose_sbi,
    decompose_cbi,
    decompose_lpm,
    decompose_lpm_z,
    decompose_lpm_zi,
    decompose_eicall,
    decompose_eijmp,
    decompose_elpm,
    decompose_elpm_z,
    decompose_elpm_zi,
    decompose_des,
    decompose_sleep,
    decompose_wdr,
    decompose_break_asm,
    decompose_spm,
    decompose_spm_zi,
    decompose_ses,
    decompose_set,
    decompose_sev,
    decompose_sez,
    decompose_seh,
    decompose_sec,
    decompose_sei,
    decompose_sen,
    decompose_cls,
    decompose_clt,
    decompose_clv,
    decompose_clz,
    decompose_clh,
    decompose_clc,
    decompose_cli,
    decompose_cln,
    decompose_bclr,
    decompose_bset
};

