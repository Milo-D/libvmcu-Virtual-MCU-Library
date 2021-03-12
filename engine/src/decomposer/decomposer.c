/* Instruction Decomposer Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/analyzer/report/plain.h"

// Project Headers (engine utilities)
#include "engine/include/misc/bitmanip.h"

/* Forward Declaration of static Functions */

static void decompose_dw(vmcu_plain_t *p);

/* Forward Declaration of static Members */

static void (*decompose_opcode[VMCU_SET_SIZE]) (vmcu_plain_t *p);

/* --- Extern --- */

int vmcu_decompose_bytes(const uint32_t bytes, vmcu_plain_t *p) {

    if(vmcu_decode_bytes(bytes, p) < 0)
        return -1;

    if(p->exec == false) {

        decompose_dw(p);
        return 0;
    }

    (*decompose_opcode[ p->key ])(p);
    return 0;
}

vmcu_plain_t* vmcu_decompose_ihex(const char *hex_file, int32_t *size) {

    vmcu_plain_t *p;

    if((p = vmcu_decode_ihex(hex_file, size)) == NULL)
        return NULL;

    for(int i = 0; i < *size; i++) {

        vmcu_plain_t *pi = &(p[i]);

        if(pi->exec == false) {

            decompose_dw(pi);
            continue;
        }

        (*decompose_opcode[ pi->key ])(pi);
    }

    return p;
}

/* --- Static --- */

static void decompose_dw(vmcu_plain_t *p) {

    p->src.type   = VMCU_UIMM16;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = p->opcode;
}

static void decompose_nop(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_movw(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 4, 0) * 2;
    p->dest.value = vmcu_extr(p->opcode, 4, 8, 0) * 2;
}

static void decompose_mul(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_muls(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 4, 0) + 16;
    p->dest.value = vmcu_extr(p->opcode, 4, 8, 0) + 16;
}

static void decompose_mulsu(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0) + 16;
    p->dest.value = vmcu_extr(p->opcode, 4, 7, 0) + 16;
}

static void decompose_fmul(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0) + 16;
    p->dest.value = vmcu_extr(p->opcode, 4, 7, 0) + 16;
}

static void decompose_fmuls(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0) + 16;
    p->dest.value = vmcu_extr(p->opcode, 4, 7, 0) + 16;
}

static void decompose_fmulsu(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0) + 16;
    p->dest.value = vmcu_extr(p->opcode, 4, 7, 0) + 16;
}

static void decompose_ldi(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IMM8;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_rjmp(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 0, 12, 0);

    if(((0x01 << 11) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 12) * -1;
}

static void decompose_jmp(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = (opc & 0x1ffff) + ((opc & 0x1f00000) >> 3);
}

static void decompose_ijmp(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_mov(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_dec(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_inc(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_add(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_adc(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_adiw(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_UIMM8;
    p->dest.type  = VMCU_REGISTERPAIR;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 6, 8, 4);
    p->dest.value = (2 * vmcu_extr(opc, 4, 6, 0)) + 24;
}

static void decompose_sub(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_subi(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IMM8;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_sbc(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_sbci(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IMM8;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_sbiw(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_UIMM8;
    p->dest.type  = VMCU_REGISTERPAIR;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 6, 8, 4);
    p->dest.value = (2 * vmcu_extr(opc, 4, 6, 0)) + 24;
}

static void decompose_push(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_pop(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_in(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IODIRECT;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 11, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_out(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_IODIRECT;

    p->src.value  = vmcu_extr(opc, 4, 9, 0);
    p->dest.value = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 11, 4);
}

static void decompose_sbis(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_IODIRECT;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 3, 8, 0);
}

static void decompose_sbic(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_IODIRECT;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 3, 8, 0);
}

static void decompose_sbrc(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_sbrs(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_cpse(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_eor(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ld_x(vmcu_plain_t *p) {

    p->src.type   = VMCU_XPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 26;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_xi(vmcu_plain_t *p) {

    p->src.type   = VMCU_XPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 26;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_dx(vmcu_plain_t *p) {

    p->src.type   = VMCU_XPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 26;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_y(vmcu_plain_t *p) {

    p->src.type   = VMCU_YPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 28;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_yi(vmcu_plain_t *p) {

    p->src.type   = VMCU_YPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 28;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_dy(vmcu_plain_t *p) {

    p->src.type   = VMCU_YPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 28;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ldd_yq(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 3, 0)
                  + vmcu_extr(opc, 10, 12, 3)
                  + vmcu_extr(opc, 13, 14, 5);

    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ldd_zq(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 3, 0)
                  + vmcu_extr(opc, 10, 12, 3)
                  + vmcu_extr(opc, 13, 14, 5);

    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_ld_z(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_zi(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ld_dz(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_st_x(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_XPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 26;
}

static void decompose_st_xi(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_XPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 26;
}

static void decompose_st_dx(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_XPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 26;
}

static void decompose_st_y(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_YPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 28;
}

static void decompose_st_yi(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_YPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 28;
}

static void decompose_st_dy(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_YPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 28;
}

static void decompose_std_yq(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_IMM;

    p->src.value  = vmcu_extr(opc, 4, 9, 0);

    p->dest.value = vmcu_extr(opc, 0, 3, 0)
                  + vmcu_extr(opc, 10, 12, 3)
                  + vmcu_extr(opc, 13, 14, 5);
}

static void decompose_st_z(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_st_zi(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_st_dz(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_std_zq(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_IMM;

    p->src.value  = vmcu_extr(opc, 4, 9, 0);

    p->dest.value = vmcu_extr(opc, 0, 3, 0)
                  + vmcu_extr(opc, 10, 12, 3)
                  + vmcu_extr(opc, 13, 14, 5);
}

static void decompose_sts(vmcu_plain_t *p) {

    const int opc = p->opcode;
    const int tmp = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 11, 4);

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_IMM;

    p->src.value  = vmcu_extr(opc, 4, 8, 0) + 16;

    p->dest.value = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) |
                     ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                     ((0x0f & opc));
}

static void decompose_sts32(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_UIMM16;

    p->src.value  = vmcu_extr(p->opcode, 20, 25, 0);
    p->dest.value = vmcu_extr(p->opcode, 0, 16, 0);
}

static void decompose_lds(vmcu_plain_t *p) {

    const int opc = p->opcode;
    const int tmp = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 11, 4);

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) |
                     ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                     ((0x0f & opc));

    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_lds32(vmcu_plain_t *p) {

    p->src.type   = VMCU_UIMM16;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 16, 0);
    p->dest.value = vmcu_extr(p->opcode, 20, 25, 0);
}

static void decompose_xch(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_brne(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_breq(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brge(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brpl(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brlo(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brlt(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brcc(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brcs(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brvs(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brts(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brtc(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brmi(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brhc(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brhs(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brid(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brie(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_brvc(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 3, 10, 0);

    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

static void decompose_rcall(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 0, 12, 0);

    if(((0x01 << 11) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 12) * -1;
}

static void decompose_ret(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_reti(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_icall(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_call(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = (p->opcode & 0x1ffff)
                  + (p->opcode & 0x1f00000);
}

static void decompose_cp(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_cpi(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_UIMM8;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_cpc(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_lsr(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_asr(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ror(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_swap(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_ori(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_UIMM8;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_or_asm(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_and_asm(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 9, 10, 4);
    p->dest.value = vmcu_extr(opc, 4, 9, 0);
}

static void decompose_andi(vmcu_plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = VMCU_UIMM8;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(opc, 0, 4, 0) + vmcu_extr(opc, 8, 12, 4);
    p->dest.value = vmcu_extr(opc, 4, 8, 0) + 16;
}

static void decompose_las(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_lac(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_lat(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_ZPTR;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

static void decompose_com(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_neg(vmcu_plain_t *p) {

    p->src.type   = VMCU_REGISTER;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_bld(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_bst(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_sbi(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_IODIRECT;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 3, 8, 0);
}

static void decompose_cbi(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_IODIRECT;

    p->src.value  = vmcu_extr(p->opcode, 0, 3, 0);
    p->dest.value = vmcu_extr(p->opcode, 3, 8, 0);
}

static void decompose_lpm(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_lpm_z(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_lpm_zi(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_eicall(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_eijmp(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_elpm(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_elpm_z(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_elpm_zi(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_REGISTER;

    p->src.value  = 30;
    p->dest.value = vmcu_extr(p->opcode, 4, 9, 0);
}

static void decompose_des(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 8, 0);
}

static void decompose_sleep(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_wdr(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_break_asm(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_spm(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_spm_zi(vmcu_plain_t *p) {

    p->src.type   = VMCU_ZPTR;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = 30;
}

static void decompose_ses(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_set(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_sev(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_sez(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_seh(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_sec(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_sei(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_sen(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_cls(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_clt(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_clv(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_clz(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_clh(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_clc(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_cli(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_cln(vmcu_plain_t *p) {

    p->src.type   = VMCU_EMPTY;
    p->dest.type  = VMCU_EMPTY;
}

static void decompose_bclr(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 7, 0);
}

static void decompose_bset(vmcu_plain_t *p) {

    p->src.type   = VMCU_IMM;
    p->dest.type  = VMCU_EMPTY;

    p->src.value  = vmcu_extr(p->opcode, 4, 7, 0);
}

static void (*decompose_opcode[VMCU_SET_SIZE]) (vmcu_plain_t *p) = {

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

