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

static void test_ihex_kmp(void);
static void test_ihex_erdy(void);
static void test_ihex_eonly(void);
static void test_ihex_dfs(void);
static void test_ihex_tov(void);

/* --- Extern --- */

void test_system(void) {

    printf("\nCurrently testing: System\n");

    printf("    |\n");
    printf("    |---- ");

    test_ihex_kmp();

    printf("    |\n");
    printf("    |---- ");

    test_ihex_erdy();

    printf("    |\n");
    printf("    |---- ");

    test_ihex_eonly();

    printf("    |\n");
    printf("    |---- ");

    test_ihex_dfs();

    printf("    |\n");
    printf("    |---- ");

    test_ihex_tov();

    printf("\n");
}

/* --- Static --- */

static void test_ihex_kmp(void) {

    printf("Simulating kmp.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_KMP);
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

static void test_ihex_erdy(void) {

    printf("Simulating erdy.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_ERDY);
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

static void test_ihex_eonly(void) {

    printf("Simulating eonly.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_EONLY);
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

static void test_ihex_dfs(void) {

    printf("Simulating dfs.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_DFS);
    vmcu_system_t *sys = vmcu_system_ctor(report);

    while(vmcu_system_get_pc(sys) != 0x01c0)
        vmcu_system_step(sys);

    uint8_t *ds = vmcu_system_dump_data(sys);
    assert(ds[0x015a] == 0x0b);

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    PASSED;
}

static void test_ihex_tov(void) {

    printf("Simulating tov.hex");

    vmcu_report_t *report = vmcu_analyze_ihex(IHEX_TOV);
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




