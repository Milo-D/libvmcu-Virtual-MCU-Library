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

/* Forward Declaration of static Functions */

static void test_decode_bytes_adc(void);
static void test_decode_bytes_add(void);
static void test_decode_bytes_adiw(void);
static void test_decode_bytes_and(void);
static void test_decode_bytes_andi(void);

/* --- Extern --- */

void test_decoder(void) {

    printf("\nCurrently testing: Decoder\n");

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_adc();

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_add();

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_adiw();

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_and();

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_andi();

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

