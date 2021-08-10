/* Instruction Decomposer Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/annotator/annotator.h"
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

    if(vmcu_annotate_bytes(bytes, instr, mcu) < 0)
        return -1;

    if(instr->exec == false) {

        decompose_dw(instr);
        return 0;
    }

    (*decompose_opcode[ instr->key ])(instr);
    return 0;
}

vmcu_instr_t* vmcu_decompose_ihex(const char *hex_file, uint32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *instr_list;

    if((instr_list = vmcu_annotate_ihex(hex_file, size, mcu)) == NULL)
        return NULL;

    for(uint32_t i = 0; i < *size; i++) {

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

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_nop(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_movw(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_RP;
    instr->dest.type    = VMCU_OPTYPE_RP;

    instr->src.rp.low   = vmcu_extr(instr->opcode, 0, 4, 0) * 2;
    instr->src.rp.high  = instr->src.rp.low + 1;

    instr->dest.rp.low  = vmcu_extr(instr->opcode, 4, 8, 0) * 2;
    instr->dest.rp.high = instr->dest.rp.low + 1;
}

static void decompose_mul(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_muls(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(instr->opcode, 0, 4, 0) + 16;
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 8, 0) + 16;
}

static void decompose_mulsu(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_fmul(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_fmuls(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_fmulsu(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(instr->opcode, 0, 3, 0) + 16;
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 7, 0) + 16;
}

static void decompose_ldi(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K8;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_rjmp(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S12;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 0, 12, 0);

    if(((0x01 << 11) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 12) * -1;
}

static void decompose_jmp(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_P22;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.p        = (opc & 0x1ffff) + ((opc & 0x1f00000) >> 3);
}

static void decompose_ijmp(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_mov(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_dec(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_inc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_add(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_adc(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_adiw(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K6;
    instr->dest.type    = VMCU_OPTYPE_RP;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 6, 8, 4);

    instr->dest.rp.low  = (2 * vmcu_extr(opc, 4, 6, 0)) + 24;
    instr->dest.rp.high = instr->dest.rp.low + 1;
}

static void decompose_sub(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_subi(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K8;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_sbc(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_sbci(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K8;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_sbiw(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K6;
    instr->dest.type    = VMCU_OPTYPE_RP;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 6, 8, 4);

    instr->dest.rp.low  = (2 * vmcu_extr(opc, 4, 6, 0)) + 24;
    instr->dest.rp.high = instr->dest.rp.low + 1;
}

static void decompose_push(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_pop(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_in(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_IO6;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.io       = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 11, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_out(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_IO6;

    instr->src.r        = vmcu_extr(opc, 4, 9, 0);
    instr->dest.io      = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 11, 4);
}

static void decompose_sbis(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_IO5;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.io      = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_sbic(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_IO5;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.io      = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_sbrc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_sbrs(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_cpse(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_eor(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ld_x(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_X;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R26;
    instr->src.rp.high  = VMCU_REGISTER_R27;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_xi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_X;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R26;
    instr->src.rp.high  = VMCU_REGISTER_R27;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_dx(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_X;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R26;
    instr->src.rp.high  = VMCU_REGISTER_R27;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_y(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Y;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R28;
    instr->src.rp.high  = VMCU_REGISTER_R29;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_yi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Y;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R28;
    instr->src.rp.high  = VMCU_REGISTER_R29;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_dy(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Y;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R28;
    instr->src.rp.high  = VMCU_REGISTER_R29;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ldd_yq(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K6;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 3, 0)
                        + vmcu_extr(opc, 10, 12, 3)
                        + vmcu_extr(opc, 13, 14, 5);

    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ldd_zq(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K6;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 3, 0)
                        + vmcu_extr(opc, 10, 12, 3)
                        + vmcu_extr(opc, 13, 14, 5);

    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ld_z(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_zi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ld_dz(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_st_x(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_X;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R26;
    instr->dest.rp.high = VMCU_REGISTER_R27;
}

static void decompose_st_xi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_X;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R26;
    instr->dest.rp.high = VMCU_REGISTER_R27;
}

static void decompose_st_dx(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_X;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R26;
    instr->dest.rp.high = VMCU_REGISTER_R27;
}

static void decompose_st_y(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Y;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R28;
    instr->dest.rp.high = VMCU_REGISTER_R29;
}

static void decompose_st_yi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Y;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R28;
    instr->dest.rp.high = VMCU_REGISTER_R29;
}

static void decompose_st_dy(vmcu_instr_t *instr) {

    instr->src.type   = VMCU_OPTYPE_R;
    instr->dest.type  = VMCU_OPTYPE_Y;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R28;
    instr->dest.rp.high = VMCU_REGISTER_R29;
}

static void decompose_std_yq(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_K6;

    instr->src.r        = vmcu_extr(opc, 4, 9, 0);

    instr->dest.k       = vmcu_extr(opc, 0, 3, 0)
                        + vmcu_extr(opc, 10, 12, 3)
                        + vmcu_extr(opc, 13, 14, 5);
}

static void decompose_st_z(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_st_zi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_st_dz(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_std_zq(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_K6;

    instr->src.r        = vmcu_extr(opc, 4, 9, 0);

    instr->dest.k       = vmcu_extr(opc, 0, 3, 0)
                        + vmcu_extr(opc, 10, 12, 3)
                        + vmcu_extr(opc, 13, 14, 5);
}

static void decompose_sts(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;
    const uint16_t tmp  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 11, 4);

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_D7;

    instr->src.r        = vmcu_extr(opc, 4, 8, 0) + 16;

    instr->dest.d       = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) |
                           ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                           ((0x0f & opc));
}

static void decompose_sts32(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_D16;

    instr->src.r        = vmcu_extr(instr->opcode, 20, 25, 0);
    instr->dest.d       = vmcu_extr(instr->opcode, 0, 16, 0);
}

static void decompose_lds(vmcu_instr_t *instr) {

    const uint32_t opc = instr->opcode;
    const uint32_t tmp = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 11, 4);

    instr->src.type     = VMCU_OPTYPE_D7;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.d        = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) |
                           ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                           ((0x0f & opc));

    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_lds32(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_D16;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.d        = vmcu_extr(instr->opcode, 0, 16, 0);
    instr->dest.r       = vmcu_extr(instr->opcode, 20, 25, 0);
}

static void decompose_xch(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_brne(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_breq(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brge(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brpl(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brlo(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brlt(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brcc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brvs(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brts(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brtc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brmi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brhc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brhs(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brid(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brie(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_brvc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S7;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 3, 10, 0);

    if(((0x01 << 6) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 7) * -1;
}

static void decompose_rcall(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_S12;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.s        = vmcu_extr(instr->opcode, 0, 12, 0);

    if(((0x01 << 11) & instr->src.s) != 0x00)
        instr->src.s = comp(instr->src.s, 12) * -1;
}

static void decompose_ret(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_reti(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_icall(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_call(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_P22;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.p        = (instr->opcode & 0x1ffff)
                        + (instr->opcode & 0x1f00000);
}

static void decompose_cp(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_cpi(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K8;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_cpc(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_lsr(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_asr(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ror(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_swap(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_ori(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K8;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_or_asm(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_and_asm(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.r        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_andi(vmcu_instr_t *instr) {

    const uint32_t opc  = instr->opcode;

    instr->src.type     = VMCU_OPTYPE_K8;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.k        = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    instr->dest.r       = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_las(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_lac(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_lat(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_Z;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);

    instr->dest.rp.low  = VMCU_REGISTER_R30;
    instr->dest.rp.high = VMCU_REGISTER_R31;
}

static void decompose_com(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_neg(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_R;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.r        = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_bld(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_bst(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_sbi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_IO5;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.io      = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_cbi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_B;
    instr->dest.type    = VMCU_OPTYPE_IO5;

    instr->src.b        = vmcu_extr(instr->opcode, 0, 3, 0);
    instr->dest.io      = vmcu_extr(instr->opcode, 3, 8, 0);
}

static void decompose_lpm(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_lpm_z(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_lpm_zi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_eicall(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_eijmp(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_elpm(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_elpm_z(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_elpm_zi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_R;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;

    instr->dest.r       = vmcu_extr(instr->opcode, 4, 9, 0);
}

static void decompose_des(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_K4;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.k        = vmcu_extr(instr->opcode, 4, 8, 0);
}

static void decompose_sleep(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_wdr(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_break_asm(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_spm(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_spm_zi(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_Z;
    instr->dest.type    = VMCU_OPTYPE_NONE;

    instr->src.rp.low   = VMCU_REGISTER_R30;
    instr->src.rp.high  = VMCU_REGISTER_R31;
}

static void decompose_ses(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_set(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_sev(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_sez(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_seh(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_sec(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_sei(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_sen(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_cls(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_clt(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_clv(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_clz(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_clh(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_clc(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_cli(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
}

static void decompose_cln(vmcu_instr_t *instr) {

    instr->src.type     = VMCU_OPTYPE_NONE;
    instr->dest.type    = VMCU_OPTYPE_NONE;
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
    decompose_cln
};

