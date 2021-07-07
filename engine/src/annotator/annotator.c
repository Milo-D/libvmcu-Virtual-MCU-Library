/* Instruction Annotator Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/annotator/annotator.h"
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/analyzer/report/instr.h"

/* Forward Declaration of static Functions */

static void annotate_dw(vmcu_instr_t *instr);

/* Forward Declaration of static Members */

static void (*annotate_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr);

/* --- Extern --- */

int vmcu_annotate_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu) {

    if(vmcu_decode_bytes(bytes, instr, mcu) < 0)
        return -1;

    if(instr->exec == false) {

        annotate_dw(instr);
        return 0;
    }

    (*annotate_opcode[ instr->key ])(instr);
    return 0;
}

vmcu_instr_t* vmcu_annotate_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *instr_list;

    if((instr_list = vmcu_decode_ihex(hex_file, size, mcu)) == NULL)
        return NULL;

    for(int32_t i = 0; i < *size; i++) {

        vmcu_instr_t *instr = &(instr_list[i]);

        if(instr->exec == false) {

            annotate_dw(instr);
            continue;
        }

        (*annotate_opcode[ instr->key ])(instr);
    }

    return instr_list;
}

/* --- Static --- */

static void annotate_dw(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_NONE;
}

static void annotate_nop(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_SYS_CTRL;
}

static void annotate_movw(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->reads.registers  = true;
}

static void annotate_mul(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_muls(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_mulsu(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_fmul(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_fmuls(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_fmulsu(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_ldi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;
    instr->writes.registers = true;
}

static void annotate_rjmp(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;
    instr->reads.pc         = true;
}

static void annotate_jmp(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;
    instr->writes.pc        = true;
}

static void annotate_ijmp(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;
    instr->reads.registers  = true;
}

static void annotate_mov(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->reads.registers  = true;
}

static void annotate_dec(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_inc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_add(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_adc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
    instr->reads.c_flag     = true;
}

static void annotate_adiw(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_sub(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_subi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_sbc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
    instr->reads.c_flag     = true;
}

static void annotate_sbci(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
    instr->reads.c_flag     = true;
}

static void annotate_sbiw(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_push(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.stack     = true;
    instr->writes.sp        = true;

    instr->reads.registers  = true;
    instr->reads.sp         = true;
}

static void annotate_pop(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.sp        = true;

    instr->reads.stack      = true;
    instr->reads.sp         = true;
}

static void annotate_in(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->reads.io         = true;
}

static void annotate_out(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.io        = true;
    instr->reads.registers  = true;
}

static void annotate_sbis(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.io         = true;
    instr->reads.pc         = true;
}

static void annotate_sbic(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.io         = true;
    instr->reads.pc         = true;
}

static void annotate_sbrc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.registers  = true;
    instr->reads.pc         = true;
}

static void annotate_sbrs(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.registers  = true;
    instr->reads.pc         = true;
}

static void annotate_cpse(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.registers  = true;
    instr->reads.pc         = true;
}

static void annotate_eor(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_ld_x(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_xi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_dx(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_y(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_yi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_dy(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ldd_yq(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ldd_zq(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_z(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_zi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_ld_dz(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_st_x(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_st_xi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
}

static void annotate_st_dx(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
}

static void annotate_st_y(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_st_yi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
}

static void annotate_st_dy(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
}

static void annotate_std_yq(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_st_z(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_st_zi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
}

static void annotate_st_dz(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
}

static void annotate_std_zq(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_sts(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_sts32(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.ds        = true;
    instr->reads.registers  = true;
}

static void annotate_lds(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->reads.ds         = true;
}

static void annotate_lds32(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->reads.ds         = true;
}

static void annotate_xch(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_brne(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.z_flag     = true;
}

static void annotate_breq(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.z_flag     = true;
}

static void annotate_brge(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.n_flag     = true;
    instr->reads.v_flag     = true;
}

static void annotate_brpl(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.n_flag     = true;
}

static void annotate_brlo(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.c_flag     = true;
}

static void annotate_brlt(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.n_flag     = true;
    instr->reads.v_flag     = true;
}

static void annotate_brcc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.z_flag     = true;
}

static void annotate_brvs(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.v_flag     = true;
}

static void annotate_brts(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.t_flag     = true;
}

static void annotate_brtc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.t_flag     = true;
}

static void annotate_brmi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.n_flag     = true;
}

static void annotate_brhc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.h_flag     = true;
}

static void annotate_brhs(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.h_flag     = true;
}

static void annotate_brid(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.i_flag     = true;
}

static void annotate_brie(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.i_flag     = true;
}

static void annotate_brvc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.pc         = true;
    instr->reads.v_flag     = true;
}

static void annotate_rcall(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.stack     = true;
    instr->writes.pc        = true;
    instr->writes.sp        = true;

    instr->reads.pc         = true;
    instr->reads.sp         = true;
}

static void annotate_ret(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;
    instr->writes.sp        = true;

    instr->reads.stack      = true;
    instr->reads.sp         = true;
}

static void annotate_reti(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;
    instr->writes.sp        = true;
    instr->writes.i_flag    = true;

    instr->reads.stack      = true;
    instr->reads.sp         = true;
}

static void annotate_icall(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.stack     = true;
    instr->writes.pc        = true;
    instr->writes.sp        = true;

    instr->reads.registers  = true;
    instr->reads.pc         = true;
    instr->reads.sp         = true;
}

static void annotate_call(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.stack     = true;
    instr->writes.pc        = true;
    instr->writes.sp        = true;

    instr->reads.pc         = true;
    instr->reads.sp         = true;
}

static void annotate_cp(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_cpi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_cpc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
    instr->reads.c_flag     = true;
}

static void annotate_lsr(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_asr(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_ror(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
    instr->reads.z_flag     = true;
}

static void annotate_swap(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;

    instr->writes.registers = true;
    instr->reads.registers  = true;
}

static void annotate_ori(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_or_asm(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_and_asm(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_andi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_las(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_lac(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_lat(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.ds        = true;

    instr->reads.registers  = true;
    instr->reads.ds         = true;
}

static void annotate_com(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_neg(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->writes.n_flag    = true;
    instr->writes.v_flag    = true;
    instr->writes.s_flag    = true;
    instr->writes.h_flag    = true;

    instr->reads.registers  = true;
}

static void annotate_bld(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;

    instr->writes.registers = true;
    instr->reads.t_flag     = true;
}

static void annotate_bst(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;

    instr->writes.t_flag    = true;
    instr->reads.registers  = true;
}

static void annotate_sbi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.io        = true;
}

static void annotate_cbi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.io        = true;
}

static void annotate_lpm(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.flash      = true;
}

static void annotate_lpm_z(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.flash      = true;
}

static void annotate_lpm_zi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.flash      = true;
}

static void annotate_eicall(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.stack     = true;
    instr->writes.pc        = true;
    instr->writes.sp        = true;

    instr->reads.registers  = true;
    instr->reads.io         = true;
    instr->reads.pc         = true;
    instr->reads.sp         = true;
}

static void annotate_eijmp(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_FLOW;

    instr->writes.pc        = true;

    instr->reads.registers  = true;
    instr->reads.io         = true;
}

static void annotate_elpm(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.flash      = true;
    instr->reads.io         = true;
}

static void annotate_elpm_z(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.flash      = true;
    instr->reads.io         = true;
}

static void annotate_elpm_zi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.flash      = true;
    instr->reads.io         = true;
}

static void annotate_des(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_MATH_LOGIC;

    instr->writes.registers = true;

    instr->reads.registers  = true;
    instr->reads.h_flag     = true;
}

static void annotate_sleep(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_SYS_CTRL;
}

static void annotate_wdr(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_SYS_CTRL;
}

static void annotate_break_asm(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_SYS_CTRL;
}

static void annotate_spm(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.flash     = true;

    instr->reads.registers  = true;
    instr->reads.io         = true;
}

static void annotate_spm_zi(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_TRANSFER;

    instr->writes.registers = true;
    instr->writes.flash     = true;

    instr->reads.registers  = true;
    instr->reads.io         = true;
}

static void annotate_ses(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.s_flag    = true;
}

static void annotate_set(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.t_flag    = true;
}

static void annotate_sev(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.v_flag    = true;
}

static void annotate_sez(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.z_flag    = true;
}

static void annotate_seh(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.h_flag    = true;
}

static void annotate_sec(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.c_flag    = true;
}

static void annotate_sei(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.i_flag    = true;
}

static void annotate_sen(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.n_flag    = true;
}

static void annotate_cls(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.s_flag    = true;
}

static void annotate_clt(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.t_flag    = true;
}

static void annotate_clv(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.v_flag    = true;
}

static void annotate_clz(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.z_flag    = true;
}

static void annotate_clh(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.h_flag    = true;
}

static void annotate_clc(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.c_flag    = true;
}

static void annotate_cli(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.i_flag    = true;
}

static void annotate_cln(vmcu_instr_t *instr) {

    instr->group            = VMCU_GROUP_BIT;
    instr->writes.n_flag    = true;
}

static void (*annotate_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr) = {

    annotate_nop,
    annotate_movw,
    annotate_mul,
    annotate_muls,
    annotate_mulsu,
    annotate_fmul,
    annotate_fmuls,
    annotate_fmulsu,
    annotate_ldi,
    annotate_rjmp,
    annotate_jmp,
    annotate_ijmp,
    annotate_mov,
    annotate_dec,
    annotate_inc,
    annotate_add,
    annotate_adc,
    annotate_adiw,
    annotate_sub,
    annotate_subi,
    annotate_sbc,
    annotate_sbci,
    annotate_sbiw,
    annotate_push,
    annotate_pop,
    annotate_in,
    annotate_out,
    annotate_sbis,
    annotate_sbic,
    annotate_sbrc,
    annotate_sbrs,
    annotate_cpse,
    annotate_eor,
    annotate_ld_x,
    annotate_ld_xi,
    annotate_ld_dx,
    annotate_ld_y,
    annotate_ld_yi,
    annotate_ld_dy,
    annotate_ldd_yq,
    annotate_ldd_zq,
    annotate_ld_z,
    annotate_ld_zi,
    annotate_ld_dz,
    annotate_st_x,
    annotate_st_xi,
    annotate_st_dx,
    annotate_st_y,
    annotate_st_yi,
    annotate_st_dy,
    annotate_std_yq,
    annotate_st_z,
    annotate_st_zi,
    annotate_st_dz,
    annotate_std_zq,
    annotate_sts,
    annotate_sts32,
    annotate_lds,
    annotate_lds32,
    annotate_xch,
    annotate_brne,
    annotate_breq,
    annotate_brge,
    annotate_brpl,
    annotate_brlo,
    annotate_brlt,
    annotate_brcc,
    annotate_brvs,
    annotate_brts,
    annotate_brtc,
    annotate_brmi,
    annotate_brhc,
    annotate_brhs,
    annotate_brid,
    annotate_brie,
    annotate_brvc,
    annotate_rcall,
    annotate_ret,
    annotate_reti,
    annotate_icall,
    annotate_call,
    annotate_cp,
    annotate_cpi,
    annotate_cpc,
    annotate_lsr,
    annotate_asr,
    annotate_ror,
    annotate_swap,
    annotate_ori,
    annotate_or_asm,
    annotate_and_asm,
    annotate_andi,
    annotate_las,
    annotate_lac,
    annotate_lat,
    annotate_com,
    annotate_neg,
    annotate_bld,
    annotate_bst,
    annotate_sbi,
    annotate_cbi,
    annotate_lpm,
    annotate_lpm_z,
    annotate_lpm_zi,
    annotate_eicall,
    annotate_eijmp,
    annotate_elpm,
    annotate_elpm_z,
    annotate_elpm_zi,
    annotate_des,
    annotate_sleep,
    annotate_wdr,
    annotate_break_asm,
    annotate_spm,
    annotate_spm_zi,
    annotate_ses,
    annotate_set,
    annotate_sev,
    annotate_sez,
    annotate_seh,
    annotate_sec,
    annotate_sei,
    annotate_sen,
    annotate_cls,
    annotate_clt,
    annotate_clv,
    annotate_clz,
    annotate_clh,
    annotate_clc,
    annotate_cli,
    annotate_cln
};


