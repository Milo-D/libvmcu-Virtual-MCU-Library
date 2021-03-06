/* Decoder Integration Test Implementation */

// C Headers
#include <stdio.h>
#include <assert.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

// Test Headers
#include "decoder_test.h"

#define DFL   "\033[0m"
#define GREEN "\033[32;1m"

#define PASSED printf("%s PASSED %s\n", GREEN, DFL)

#define start(fn) printf("    |\n");    \
                  printf("    |---- "); \
                  fn()
/*
 * brlo shadows brcs.
 * cl<x> shadows bclr.
 * se<x> shadows bset.
 *
 * todo: remove brcs, bclr, bset
 *
 * */

/* Forward Declaration of static Functions */

static void test_decode_bytes_adc(void);
static void test_decode_bytes_add(void);
static void test_decode_bytes_adiw(void);
static void test_decode_bytes_and(void);
static void test_decode_bytes_andi(void);
static void test_decode_bytes_asr(void);
static void test_decode_bytes_bld(void);
static void test_decode_bytes_brcc(void);
static void test_decode_bytes_break(void);
static void test_decode_bytes_breq(void);
static void test_decode_bytes_brge(void);
static void test_decode_bytes_brhc(void);
static void test_decode_bytes_brhs(void);
static void test_decode_bytes_brid(void);
static void test_decode_bytes_brie(void);
static void test_decode_bytes_brlo(void);
static void test_decode_bytes_brlt(void);
static void test_decode_bytes_brmi(void);
static void test_decode_bytes_brne(void);
static void test_decode_bytes_brpl(void);
static void test_decode_bytes_brtc(void);
static void test_decode_bytes_brts(void);
static void test_decode_bytes_brvc(void);
static void test_decode_bytes_brvs(void);
static void test_decode_bytes_bst(void);
static void test_decode_bytes_call(void);
static void test_decode_bytes_cbi(void);
static void test_decode_bytes_clc(void);
static void test_decode_bytes_clh(void);
static void test_decode_bytes_cli(void);
static void test_decode_bytes_cln(void);
static void test_decode_bytes_cls(void);
static void test_decode_bytes_clt(void);
static void test_decode_bytes_clv(void);
static void test_decode_bytes_clz(void);
static void test_decode_bytes_com(void);
static void test_decode_bytes_cp(void);
static void test_decode_bytes_cpc(void);
static void test_decode_bytes_cpi(void);
static void test_decode_bytes_cpse(void);
static void test_decode_bytes_dec(void);
static void test_decode_bytes_des(void);
static void test_decode_bytes_eicall(void);
static void test_decode_bytes_eijmp(void);
static void test_decode_bytes_elpm(void);
static void test_decode_bytes_eor(void);
static void test_decode_bytes_fmul(void);
static void test_decode_bytes_fmuls(void);
static void test_decode_bytes_fmulsu(void);
static void test_decode_bytes_icall(void);
static void test_decode_bytes_ijmp(void);
static void test_decode_bytes_in(void);
static void test_decode_bytes_inc(void);
static void test_decode_bytes_jmp(void);
static void test_decode_bytes_lac(void);
static void test_decode_bytes_las(void);
static void test_decode_bytes_lat(void);
static void test_decode_bytes_ldx(void);
static void test_decode_bytes_ldy(void);
static void test_decode_bytes_ldz(void);

/* --- Extern --- */

void test_decoder(void) {

    printf("\nCurrently testing: Decoder\n");

    start(test_decode_bytes_adc);
    start(test_decode_bytes_add);
    start(test_decode_bytes_adiw);
    start(test_decode_bytes_and);
    start(test_decode_bytes_andi);
    start(test_decode_bytes_asr);
    start(test_decode_bytes_bld);
    start(test_decode_bytes_brcc);
    start(test_decode_bytes_break);
    start(test_decode_bytes_breq);
    start(test_decode_bytes_brge);
    start(test_decode_bytes_brhc);
    start(test_decode_bytes_brhs);
    start(test_decode_bytes_brid);
    start(test_decode_bytes_brie);
    start(test_decode_bytes_brlo);
    start(test_decode_bytes_brlt);
    start(test_decode_bytes_brmi);
    start(test_decode_bytes_brne);
    start(test_decode_bytes_brpl);
    start(test_decode_bytes_brtc);
    start(test_decode_bytes_brts);
    start(test_decode_bytes_brvc);
    start(test_decode_bytes_brvs);
    start(test_decode_bytes_bst);
    start(test_decode_bytes_call);
    start(test_decode_bytes_cbi);
    start(test_decode_bytes_clc);
    start(test_decode_bytes_clh);
    start(test_decode_bytes_cli);
    start(test_decode_bytes_cln);
    start(test_decode_bytes_cls);
    start(test_decode_bytes_clt);
    start(test_decode_bytes_clv);
    start(test_decode_bytes_clz);
    start(test_decode_bytes_com);
    start(test_decode_bytes_cp);
    start(test_decode_bytes_cpc);
    start(test_decode_bytes_cpi);
    start(test_decode_bytes_cpse);
    start(test_decode_bytes_dec);
    start(test_decode_bytes_des);
    start(test_decode_bytes_eicall);
    start(test_decode_bytes_eijmp);
    start(test_decode_bytes_elpm);
    start(test_decode_bytes_eor);
    start(test_decode_bytes_fmul);
    start(test_decode_bytes_fmuls);
    start(test_decode_bytes_fmulsu);
    start(test_decode_bytes_icall);
    start(test_decode_bytes_ijmp);
    start(test_decode_bytes_in);
    start(test_decode_bytes_inc);
    start(test_decode_bytes_jmp);
    start(test_decode_bytes_lac);
    start(test_decode_bytes_las);
    start(test_decode_bytes_lat);
    start(test_decode_bytes_ldx);
    start(test_decode_bytes_ldy);
    start(test_decode_bytes_ldz);

    printf("\n");
}

/* --- Static --- */

static void test_decode_bytes_adc(void) {

    printf("vmcu_decode_bytes() - ADC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x001c, &instr);

    assert(instr.key    == VMCU_ADC);
    assert(instr.opcode == 0x1c00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x001f, &instr);

    assert(instr.key    == VMCU_ADC);
    assert(instr.opcode == 0x1f00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_add(void) {

    printf("vmcu_decode_bytes() - ADD");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x000c, &instr);

    assert(instr.key    == VMCU_ADD);
    assert(instr.opcode == 0x0c00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x110e, &instr);

    assert(instr.key    == VMCU_ADD);
    assert(instr.opcode == 0x0e11);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_adiw(void) {

    printf("vmcu_decode_bytes() - ADIW");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0096, &instr);

    assert(instr.key    == VMCU_ADIW);
    assert(instr.opcode == 0x9600);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x9096, &instr);

    assert(instr.key    == VMCU_ADIW);
    assert(instr.opcode == 0x9690);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_and(void) {

    printf("vmcu_decode_bytes() - AND");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0020, &instr);

    assert(instr.key    == VMCU_AND);
    assert(instr.opcode == 0x2000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x5921, &instr);

    assert(instr.key    == VMCU_AND);
    assert(instr.opcode == 0x2159);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_andi(void) {

    printf("vmcu_decode_bytes() - ANDI");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0070, &instr);

    assert(instr.key    == VMCU_ANDI);
    assert(instr.opcode == 0x7000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xc872, &instr);

    assert(instr.key    == VMCU_ANDI);
    assert(instr.opcode == 0x72c8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_asr(void) {

    printf("vmcu_decode_bytes() - ASR");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0594, &instr);

    assert(instr.key    == VMCU_ASR);
    assert(instr.opcode == 0x9405);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf595, &instr);

    assert(instr.key    == VMCU_ASR);
    assert(instr.opcode == 0x95f5);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_bld(void) {

    printf("vmcu_decode_bytes() - BLD");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x00f8, &instr);

    assert(instr.key    == VMCU_BLD);
    assert(instr.opcode == 0xf800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x07f8, &instr);

    assert(instr.key    == VMCU_BLD);
    assert(instr.opcode == 0xf807);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brcc(void) {

    printf("vmcu_decode_bytes() - BRCC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x00f4, &instr);

    assert(instr.key    == VMCU_BRCC);
    assert(instr.opcode == 0xf400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x08f4, &instr);

    assert(instr.key    == VMCU_BRCC);
    assert(instr.opcode == 0xf408);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_break(void) {

    printf("vmcu_decode_bytes() - BREAK");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x9895, &instr);

    assert(instr.key    == VMCU_BREAK);
    assert(instr.opcode == 0x9598);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_breq(void) {

    printf("vmcu_decode_bytes() - BREQ");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x01f0, &instr);

    assert(instr.key    == VMCU_BREQ);
    assert(instr.opcode == 0xf001);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x01f3, &instr);

    assert(instr.key    == VMCU_BREQ);
    assert(instr.opcode == 0xf301);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brge(void) {

    printf("vmcu_decode_bytes() - BRGE");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x04f4, &instr);

    assert(instr.key    == VMCU_BRGE);
    assert(instr.opcode == 0xf404);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0cf4, &instr);

    assert(instr.key    == VMCU_BRGE);
    assert(instr.opcode == 0xf40c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brhc(void) {

    printf("vmcu_decode_bytes() - BRHC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x05f4, &instr);

    assert(instr.key    == VMCU_BRHC);
    assert(instr.opcode == 0xf405);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x45f4, &instr);

    assert(instr.key    == VMCU_BRHC);
    assert(instr.opcode == 0xf445);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brhs(void) {

    printf("vmcu_decode_bytes() - BRHS");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x05f0, &instr);

    assert(instr.key    == VMCU_BRHS);
    assert(instr.opcode == 0xf005);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3df0, &instr);

    assert(instr.key    == VMCU_BRHS);
    assert(instr.opcode == 0xf03d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brid(void) {

    printf("vmcu_decode_bytes() - BRID");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x07f4, &instr);

    assert(instr.key    == VMCU_BRID);
    assert(instr.opcode == 0xf407);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x07f7, &instr);

    assert(instr.key    == VMCU_BRID);
    assert(instr.opcode == 0xf707);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brie(void) {

    printf("vmcu_decode_bytes() - BRIE");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x07f0, &instr);

    assert(instr.key    == VMCU_BRIE);
    assert(instr.opcode == 0xf007);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x47f0, &instr);

    assert(instr.key    == VMCU_BRIE);
    assert(instr.opcode == 0xf047);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brlo(void) {

    printf("vmcu_decode_bytes() - BRLO");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x00f0, &instr);

    assert(instr.key    == VMCU_BRLO);
    assert(instr.opcode == 0xf000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x20f0, &instr);

    assert(instr.key    == VMCU_BRLO);
    assert(instr.opcode == 0xf020);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brlt(void) {

    printf("vmcu_decode_bytes() - BRLT");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x04f0, &instr);

    assert(instr.key    == VMCU_BRLT);
    assert(instr.opcode == 0xf004);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0cf0, &instr);

    assert(instr.key    == VMCU_BRLT);
    assert(instr.opcode == 0xf00c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brmi(void) {

    printf("vmcu_decode_bytes() - BRMI");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x02f0, &instr);

    assert(instr.key    == VMCU_BRMI);
    assert(instr.opcode == 0xf002);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3af0, &instr);

    assert(instr.key    == VMCU_BRMI);
    assert(instr.opcode == 0xf03a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brne(void) {

    printf("vmcu_decode_bytes() - BRNE");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x01f4, &instr);

    assert(instr.key    == VMCU_BRNE);
    assert(instr.opcode == 0xf401);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x11f4, &instr);

    assert(instr.key    == VMCU_BRNE);
    assert(instr.opcode == 0xf411);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brpl(void) {

    printf("vmcu_decode_bytes() - BRPL");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x02f4, &instr);

    assert(instr.key    == VMCU_BRPL);
    assert(instr.opcode == 0xf402);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0af4, &instr);

    assert(instr.key    == VMCU_BRPL);
    assert(instr.opcode == 0xf40a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brtc(void) {

    printf("vmcu_decode_bytes() - BRTC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x06f4, &instr);

    assert(instr.key    == VMCU_BRTC);
    assert(instr.opcode == 0xf406);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0ef6, &instr);

    assert(instr.key    == VMCU_BRTC);
    assert(instr.opcode == 0xf60e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brts(void) {

    printf("vmcu_decode_bytes() - BRTS");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x06f0, &instr);

    assert(instr.key    == VMCU_BRTS);
    assert(instr.opcode == 0xf006);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0ef0, &instr);

    assert(instr.key    == VMCU_BRTS);
    assert(instr.opcode == 0xf00e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brvc(void) {

    printf("vmcu_decode_bytes() - BRVC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x03f4, &instr);

    assert(instr.key    == VMCU_BRVC);
    assert(instr.opcode == 0xf403);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1bf4, &instr);

    assert(instr.key    == VMCU_BRVC);
    assert(instr.opcode == 0xf41b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_brvs(void) {

    printf("vmcu_decode_bytes() - BRVS");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x03f0, &instr);

    assert(instr.key    == VMCU_BRVS);
    assert(instr.opcode == 0xf003);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0bf0, &instr);

    assert(instr.key    == VMCU_BRVS);
    assert(instr.opcode == 0xf00b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_bst(void) {

    printf("vmcu_decode_bytes() - BST");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x00fa, &instr);

    assert(instr.key    == VMCU_BST);
    assert(instr.opcode == 0xfa00);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x01fa, &instr);

    assert(instr.key    == VMCU_BST);
    assert(instr.opcode == 0xfa01);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_call(void) {

    printf("vmcu_decode_bytes() - CALL");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0e940000, &instr);

    assert(instr.key    == VMCU_CALL);
    assert(instr.opcode == 0x940e0000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    vmcu_decode_bytes(0x0e950100, &instr);

    assert(instr.key    == VMCU_CALL);
    assert(instr.opcode == 0x950e0001);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    PASSED;
}

static void test_decode_bytes_cbi(void) {

    printf("vmcu_decode_bytes() - CBI");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0098, &instr);

    assert(instr.key    == VMCU_CBI);
    assert(instr.opcode == 0x9800);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0198, &instr);

    assert(instr.key    == VMCU_CBI);
    assert(instr.opcode == 0x9801);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clc(void) {

    printf("vmcu_decode_bytes() - CLC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x8894, &instr);

    assert(instr.key    == VMCU_CLC);
    assert(instr.opcode == 0x9488);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clh(void) {

    printf("vmcu_decode_bytes() - CLH");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xd894, &instr);

    assert(instr.key    == VMCU_CLH);
    assert(instr.opcode == 0x94d8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cli(void) {

    printf("vmcu_decode_bytes() - CLI");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xf894, &instr);

    assert(instr.key    == VMCU_CLI);
    assert(instr.opcode == 0x94f8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cln(void) {

    printf("vmcu_decode_bytes() - CLN");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xa894, &instr);

    assert(instr.key    == VMCU_CLN);
    assert(instr.opcode == 0x94a8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cls(void) {

    printf("vmcu_decode_bytes() - CLS");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xc894, &instr);

    assert(instr.key    == VMCU_CLS);
    assert(instr.opcode == 0x94c8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clt(void) {

    printf("vmcu_decode_bytes() - CLT");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xe894, &instr);

    assert(instr.key    == VMCU_CLT);
    assert(instr.opcode == 0x94e8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clv(void) {

    printf("vmcu_decode_bytes() - CLV");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xb894, &instr);

    assert(instr.key    == VMCU_CLV);
    assert(instr.opcode == 0x94b8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_clz(void) {

    printf("vmcu_decode_bytes() - CLZ");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x9894, &instr);

    assert(instr.key    == VMCU_CLZ);
    assert(instr.opcode == 0x9498);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_com(void) {

    printf("vmcu_decode_bytes() - COM");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0094, &instr);

    assert(instr.key    == VMCU_COM);
    assert(instr.opcode == 0x9400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x2094, &instr);

    assert(instr.key    == VMCU_COM);
    assert(instr.opcode == 0x9420);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cp(void) {

    printf("vmcu_decode_bytes() - CP");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0014, &instr);

    assert(instr.key    == VMCU_CP);
    assert(instr.opcode == 0x1400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0f14, &instr);

    assert(instr.key    == VMCU_CP);
    assert(instr.opcode == 0x140f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cpc(void) {

    printf("vmcu_decode_bytes() - CPC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0004, &instr);

    assert(instr.key    == VMCU_CPC);
    assert(instr.opcode == 0x0400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x6304, &instr);

    assert(instr.key    == VMCU_CPC);
    assert(instr.opcode == 0x0463);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cpi(void) {

    printf("vmcu_decode_bytes() - CPI");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0030, &instr);

    assert(instr.key    == VMCU_CPI);
    assert(instr.opcode == 0x3000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0138, &instr);

    assert(instr.key    == VMCU_CPI);
    assert(instr.opcode == 0x3801);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_cpse(void) {

    printf("vmcu_decode_bytes() - CPSE");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0010, &instr);

    assert(instr.key    == VMCU_CPSE);
    assert(instr.opcode == 0x1000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0012, &instr);

    assert(instr.key    == VMCU_CPSE);
    assert(instr.opcode == 0x1200);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_dec(void) {

    printf("vmcu_decode_bytes() - DEC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0a94, &instr);

    assert(instr.key    == VMCU_DEC);
    assert(instr.opcode == 0x940a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1a94, &instr);

    assert(instr.key    == VMCU_DEC);
    assert(instr.opcode == 0x941a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_des(void) {

    printf("vmcu_decode_bytes() - DES");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0b94, &instr);

    assert(instr.key    == VMCU_DES);
    assert(instr.opcode == 0x940b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xfb94, &instr);

    assert(instr.key    == VMCU_DES);
    assert(instr.opcode == 0x94fb);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_eicall(void) {

    printf("vmcu_decode_bytes() - EICALL");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x1995, &instr);

    assert(instr.key    == VMCU_EICALL);
    assert(instr.opcode == 0x9519);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_eijmp(void) {

    printf("vmcu_decode_bytes() - EIJMP");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x1994, &instr);

    assert(instr.key    == VMCU_EIJMP);
    assert(instr.opcode == 0x9419);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_elpm(void) {

    printf("vmcu_decode_bytes() - ELPM");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xd895, &instr);

    /* (i) */
    assert(instr.key    == VMCU_ELPM);
    assert(instr.opcode == 0x95d8);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0690, &instr);

    /* (ii) - base */
    assert(instr.key    == VMCU_ELPMZ);
    assert(instr.opcode == 0x9006);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1690, &instr);

    /* (ii) */
    assert(instr.key    == VMCU_ELPMZ);
    assert(instr.opcode == 0x9016);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0790, &instr);

    /* (iii) - base */
    assert(instr.key    == VMCU_ELPMZI);
    assert(instr.opcode == 0x9007);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0791, &instr);

    /* (iii) */
    assert(instr.key    == VMCU_ELPMZI);
    assert(instr.opcode == 0x9107);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_eor(void) {

    printf("vmcu_decode_bytes() - EOR");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0024, &instr);

    assert(instr.key    == VMCU_EOR);
    assert(instr.opcode == 0x2400);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xff27, &instr);

    assert(instr.key    == VMCU_EOR);
    assert(instr.opcode == 0x27ff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_fmul(void) {

    printf("vmcu_decode_bytes() - FMUL");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0803, &instr);

    assert(instr.key    == VMCU_FMUL);
    assert(instr.opcode == 0x0308);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0b03, &instr);

    assert(instr.key    == VMCU_FMUL);
    assert(instr.opcode == 0x030b);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_fmuls(void) {

    printf("vmcu_decode_bytes() - FMULS");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x8003, &instr);

    assert(instr.key    == VMCU_FMULS);
    assert(instr.opcode == 0x0380);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xb703, &instr);

    assert(instr.key    == VMCU_FMULS);
    assert(instr.opcode == 0x03b7);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_fmulsu(void) {

    printf("vmcu_decode_bytes() - FMULSU");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x8803, &instr);

    assert(instr.key    == VMCU_FMULSU);
    assert(instr.opcode == 0x0388);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x8903, &instr);

    assert(instr.key    == VMCU_FMULSU);
    assert(instr.opcode == 0x0389);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_icall(void) {

    printf("vmcu_decode_bytes() - ICALL");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0995, &instr);

    assert(instr.key    == VMCU_ICALL);
    assert(instr.opcode == 0x9509);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ijmp(void) {

    printf("vmcu_decode_bytes() - IJMP");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0994, &instr);

    assert(instr.key    == VMCU_IJMP);
    assert(instr.opcode == 0x9409);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_in(void) {

    printf("vmcu_decode_bytes() - IN");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x00b0, &instr);

    assert(instr.key    == VMCU_IN);
    assert(instr.opcode == 0xb000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0fb0, &instr);

    assert(instr.key    == VMCU_IN);
    assert(instr.opcode == 0xb00f);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_inc(void) {

    printf("vmcu_decode_bytes() - INC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0394, &instr);

    assert(instr.key    == VMCU_INC);
    assert(instr.opcode == 0x9403);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1394, &instr);

    assert(instr.key    == VMCU_INC);
    assert(instr.opcode == 0x9413);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_jmp(void) {

    printf("vmcu_decode_bytes() - JMP");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0c940000, &instr);

    assert(instr.key    == VMCU_JMP);
    assert(instr.opcode == 0x940c0000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    vmcu_decode_bytes(0xfd95ffff, &instr);

    assert(instr.key    == VMCU_JMP);
    assert(instr.opcode == 0x95fdffff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == true);

    PASSED;
}

static void test_decode_bytes_lac(void) {

    printf("vmcu_decode_bytes() - LAC");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0692, &instr);

    assert(instr.key    == VMCU_LAC);
    assert(instr.opcode == 0x9206);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x9693, &instr);

    assert(instr.key    == VMCU_LAC);
    assert(instr.opcode == 0x9396);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_las(void) {

    printf("vmcu_decode_bytes() - LAS");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0592, &instr);

    assert(instr.key    == VMCU_LAS);
    assert(instr.opcode == 0x9205);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1592, &instr);

    assert(instr.key    == VMCU_LAS);
    assert(instr.opcode == 0x9215);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_lat(void) {

    printf("vmcu_decode_bytes() - LAT");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0792, &instr);

    assert(instr.key    == VMCU_LAT);
    assert(instr.opcode == 0x9207);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0793, &instr);

    assert(instr.key    == VMCU_LAT);
    assert(instr.opcode == 0x9307);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldx(void) {

    printf("vmcu_decode_bytes() - LD_X");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0c90, &instr);

    /* (i) - base */
    assert(instr.key    == VMCU_LDX);
    assert(instr.opcode == 0x900c);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xfc91, &instr);

    /* (i) */
    assert(instr.key    == VMCU_LDX);
    assert(instr.opcode == 0x91fc);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0d90, &instr);

    /* (ii) - base */
    assert(instr.key    == VMCU_LDXI);
    assert(instr.opcode == 0x900d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1d90, &instr);

    /* (ii) */
    assert(instr.key    == VMCU_LDXI);
    assert(instr.opcode == 0x901d);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0e90, &instr);

    /* (iii) - base */
    assert(instr.key    == VMCU_LDDX);
    assert(instr.opcode == 0x900e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0e91, &instr);

    /* (iii) */
    assert(instr.key    == VMCU_LDDX);
    assert(instr.opcode == 0x910e);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldy(void) {

    printf("vmcu_decode_bytes() - LD_Y");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0880, &instr);

    /* (i) - base */
    assert(instr.key    == VMCU_LDY);
    assert(instr.opcode == 0x8008);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3880, &instr);

    /* (i) */
    assert(instr.key    == VMCU_LDY);
    assert(instr.opcode == 0x8038);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0990, &instr);

    /* (ii) - base */
    assert(instr.key    == VMCU_LDYI);
    assert(instr.opcode == 0x9009);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1990, &instr);

    /* (ii) */
    assert(instr.key    == VMCU_LDYI);
    assert(instr.opcode == 0x9019);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0a90, &instr);

    /* (iii) - base */
    assert(instr.key    == VMCU_LDDY);
    assert(instr.opcode == 0x900a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x3a90, &instr);

    /* (iii) */
    assert(instr.key    == VMCU_LDDY);
    assert(instr.opcode == 0x903a);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x08a0, &instr);

    /* (iv) (no base, (q = 0) => 0x8008 => ld_y */
    assert(instr.key    == VMCU_LDDYQ);
    assert(instr.opcode == 0xa008);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_ldz(void) {

    // todo: swap ld_zq in opcode table
    //       currently ld_zq is preferred
    //       over ld_z. But for the sake of
    //       simplicity, the decoder should
    //       choose ld_z over ld_zq if q = 0.

    printf("vmcu_decode_bytes() - LD_Z");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0080, &instr);

    /* (i) - base */
    assert(instr.key    == VMCU_LDDZQ);
    assert(instr.opcode == 0x8000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x1080, &instr);

    /* (i) */
    assert(instr.key    == VMCU_LDDZQ);
    assert(instr.opcode == 0x8010);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0190, &instr);

    /* (ii) - base */
    assert(instr.key    == VMCU_LDZI);
    assert(instr.opcode == 0x9001);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf191, &instr);

    /* (ii) */
    assert(instr.key    == VMCU_LDZI);
    assert(instr.opcode == 0x91f1);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x0290, &instr);

    /* (iii) - base */
    assert(instr.key    == VMCU_LDDZ);
    assert(instr.opcode == 0x9002);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0x7290, &instr);

    /* (iii) */
    assert(instr.key    == VMCU_LDDZ);
    assert(instr.opcode == 0x9072);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    vmcu_decode_bytes(0xf7ad, &instr);

    /* (iv) (no base, (q = 0) => 0x8000 => ld_z */
    assert(instr.key    == VMCU_LDDZQ);
    assert(instr.opcode == 0xadf7);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}



