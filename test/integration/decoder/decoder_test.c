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

static void test_decode_ihex_no_file(void);

static void test_decode_bytes_ffcf(void);
static void test_decode_bytes_0000(void);

/* --- Extern --- */

void test_decoder(void) {

    printf("\nCurrently testing: Decoder\n");

    printf("|\n");
    printf("|---- ");

    test_decode_ihex_no_file();

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_ffcf();

    printf("|\n");
    printf("|---- ");

    test_decode_bytes_0000();

    printf("\n");
}

/* --- Static --- */

static void test_decode_ihex_no_file(void) {

    printf("vmcu_decode_ihex() - nofile");

    int32_t size = 0;
    const char *f = " ";

    vmcu_plain_t *buffer = vmcu_decode_ihex(f, &size);
    assert(buffer == NULL);

    PASSED;
}

static void test_decode_bytes_ffcf(void) {

    printf("vmcu_decode_bytes() - 0xffcf");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0xffcf, &instr);

    assert(instr.key    == VMCU_RJMP);
    assert(instr.opcode == 0xcfff);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

static void test_decode_bytes_0000(void) {

    printf("vmcu_decode_bytes() - 0x0000");

    vmcu_plain_t instr;
    vmcu_decode_bytes(0x0000, &instr);

    assert(instr.key    == VMCU_NOP);
    assert(instr.opcode == 0x0000);
    assert(instr.addr   == 0x0000);
    assert(instr.exec   == true);
    assert(instr.dword  == false);

    PASSED;
}

