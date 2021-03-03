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

/*
 * brlo shadows brcs.
 * cl<x> shadows bclr.
 *
 * todo: remove brcs, bclr
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

/* --- Extern --- */

void test_decoder(void) {

    printf("\nCurrently testing: Decoder\n");

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_adc();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_add();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_adiw();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_and();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_andi();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_asr();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_bld();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brcc();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_break();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_breq();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brge();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brhc();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brhs();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brid();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brie();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brlo();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brlt();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brmi();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brne();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brpl();

    printf("    |\n");
    printf("    |---- ");

    test_decode_bytes_brtc();

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