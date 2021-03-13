/* System Integration Test Implementation */

// C Headers
#include <stdio.h>
#include <string.h>
#include <assert.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

// Test Headers
#include "system_test.h"

#define DFL   "\033[0m"
#define GREEN "\033[32;1m"

#define PASSED printf("%s PASSED %s\n", GREEN, DFL)

/* Test Files for the dynamic analysis */

#define IHEX_KMP    "../../examples/m328p/algo/kmp.hex"
#define IHEX_ERDY   "../../examples/m328p/peripherals/eeprom/erdy.hex"
#define IHEX_EONLY  "../../examples/m328p/peripherals/eeprom/eonly.hex"
#define IHEX_DFS    "../../examples/m32/smalldragon/dfs.hex"
#define IHEX_TOV    "../../examples/m328p/peripherals/timer0/tov.hex"

/* Forward Declaration of static Functions */

static void test_ihex_kmp(vmcu_model_t *mcu);
static void test_ihex_erdy(vmcu_model_t *mcu);
static void test_ihex_eonly(vmcu_model_t *mcu);
static void test_ihex_dfs(vmcu_model_t *mcu);
static void test_ihex_tov(vmcu_model_t *mcu);

/* --- Extern --- */

void test_system(void) {

    printf("\nCurrently testing: System\n");

    /* init device models */

    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_M328P);

    /* start system tests */

    printf("    |\n");
    printf("    |---- ");

    test_ihex_kmp(m328p);

    printf("    |\n");
    printf("    |---- ");

    test_ihex_erdy(m328p);

    printf("    |\n");
    printf("    |---- ");

    test_ihex_eonly(m328p);

    printf("    |\n");
    printf("    |---- ");

    test_ihex_dfs(m328p);

    printf("    |\n");
    printf("    |---- ");

    test_ihex_tov(m328p);

    /* destroy device model */

    vmcu_model_dtor(m328p);

    /* end system tests */

    printf("\n");
}

/* --- Static --- */

static void test_ihex_kmp(vmcu_model_t *mcu) {

    printf("Simulating kmp.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_KMP, mcu);
    vmcu_system_t *sys = vmcu_system_ctor(report);

    while(vmcu_system_get_pc(sys) != 0x0141)
        vmcu_system_step(sys);

    uint64_t c = sys->cycles;

    while(sys->cycles <= (c + 3300))
        vmcu_system_step(sys);

    uint8_t *eep = vmcu_system_dump_eeprom(sys);
    assert(eep[0x0000] == 0x0a);

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    PASSED;
}

static void test_ihex_erdy(vmcu_model_t *mcu) {

    printf("Simulating erdy.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_ERDY, mcu);
    vmcu_system_t *sys = vmcu_system_ctor(report);

    while(sys->cycles < 33007)
        vmcu_system_step(sys);

    uint8_t *eep = vmcu_system_dump_eeprom(sys);

    char msg[10] = {

        eep[0], eep[1], eep[2], eep[3],
        eep[4], eep[5], eep[6], eep[7],
        eep[8], '\0'
    };

    assert(strcmp(msg, "ERDY_Test") == 0);

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    PASSED;
}

static void test_ihex_eonly(vmcu_model_t *mcu) {

    printf("Simulating eonly.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_EONLY, mcu);
    vmcu_system_t *sys = vmcu_system_ctor(report);

    while(vmcu_system_get_pc(sys) != 0x001e)
        vmcu_system_step(sys);

    uint8_t *eep = vmcu_system_dump_eeprom(sys);

    char msg[6] = {

        eep[0], eep[1], eep[2],
        eep[3], eep[4], '\0'
    };

    assert(strcmp(msg, "EEPRO") == 0);

    while(vmcu_system_get_pc(sys) != 0x0036)
        vmcu_system_step(sys);

    for(int32_t i = 0x0000; i <= 0x0004; i++)
        assert(eep[i] == 0xff);

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    PASSED;
}

static void test_ihex_dfs(vmcu_model_t *mcu) {

    printf("Simulating dfs.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_DFS, mcu);
    vmcu_system_t *sys = vmcu_system_ctor(report);

    while(vmcu_system_get_pc(sys) != 0x01c0)
        vmcu_system_step(sys);

    uint8_t *ds = vmcu_system_dump_data(sys);
    assert(ds[0x015a] == 0x0b);

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    PASSED;
}

static void test_ihex_tov(vmcu_model_t *mcu) {

    printf("Simulating tov.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_TOV, mcu);
    vmcu_system_t *sys = vmcu_system_ctor(report);

    uint8_t irqc = 0;

    const uint16_t resolution = 0x100;
    const uint8_t prescaler = 64;
    const uint8_t irq_max = 2;
    const uint8_t start = 26;

    uint32_t timeout = (resolution * prescaler * irq_max);
    timeout += start;

    while(irqc != 2) {

        if(vmcu_system_get_pc(sys) == 0x0040)
            irqc += 1;

        if(irqc == irq_max)
            break;

        assert(sys->cycles < timeout);
        vmcu_system_step(sys);
    }

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    PASSED;
}




