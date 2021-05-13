/* Decoder Integration Test Implementation */

// C Headers
#include <stdio.h>
#include <assert.h>

// libvmcu Headers
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

// Test Headers
#include "decoder_test.h"

#define DFL   "\033[0m"
#define GREEN "\033[32;1m"

#define PASSED printf("%s PASSED %s\n", GREEN, DFL)

#define start(fn, mcu) printf("    |\n");    \
                       printf("    |---- "); \
                       fn(mcu)
/*
 * ldd shadows lds (16-bit)
 * std shadows sts (16-bit)
 *
 * todo: replace brcc with brsh
 *
 * todo: swap ld_zq in opcode table
 *       currently ld_zq is preferred
 *       over ld_z. But for the sake of
 *       simplicity, the decoder should
 *       choose ld_z over ld_zq if q = 0.
 *
 * note: spm and spm #2
 *
 * */

/* Forward Declaration of static Functions */

static void test_decode_bytes_adc(vmcu_model_t *mcu);
static void test_decode_bytes_add(vmcu_model_t *mcu);
static void test_decode_bytes_adiw(vmcu_model_t *mcu);
static void test_decode_bytes_and(vmcu_model_t *mcu);
static void test_decode_bytes_andi(vmcu_model_t *mcu);
static void test_decode_bytes_asr(vmcu_model_t *mcu);
static void test_decode_bytes_bld(vmcu_model_t *mcu);
static void test_decode_bytes_brcc(vmcu_model_t *mcu);
static void test_decode_bytes_break(vmcu_model_t *mcu);
static void test_decode_bytes_breq(vmcu_model_t *mcu);
static void test_decode_bytes_brge(vmcu_model_t *mcu);
static void test_decode_bytes_brhc(vmcu_model_t *mcu);
static void test_decode_bytes_brhs(vmcu_model_t *mcu);
static void test_decode_bytes_brid(vmcu_model_t *mcu);
static void test_decode_bytes_brie(vmcu_model_t *mcu);
static void test_decode_bytes_brlo(vmcu_model_t *mcu);
static void test_decode_bytes_brlt(vmcu_model_t *mcu);
static void test_decode_bytes_brmi(vmcu_model_t *mcu);
static void test_decode_bytes_brne(vmcu_model_t *mcu);
static void test_decode_bytes_brpl(vmcu_model_t *mcu);
static void test_decode_bytes_brtc(vmcu_model_t *mcu);
static void test_decode_bytes_brts(vmcu_model_t *mcu);
static void test_decode_bytes_brvc(vmcu_model_t *mcu);
static void test_decode_bytes_brvs(vmcu_model_t *mcu);
static void test_decode_bytes_bst(vmcu_model_t *mcu);
static void test_decode_bytes_call(vmcu_model_t *mcu);
static void test_decode_bytes_cbi(vmcu_model_t *mcu);
static void test_decode_bytes_clc(vmcu_model_t *mcu);
static void test_decode_bytes_clh(vmcu_model_t *mcu);
static void test_decode_bytes_cli(vmcu_model_t *mcu);
static void test_decode_bytes_cln(vmcu_model_t *mcu);
static void test_decode_bytes_cls(vmcu_model_t *mcu);
static void test_decode_bytes_clt(vmcu_model_t *mcu);
static void test_decode_bytes_clv(vmcu_model_t *mcu);
static void test_decode_bytes_clz(vmcu_model_t *mcu);
static void test_decode_bytes_com(vmcu_model_t *mcu);
static void test_decode_bytes_cp(vmcu_model_t *mcu);
static void test_decode_bytes_cpc(vmcu_model_t *mcu);
static void test_decode_bytes_cpi(vmcu_model_t *mcu);
static void test_decode_bytes_cpse(vmcu_model_t *mcu);
static void test_decode_bytes_dec(vmcu_model_t *mcu);
static void test_decode_bytes_des(vmcu_model_t *mcu);
static void test_decode_bytes_eicall(vmcu_model_t *mcu);
static void test_decode_bytes_eijmp(vmcu_model_t *mcu);
static void test_decode_bytes_elpm(vmcu_model_t *mcu);
static void test_decode_bytes_eor(vmcu_model_t *mcu);
static void test_decode_bytes_fmul(vmcu_model_t *mcu);
static void test_decode_bytes_fmuls(vmcu_model_t *mcu);
static void test_decode_bytes_fmulsu(vmcu_model_t *mcu);
static void test_decode_bytes_icall(vmcu_model_t *mcu);
static void test_decode_bytes_ijmp(vmcu_model_t *mcu);
static void test_decode_bytes_in(vmcu_model_t *mcu);
static void test_decode_bytes_inc(vmcu_model_t *mcu);
static void test_decode_bytes_jmp(vmcu_model_t *mcu);
static void test_decode_bytes_lac(vmcu_model_t *mcu);
static void test_decode_bytes_las(vmcu_model_t *mcu);
static void test_decode_bytes_lat(vmcu_model_t *mcu);
static void test_decode_bytes_ldx(vmcu_model_t *mcu);
static void test_decode_bytes_ldy(vmcu_model_t *mcu);
static void test_decode_bytes_ldz(vmcu_model_t *mcu);
static void test_decode_bytes_ldi(vmcu_model_t *mcu);
static void test_decode_bytes_lds_32(vmcu_model_t *mcu);
static void test_decode_bytes_lds(vmcu_model_t *mcu);
static void test_decode_bytes_lpm(vmcu_model_t *mcu);
static void test_decode_bytes_lsr(vmcu_model_t *mcu);
static void test_decode_bytes_mov(vmcu_model_t *mcu);
static void test_decode_bytes_movw(vmcu_model_t *mcu);
static void test_decode_bytes_mul(vmcu_model_t *mcu);
static void test_decode_bytes_muls(vmcu_model_t *mcu);
static void test_decode_bytes_mulsu(vmcu_model_t *mcu);
static void test_decode_bytes_neg(vmcu_model_t *mcu);
static void test_decode_bytes_nop(vmcu_model_t *mcu);
static void test_decode_bytes_or(vmcu_model_t *mcu);
static void test_decode_bytes_ori(vmcu_model_t *mcu);
static void test_decode_bytes_out(vmcu_model_t *mcu);
static void test_decode_bytes_pop(vmcu_model_t *mcu);
static void test_decode_bytes_push(vmcu_model_t *mcu);
static void test_decode_bytes_rcall(vmcu_model_t *mcu);
static void test_decode_bytes_ret(vmcu_model_t *mcu);
static void test_decode_bytes_reti(vmcu_model_t *mcu);
static void test_decode_bytes_rjmp(vmcu_model_t *mcu);
static void test_decode_bytes_ror(vmcu_model_t *mcu);
static void test_decode_bytes_sbc(vmcu_model_t *mcu);
static void test_decode_bytes_sbci(vmcu_model_t *mcu);
static void test_decode_bytes_sbi(vmcu_model_t *mcu);
static void test_decode_bytes_sbic(vmcu_model_t *mcu);
static void test_decode_bytes_sbis(vmcu_model_t *mcu);
static void test_decode_bytes_sbiw(vmcu_model_t *mcu);
static void test_decode_bytes_sbrc(vmcu_model_t *mcu);
static void test_decode_bytes_sbrs(vmcu_model_t *mcu);
static void test_decode_bytes_sec(vmcu_model_t *mcu);
static void test_decode_bytes_seh(vmcu_model_t *mcu);
static void test_decode_bytes_sei(vmcu_model_t *mcu);
static void test_decode_bytes_sen(vmcu_model_t *mcu);
static void test_decode_bytes_ses(vmcu_model_t *mcu);
static void test_decode_bytes_set(vmcu_model_t *mcu);
static void test_decode_bytes_sev(vmcu_model_t *mcu);
static void test_decode_bytes_sez(vmcu_model_t *mcu);
static void test_decode_bytes_sleep(vmcu_model_t *mcu);
static void test_decode_bytes_spm(vmcu_model_t *mcu);
static void test_decode_bytes_stx(vmcu_model_t *mcu);
static void test_decode_bytes_sty(vmcu_model_t *mcu);
static void test_decode_bytes_stz(vmcu_model_t *mcu);
static void test_decode_bytes_sts_32(vmcu_model_t *mcu);
static void test_decode_bytes_sts(vmcu_model_t *mcu);
static void test_decode_bytes_sub(vmcu_model_t *mcu);
static void test_decode_bytes_subi(vmcu_model_t *mcu);
static void test_decode_bytes_swap(vmcu_model_t *mcu);
static void test_decode_bytes_wdr(vmcu_model_t *mcu);
static void test_decode_bytes_xch(vmcu_model_t *mcu);

/* --- Extern --- */

void test_decoder(void) {

    printf("\nCurrently testing: Decoder\n");

    /* init device models */

    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);

    /* start decoder tests */

    start(test_decode_bytes_adc, m328p);
    start(test_decode_bytes_add, m328p);
    start(test_decode_bytes_adiw, m328p);
    start(test_decode_bytes_and, m328p);
    start(test_decode_bytes_andi, m328p);
    start(test_decode_bytes_asr, m328p);
    start(test_decode_bytes_bld, m328p);
    start(test_decode_bytes_brcc, m328p);
    start(test_decode_bytes_break, m328p);
    start(test_decode_bytes_breq, m328p);
    start(test_decode_bytes_brge, m328p);
    start(test_decode_bytes_brhc, m328p);
    start(test_decode_bytes_brhs, m328p);
    start(test_decode_bytes_brid, m328p);
    start(test_decode_bytes_brie, m328p);
    start(test_decode_bytes_brlo, m328p);
    start(test_decode_bytes_brlt, m328p);
    start(test_decode_bytes_brmi, m328p);
    start(test_decode_bytes_brne, m328p);
    start(test_decode_bytes_brpl, m328p);
    start(test_decode_bytes_brtc, m328p);
    start(test_decode_bytes_brts, m328p);
    start(test_decode_bytes_brvc, m328p);
    start(test_decode_bytes_brvs, m328p);
    start(test_decode_bytes_bst, m328p);
    start(test_decode_bytes_call, m328p);
    start(test_decode_bytes_cbi, m328p);
    start(test_decode_bytes_clc, m328p);
    start(test_decode_bytes_clh, m328p);
    start(test_decode_bytes_cli, m328p);
    start(test_decode_bytes_cln, m328p);
    start(test_decode_bytes_cls, m328p);
    start(test_decode_bytes_clt, m328p);
    start(test_decode_bytes_clv, m328p);
    start(test_decode_bytes_clz, m328p);
    start(test_decode_bytes_com, m328p);
    start(test_decode_bytes_cp, m328p);
    start(test_decode_bytes_cpc, m328p);
    start(test_decode_bytes_cpi, m328p);
    start(test_decode_bytes_cpse, m328p);
    start(test_decode_bytes_dec, m328p);
    start(test_decode_bytes_des, m328p);
    start(test_decode_bytes_eicall, m328p);
    start(test_decode_bytes_eijmp, m328p);
    start(test_decode_bytes_elpm, m328p);
    start(test_decode_bytes_eor, m328p);
    start(test_decode_bytes_fmul, m328p);
    start(test_decode_bytes_fmuls, m328p);
    start(test_decode_bytes_fmulsu, m328p);
    start(test_decode_bytes_icall, m328p);
    start(test_decode_bytes_ijmp, m328p);
    start(test_decode_bytes_in, m328p);
    start(test_decode_bytes_inc, m328p);
    start(test_decode_bytes_jmp, m328p);
    start(test_decode_bytes_lac, m328p);
    start(test_decode_bytes_las, m328p);
    start(test_decode_bytes_lat, m328p);
    start(test_decode_bytes_ldx, m328p);
    start(test_decode_bytes_ldy, m328p);
    start(test_decode_bytes_ldz, m328p);
    start(test_decode_bytes_ldi, m328p);
    start(test_decode_bytes_lds_32, m328p);
    start(test_decode_bytes_lds, m328p);
    start(test_decode_bytes_lpm, m328p);
    start(test_decode_bytes_lsr, m328p);
    start(test_decode_bytes_mov, m328p);
    start(test_decode_bytes_movw, m328p);
    start(test_decode_bytes_mul, m328p);
    start(test_decode_bytes_muls, m328p);
    start(test_decode_bytes_mulsu, m328p);
    start(test_decode_bytes_neg, m328p);
    start(test_decode_bytes_nop, m328p);
    start(test_decode_bytes_or, m328p);
    start(test_decode_bytes_ori, m328p);
    start(test_decode_bytes_out, m328p);
    start(test_decode_bytes_pop, m328p);
    start(test_decode_bytes_push, m328p);
    start(test_decode_bytes_rcall, m328p);
    start(test_decode_bytes_ret, m328p);
    start(test_decode_bytes_reti, m328p);
    start(test_decode_bytes_rjmp, m328p);
    start(test_decode_bytes_ror, m328p);
    start(test_decode_bytes_sbc, m328p);
    start(test_decode_bytes_sbci, m328p);
    start(test_decode_bytes_sbi, m328p);
    start(test_decode_bytes_sbic, m328p);
    start(test_decode_bytes_sbis, m328p);
    start(test_decode_bytes_sbiw, m328p);
    start(test_decode_bytes_sbrc, m328p);
    start(test_decode_bytes_sbrs, m328p);
    start(test_decode_bytes_sec, m328p);
    start(test_decode_bytes_seh, m328p);
    start(test_decode_bytes_sei, m328p);
    start(test_decode_bytes_sen, m328p);
    start(test_decode_bytes_ses, m328p);
    start(test_decode_bytes_set, m328p);
    start(test_decode_bytes_sev, m328p);
    start(test_decode_bytes_sez, m328p);
    start(test_decode_bytes_sleep, m328p);
    start(test_decode_bytes_spm, m328p);
    start(test_decode_bytes_stx, m328p);
    start(test_decode_bytes_sty, m328p);
    start(test_decode_bytes_stz, m328p);
    start(test_decode_bytes_sts_32, m328p);
    start(test_decode_bytes_sts, m328p);
    start(test_decode_bytes_sub, m328p);
    start(test_decode_bytes_subi, m328p);
    start(test_decode_bytes_swap, m328p);
    start(test_decode_bytes_wdr, m328p);
    start(test_decode_bytes_xch, m328p);

    /* destroy device model */

    vmcu_model_dtor(m328p);

    /* end decoder tests */

    printf("\n");
}

/* --- Static --- */

static void test_decode_bytes_adc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ADC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x001c, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ADC);
    assert(instr.opcode == 0x1c00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x001f, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ADC);
    assert(instr.opcode == 0x1f00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_add(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ADD");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x000c, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ADD);
    assert(instr.opcode == 0x0c00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x110e, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ADD);
    assert(instr.opcode == 0x0e11);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_adiw(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ADIW");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0096, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ADIW);
    assert(instr.opcode == 0x9600);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x9096, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ADIW);
    assert(instr.opcode == 0x9690);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_and(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - AND");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0020, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_AND);
    assert(instr.opcode == 0x2000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x5921, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_AND);
    assert(instr.opcode == 0x2159);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_andi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ANDI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0070, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ANDI);
    assert(instr.opcode == 0x7000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xc872, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ANDI);
    assert(instr.opcode == 0x72c8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_asr(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ASR");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0594, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ASR);
    assert(instr.opcode == 0x9405);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf595, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ASR);
    assert(instr.opcode == 0x95f5);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_bld(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BLD");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00f8, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BLD);
    assert(instr.opcode == 0xf800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x07f8, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BLD);
    assert(instr.opcode == 0xf807);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brcc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRCC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRCC);
    assert(instr.opcode == 0xf400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x08f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRCC);
    assert(instr.opcode == 0xf408);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_break(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BREAK");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x9895, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BREAK);
    assert(instr.opcode == 0x9598);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_breq(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BREQ");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x01f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BREQ);
    assert(instr.opcode == 0xf001);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x01f3, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BREQ);
    assert(instr.opcode == 0xf301);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brge(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRGE");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x04f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRGE);
    assert(instr.opcode == 0xf404);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0cf4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRGE);
    assert(instr.opcode == 0xf40c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brhc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRHC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x05f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRHC);
    assert(instr.opcode == 0xf405);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x45f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRHC);
    assert(instr.opcode == 0xf445);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brhs(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRHS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x05f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRHS);
    assert(instr.opcode == 0xf005);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3df0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRHS);
    assert(instr.opcode == 0xf03d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brid(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRID");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x07f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRID);
    assert(instr.opcode == 0xf407);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x07f7, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRID);
    assert(instr.opcode == 0xf707);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brie(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRIE");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x07f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRIE);
    assert(instr.opcode == 0xf007);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x47f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRIE);
    assert(instr.opcode == 0xf047);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brlo(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRLO");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRLO);
    assert(instr.opcode == 0xf000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x20f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRLO);
    assert(instr.opcode == 0xf020);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brlt(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRLT");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x04f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRLT);
    assert(instr.opcode == 0xf004);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0cf0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRLT);
    assert(instr.opcode == 0xf00c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brmi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRMI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x02f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRMI);
    assert(instr.opcode == 0xf002);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3af0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRMI);
    assert(instr.opcode == 0xf03a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brne(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRNE");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x01f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRNE);
    assert(instr.opcode == 0xf401);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x11f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRNE);
    assert(instr.opcode == 0xf411);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brpl(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRPL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x02f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRPL);
    assert(instr.opcode == 0xf402);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0af4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRPL);
    assert(instr.opcode == 0xf40a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brtc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRTC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x06f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRTC);
    assert(instr.opcode == 0xf406);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0ef6, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRTC);
    assert(instr.opcode == 0xf60e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brts(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRTS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x06f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRTS);
    assert(instr.opcode == 0xf006);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0ef0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRTS);
    assert(instr.opcode == 0xf00e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brvc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRVC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x03f4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRVC);
    assert(instr.opcode == 0xf403);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1bf4, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRVC);
    assert(instr.opcode == 0xf41b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brvs(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BRVS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x03f0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRVS);
    assert(instr.opcode == 0xf003);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0bf0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BRVS);
    assert(instr.opcode == 0xf00b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_bst(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - BST");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00fa, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BST);
    assert(instr.opcode == 0xfa00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x01fa, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_BST);
    assert(instr.opcode == 0xfa01);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_call(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CALL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0e940000, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CALL);
    assert(instr.opcode == 0x940e0000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    vmcu_decode_bytes(0x0e950100, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CALL);
    assert(instr.opcode == 0x950e0001);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    PASSED;
}

static void test_decode_bytes_cbi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CBI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0098, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CBI);
    assert(instr.opcode == 0x9800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0198, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CBI);
    assert(instr.opcode == 0x9801);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x8894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLC);
    assert(instr.opcode == 0x9488);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clh(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLH");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xd894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLH);
    assert(instr.opcode == 0x94d8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cli(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xf894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLI);
    assert(instr.opcode == 0x94f8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cln(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLN");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xa894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLN);
    assert(instr.opcode == 0x94a8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cls(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xc894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLS);
    assert(instr.opcode == 0x94c8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clt(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLT");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xe894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLT);
    assert(instr.opcode == 0x94e8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clv(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLV");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xb894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLV);
    assert(instr.opcode == 0x94b8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clz(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CLZ");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x9894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CLZ);
    assert(instr.opcode == 0x9498);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_com(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - COM");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0094, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_COM);
    assert(instr.opcode == 0x9400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x2094, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_COM);
    assert(instr.opcode == 0x9420);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cp(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0014, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CP);
    assert(instr.opcode == 0x1400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0f14, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CP);
    assert(instr.opcode == 0x140f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cpc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CPC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0004, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CPC);
    assert(instr.opcode == 0x0400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x6304, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CPC);
    assert(instr.opcode == 0x0463);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cpi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CPI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0030, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CPI);
    assert(instr.opcode == 0x3000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0138, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CPI);
    assert(instr.opcode == 0x3801);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cpse(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - CPSE");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0010, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CPSE);
    assert(instr.opcode == 0x1000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0012, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_CPSE);
    assert(instr.opcode == 0x1200);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_dec(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - DEC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0a94, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_DEC);
    assert(instr.opcode == 0x940a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1a94, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_DEC);
    assert(instr.opcode == 0x941a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_des(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - DES");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0b94, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_DES);
    assert(instr.opcode == 0x940b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xfb94, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_DES);
    assert(instr.opcode == 0x94fb);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_eicall(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - EICALL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x1995, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_EICALL);
    assert(instr.opcode == 0x9519);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_eijmp(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - EIJMP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x1994, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_EIJMP);
    assert(instr.opcode == 0x9419);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_elpm(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ELPM");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xd895, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_ELPM);
    assert(instr.opcode == 0x95d8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0690, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_ELPMZ);
    assert(instr.opcode == 0x9006);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1690, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_ELPMZ);
    assert(instr.opcode == 0x9016);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0790, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_ELPMZI);
    assert(instr.opcode == 0x9007);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0791, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_ELPMZI);
    assert(instr.opcode == 0x9107);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_eor(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - EOR");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0024, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_EOR);
    assert(instr.opcode == 0x2400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xff27, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_EOR);
    assert(instr.opcode == 0x27ff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_fmul(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - FMUL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0803, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_FMUL);
    assert(instr.opcode == 0x0308);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0b03, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_FMUL);
    assert(instr.opcode == 0x030b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_fmuls(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - FMULS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x8003, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_FMULS);
    assert(instr.opcode == 0x0380);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xb703, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_FMULS);
    assert(instr.opcode == 0x03b7);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_fmulsu(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - FMULSU");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x8803, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_FMULSU);
    assert(instr.opcode == 0x0388);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x8903, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_FMULSU);
    assert(instr.opcode == 0x0389);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_icall(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ICALL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0995, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ICALL);
    assert(instr.opcode == 0x9509);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ijmp(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - IJMP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0994, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_IJMP);
    assert(instr.opcode == 0x9409);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_in(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - IN");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00b0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_IN);
    assert(instr.opcode == 0xb000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0fb0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_IN);
    assert(instr.opcode == 0xb00f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_inc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - INC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0394, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_INC);
    assert(instr.opcode == 0x9403);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1394, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_INC);
    assert(instr.opcode == 0x9413);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_jmp(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - JMP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0c940000, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_JMP);
    assert(instr.opcode == 0x940c0000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    vmcu_decode_bytes(0xfd95ffff, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_JMP);
    assert(instr.opcode == 0x95fdffff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    PASSED;
}

static void test_decode_bytes_lac(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LAC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0692, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LAC);
    assert(instr.opcode == 0x9206);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x9693, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LAC);
    assert(instr.opcode == 0x9396);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_las(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LAS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0592, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LAS);
    assert(instr.opcode == 0x9205);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1592, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LAS);
    assert(instr.opcode == 0x9215);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_lat(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LAT");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0792, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LAT);
    assert(instr.opcode == 0x9207);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0793, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LAT);
    assert(instr.opcode == 0x9307);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldx(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LD_X");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0c90, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_LDX);
    assert(instr.opcode == 0x900c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xfc91, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_LDX);
    assert(instr.opcode == 0x91fc);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0d90, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_LDXI);
    assert(instr.opcode == 0x900d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1d90, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_LDXI);
    assert(instr.opcode == 0x901d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0e90, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_LDDX);
    assert(instr.opcode == 0x900e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0e91, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_LDDX);
    assert(instr.opcode == 0x910e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldy(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LD_Y");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0880, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_LDY);
    assert(instr.opcode == 0x8008);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3880, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_LDY);
    assert(instr.opcode == 0x8038);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0990, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_LDYI);
    assert(instr.opcode == 0x9009);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1990, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_LDYI);
    assert(instr.opcode == 0x9019);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0a90, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_LDDY);
    assert(instr.opcode == 0x900a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3a90, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_LDDY);
    assert(instr.opcode == 0x903a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x08a0, &instr, mcu);

    /* (iv) (no base, (q = 0) => 0x8008 => ld_y */
    assert(instr.key    == VMCU_IKEY_LDDYQ);
    assert(instr.opcode == 0xa008);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldz(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LD_Z");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0080, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_LDDZQ);
    assert(instr.opcode == 0x8000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1080, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_LDDZQ);
    assert(instr.opcode == 0x8010);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0190, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_LDZI);
    assert(instr.opcode == 0x9001);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf191, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_LDZI);
    assert(instr.opcode == 0x91f1);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0290, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_LDDZ);
    assert(instr.opcode == 0x9002);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x7290, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_LDDZ);
    assert(instr.opcode == 0x9072);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf7ad, &instr, mcu);

    /* (iv) (no base, (q = 0) => 0x8000 => ld_z */
    assert(instr.key    == VMCU_IKEY_LDDZQ);
    assert(instr.opcode == 0xadf7);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LDI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00e0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LDI);
    assert(instr.opcode == 0xe000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x80e8, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LDI);
    assert(instr.opcode == 0xe880);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_lds_32(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LDS_32");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00900000, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LDS32);
    assert(instr.opcode == 0x90000000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    vmcu_decode_bytes(0x00910181, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LDS32);
    assert(instr.opcode == 0x91008101);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    PASSED;
}

static void test_decode_bytes_lds(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LDS");

    /* opcode collision with LDD */

    PASSED;
}

static void test_decode_bytes_lpm(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LPM");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xc895, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_LPM);
    assert(instr.opcode == 0x95c8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0490, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_LPMZ);
    assert(instr.opcode == 0x9004);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf491, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_LPMZ);
    assert(instr.opcode == 0x91f4);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0590, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_LPMZI);
    assert(instr.opcode == 0x9005);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3590, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_LPMZI);
    assert(instr.opcode == 0x9035);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_lsr(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - LSR");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0694, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LSR);
    assert(instr.opcode == 0x9406);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3694, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_LSR);
    assert(instr.opcode == 0x9436);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_mov(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - MOV");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x002c, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MOV);
    assert(instr.opcode == 0x2c00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x272c, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MOV);
    assert(instr.opcode == 0x2c27);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_movw(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - MOVW");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0001, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MOVW);
    assert(instr.opcode == 0x0100);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3f01, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MOVW);
    assert(instr.opcode == 0x013f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_mul(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - MUL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x009c, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MUL);
    assert(instr.opcode == 0x9c00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x019f, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MUL);
    assert(instr.opcode == 0x9f01);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_muls(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - MULS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0002, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MULS);
    assert(instr.opcode == 0x0200);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0f02, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MULS);
    assert(instr.opcode == 0x020f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_mulsu(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - MULSU");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0003, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MULSU);
    assert(instr.opcode == 0x0300);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0703, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_MULSU);
    assert(instr.opcode == 0x0307);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_neg(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - NEG");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0194, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_NEG);
    assert(instr.opcode == 0x9401);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x5194, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_NEG);
    assert(instr.opcode == 0x9451);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_nop(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - NOP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0000, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_NOP);
    assert(instr.opcode == 0x0000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_or(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - OR");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0028, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_OR);
    assert(instr.opcode == 0x2800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0128, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_OR);
    assert(instr.opcode == 0x2801);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ori(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ORI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0060, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ORI);
    assert(instr.opcode == 0x6000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xff6f, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ORI);
    assert(instr.opcode == 0x6fff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_out(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - OUT");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00b8, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_OUT);
    assert(instr.opcode == 0xb800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1fb8, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_OUT);
    assert(instr.opcode == 0xb81f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_pop(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - POP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0f90, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_POP);
    assert(instr.opcode == 0x900f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3f90, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_POP);
    assert(instr.opcode == 0x903f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_push(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - PUSH");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0f92, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_PUSH);
    assert(instr.opcode == 0x920f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1f92, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_PUSH);
    assert(instr.opcode == 0x921f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_rcall(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - RCALL");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00d0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_RCALL);
    assert(instr.opcode == 0xd000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xffdf, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_RCALL);
    assert(instr.opcode == 0xdfff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ret(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - RET");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0895, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_RET);
    assert(instr.opcode == 0x9508);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_reti(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - RETI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x1895, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_RETI);
    assert(instr.opcode == 0x9518);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_rjmp(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - RJMP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00c0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_RJMP);
    assert(instr.opcode == 0xc000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x03c0, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_RJMP);
    assert(instr.opcode == 0xc003);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}


static void test_decode_bytes_ror(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ROR");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0794, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ROR);
    assert(instr.opcode == 0x9407);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3794, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_ROR);
    assert(instr.opcode == 0x9437);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0008, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBC);
    assert(instr.opcode == 0x0800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0708, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBC);
    assert(instr.opcode == 0x0807);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbci(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBCI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0040, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBCI);
    assert(instr.opcode == 0x4000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1f40, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBCI);
    assert(instr.opcode == 0x401f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x009a, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBI);
    assert(instr.opcode == 0x9a00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0d9a, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBI);
    assert(instr.opcode == 0x9a0d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbic(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBIC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0099, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBIC);
    assert(instr.opcode == 0x9900);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xff99, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBIC);
    assert(instr.opcode == 0x99ff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbis(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBIS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x009b, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBIS);
    assert(instr.opcode == 0x9b00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1f9b, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBIS);
    assert(instr.opcode == 0x9b1f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbiw(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBIW");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0097, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBIW);
    assert(instr.opcode == 0x9700);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0797, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBIW);
    assert(instr.opcode == 0x9707);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbrc(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBRC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00fc, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBRC);
    assert(instr.opcode == 0xfc00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x17fc, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBRC);
    assert(instr.opcode == 0xfc17);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sbrs(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SBRS");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00fe, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBRS);
    assert(instr.opcode == 0xfe00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x07fe, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SBRS);
    assert(instr.opcode == 0xfe07);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sec(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SEC");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SEC);
    assert(instr.opcode == 0x9408);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_seh(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SEH");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x5894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SEH);
    assert(instr.opcode == 0x9458);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sei(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SEI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x7894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SEI);
    assert(instr.opcode == 0x9478);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sen(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SEN");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x2894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SEN);
    assert(instr.opcode == 0x9428);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ses(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SES");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x4894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SES);
    assert(instr.opcode == 0x9448);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_set(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SET");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x6894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SET);
    assert(instr.opcode == 0x9468);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sev(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SEV");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x3894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SEV);
    assert(instr.opcode == 0x9438);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sez(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SEZ");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x1894, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SEZ);
    assert(instr.opcode == 0x9418);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sleep(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SLEEP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x8895, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SLEEP);
    assert(instr.opcode == 0x9588);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_spm(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SPM");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xe895, &instr, mcu);

    /* (i) - (iii) */
    assert(instr.key    == VMCU_IKEY_SPM);
    assert(instr.opcode == 0x95e8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    /* (iv) - (vi) */
    vmcu_decode_bytes(0xf895, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SPMZI);
    assert(instr.opcode == 0x95f8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_stx(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ST_X");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0c92, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_STX);
    assert(instr.opcode == 0x920c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x7c92, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_STX);
    assert(instr.opcode == 0x927c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0d92, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_STXI);
    assert(instr.opcode == 0x920d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3d92, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_STXI);
    assert(instr.opcode == 0x923d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0e92, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_STDX);
    assert(instr.opcode == 0x920e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x5e92, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_STDX);
    assert(instr.opcode == 0x925e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sty(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ST_Y");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0882, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_STY);
    assert(instr.opcode == 0x8208);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf883, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_STY);
    assert(instr.opcode == 0x83f8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0992, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_STYI);
    assert(instr.opcode == 0x9209);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1992, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_STYI);
    assert(instr.opcode == 0x9219);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0a92, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_STDY);
    assert(instr.opcode == 0x920a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1a92, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_STDY);
    assert(instr.opcode == 0x921a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3fa2, &instr, mcu);

    /* (iv) (no base, (q = 0) => 0x8208 => st_y */
    assert(instr.key    == VMCU_IKEY_STDYQ);
    assert(instr.opcode == 0xa23f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_stz(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - ST_Z");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0082, &instr, mcu);

    /* (i) - base */
    assert(instr.key    == VMCU_IKEY_STZ);
    assert(instr.opcode == 0x8200);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3082, &instr, mcu);

    /* (i) */
    assert(instr.key    == VMCU_IKEY_STZ);
    assert(instr.opcode == 0x8230);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0192, &instr, mcu);

    /* (ii) - base */
    assert(instr.key    == VMCU_IKEY_STZI);
    assert(instr.opcode == 0x9201);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1192, &instr, mcu);

    /* (ii) */
    assert(instr.key    == VMCU_IKEY_STZI);
    assert(instr.opcode == 0x9211);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0292, &instr, mcu);

    /* (iii) - base */
    assert(instr.key    == VMCU_IKEY_STDZ);
    assert(instr.opcode == 0x9202);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf293, &instr, mcu);

    /* (iii) */
    assert(instr.key    == VMCU_IKEY_STDZ);
    assert(instr.opcode == 0x93f2);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x00ae, &instr, mcu);

    /* (iv) (no base, (q = 0) => 0x8200 => st_z */
    assert(instr.key    == VMCU_IKEY_STDZQ);
    assert(instr.opcode == 0xae00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_sts_32(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - STS_32");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x00920000, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_STS32);
    assert(instr.opcode == 0x92000000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    vmcu_decode_bytes(0x409341e6, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_STS32);
    assert(instr.opcode == 0x9340e641);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    PASSED;
}

static void test_decode_bytes_sts(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - STS");

    /* opcode collision with std */

    PASSED;
}

static void test_decode_bytes_sub(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SUB");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0018, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SUB);
    assert(instr.opcode == 0x1800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0318, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SUB);
    assert(instr.opcode == 0x1803);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_subi(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SUBI");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0050, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SUBI);
    assert(instr.opcode == 0x5000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1f50, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SUBI);
    assert(instr.opcode == 0x501f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_swap(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - SWAP");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0294, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SWAP);
    assert(instr.opcode == 0x9402);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1294, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_SWAP);
    assert(instr.opcode == 0x9412);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_wdr(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - WDR");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0xa895, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_WDR);
    assert(instr.opcode == 0x95a8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_xch(vmcu_model_t *mcu) {

    printf("vmcu_decode_bytes() - XCH");

    vmcu_instr_t instr;
    vmcu_decode_bytes(0x0492, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_XCH);
    assert(instr.opcode == 0x9204);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x7492, &instr, mcu);

    assert(instr.key    == VMCU_IKEY_XCH);
    assert(instr.opcode == 0x9274);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}