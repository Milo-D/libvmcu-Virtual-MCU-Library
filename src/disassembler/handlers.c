#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "handlers.h"
#include <lib/word.h>
#include <lib/instr.h>

/* --- Internal helper functions --- */

static inline int16_t sext_s12(int16_t s12) {

    /* sign extend from 12-bit to 16-bit */
    return (s12 & 0x0800) ? s12 |= 0xf000 : s12;
}

static void init_operand_regpair(vmcu_operand_t* op, const vmcu_register_t rh, const vmcu_register_t rl) {

    op->type    = VMCU_OPTYPE_RP;
    op->rp.high = rh;
    op->rp.low  = rl;

    snprintf(op->str, sizeof(op->str), "r%d:r%d", rh, rl);
}

static void init_operand_reg(vmcu_operand_t* op, const vmcu_register_t reg) {

    op->type = VMCU_OPTYPE_R;
    op->r    = reg;

    snprintf(op->str, sizeof(op->str), "r%d", reg);
}

static void init_operand_uimm8(vmcu_operand_t* op, const vmcu_optype_t type, const uint8_t imm) {

    op->type  = type;
    op->uimm8 = imm;

    snprintf(op->str, sizeof(op->str), "0x%02" PRIx8, imm);
}

static void init_operand_imm16(vmcu_operand_t* op, const vmcu_optype_t type, const int16_t imm) {

    op->type  = type;
    op->imm16 = imm;

    snprintf(op->str, sizeof(op->str), "%" PRId16, imm);
}

static void init_operand_uimm32(vmcu_operand_t* op, const uint32_t imm) {

    op->type   = VMCU_OPTYPE_P22;
    op->uimm32 = imm;

    snprintf(op->str, sizeof(op->str), "0x%" PRIx32, imm);
}

/* --- Exposed --- */

void disassemble_invalid_opcode(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    instr->id         = VMCU_IID_ENUM_START;
    instr->group      = VMCU_GROUP_ENUM_START;
    instr->addr       = w0->addr;
    instr->n_words    = 1;
    instr->words[0]   = w0->raw;
    instr->n_operands = 0;

    strcpy(instr->str, ".dw");
}

void disassemble_nop(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    instr->id         = VMCU_IID_NOP;
    instr->group      = VMCU_GROUP_SYS_CTRL;
    instr->addr       = w0->addr;
    instr->n_words    = 1;
    instr->words[0]   = w0->raw;
    instr->n_operands = 0;

    strcpy(instr->str, "nop");
}

void disassemble_movw(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rl;

    instr->id               = VMCU_IID_MOVW;
    instr->group            = VMCU_GROUP_TRANSFER;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "movw");

    rl = ((w0->raw & 0xf0) >> 4) * 2;
    init_operand_regpair(&instr->operands[0], rl + 1, rl);

    rl = (w0->raw & 0x0f) * 2;
    init_operand_regpair(&instr->operands[1], rl + 1, rl);
}

void disassemble_mul(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd, rr;

    instr->id               = VMCU_IID_MUL;
    instr->group            = VMCU_GROUP_MATH_LOGIC;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "mul");

    rd = ((w0->raw & 0x1f0) >> 4);
    init_operand_reg(&instr->operands[0], rd);

    rr = (((w0->raw & 0x200) >> 5) + (w0->raw & 0xf));
    init_operand_reg(&instr->operands[1], rr);
}

void disassemble_muls(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd, rr;

    instr->id               = VMCU_IID_MULS;
    instr->group            = VMCU_GROUP_MATH_LOGIC;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "muls");

    rd = ((w0->raw & 0xf0) >> 4) + 16;
    init_operand_reg(&instr->operands[0], rd);

    rr = (w0->raw & 0xf) + 16;
    init_operand_reg(&instr->operands[1], rr);
}

void disassemble_mulsu(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd, rr;

    instr->id               = VMCU_IID_MULSU;
    instr->group            = VMCU_GROUP_MATH_LOGIC;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "mulsu");

    rd = ((w0->raw & 0x70) >> 4) + 16;
    init_operand_reg(&instr->operands[0], rd);

    rr = (w0->raw & 0x7) + 16;
    init_operand_reg(&instr->operands[1], rr);
}

void disassemble_fmul(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd, rr;

    instr->id               = VMCU_IID_FMUL;
    instr->group            = VMCU_GROUP_MATH_LOGIC;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "fmul");

    rd = ((w0->raw & 0x70) >> 4) + 16;
    init_operand_reg(&instr->operands[0], rd);

    rr = (w0->raw & 0x7) + 16;
    init_operand_reg(&instr->operands[1], rr);
}

void disassemble_fmuls(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd, rr;

    instr->id               = VMCU_IID_FMULS;
    instr->group            = VMCU_GROUP_MATH_LOGIC;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "fmuls");

    rd = ((w0->raw & 0x70) >> 4) + 16;
    init_operand_reg(&instr->operands[0], rd);

    rr = (w0->raw & 0x7) + 16;
    init_operand_reg(&instr->operands[1], rr);
}

void disassemble_fmulsu(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd, rr;

    instr->id               = VMCU_IID_FMULSU;
    instr->group            = VMCU_GROUP_MATH_LOGIC;
    instr->reads.registers  = true;
    instr->writes.registers = true;
    instr->writes.c_flag    = true;
    instr->writes.z_flag    = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "fmulsu");

    rd = ((w0->raw & 0x70) >> 4) + 16;
    init_operand_reg(&instr->operands[0], rd);

    rr = (w0->raw & 0x7) + 16;
    init_operand_reg(&instr->operands[1], rr);
}

void disassemble_ldi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    vmcu_register_t rd;
    uint8_t         k;

    instr->id               = VMCU_IID_LDI;
    instr->group            = VMCU_GROUP_TRANSFER;
    instr->writes.registers = true;
    instr->addr             = w0->addr;
    instr->n_words          = 1;
    instr->words[0]         = w0->raw;
    instr->n_operands       = 2;

    strcpy(instr->str, "ldi");

    rd = ((w0->raw & 0xf0) >> 4) + 16;
    init_operand_reg(&instr->operands[0], rd);

    k = ((w0->raw & 0xf00) >> 4) + (w0->raw & 0xf);
    init_operand_uimm8(&instr->operands[1], VMCU_OPTYPE_K8, k);
}

void disassemble_rjmp(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    int16_t s12;

    instr->id         = VMCU_IID_RJMP;
    instr->group      = VMCU_GROUP_FLOW;
    instr->writes.pc  = true;
    instr->addr       = w0->addr;
    instr->n_words    = 1;
    instr->words[0]   = w0->raw;
    instr->n_operands = 1;

    strcpy(instr->str, "rjmp");

    s12 = sext_s12(w0->raw & 0x0fff);
    init_operand_imm16(&instr->operands[0], VMCU_OPTYPE_S12, s12);
}

void disassemble_jmp(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {

    uint32_t p;

    instr->id         = VMCU_IID_JMP;
    instr->group      = VMCU_GROUP_FLOW;
    instr->writes.pc  = true;
    instr->addr       = w0->addr;
    instr->n_words    = 2;
    instr->words[0]   = w0->raw;
    instr->words[1]   = w1->raw;
    instr->n_operands = 1;

    strcpy(instr->str, "jmp");

    p = ((((uint32_t) (((w0->raw & 0x1f0) >> 3) | (w0->raw & 1))) << 16) | w1->raw);
    init_operand_uimm32(&instr->operands[0], p);
}

void disassemble_ijmp(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_mov(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_dec(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_inc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_add(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_adc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_adiw(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sub(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_subi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbci(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbiw(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_push(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_pop(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_in(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_out(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbis(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbic(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbrc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbrs(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cpse(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_eor(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ldx(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ldxi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lddx(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ldy(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ldyi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lddy(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lddyq(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lddzq(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ldz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ldzi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lddz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stx(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stxi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stdx(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sty(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_styi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stdy(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stdyq(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stzi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stdz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_stdzq(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sts(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sts32(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lds(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lds32(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_xch(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brne(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_breq(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brge(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brpl(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brlo(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brlt(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brcc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brvs(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brts(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brtc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brmi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brhc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brhs(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brid(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brie(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_brvc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_rcall(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ret(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_reti(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_icall(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_call(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cp(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cpi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cpc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lsr(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_asr(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ror(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_swap(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ori(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_or(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_and(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_andi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_las(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lac(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lat(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_com(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_neg(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_bld(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_bst(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sbi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cbi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lpm(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lpmz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_lpmzi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_eicall(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_eijmp(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_elpm(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_elpmz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_elpmzi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_des(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sleep(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_wdr(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_break(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_spm(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_spmzi(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_ses(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_set(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sev(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sez(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_seh(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sec(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sei(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_sen(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cls(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_clt(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_clv(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_clz(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_clh(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_clc(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cli(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}

void disassemble_cln(vmcu_instr_t* instr, const vmcu_word_t* w0, const vmcu_word_t* w1) {
}
