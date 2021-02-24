/* Regression Test for the (ATmega328) */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// assert Header
#include <assert.h>

// libvmcu Headers
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

#define TESTFILE "../../driver/led/led.hex"

#define PORTB    0x0025
#define PB5      0x05

#define bit(v, b) ((v & (1 << b)) >> b)

/* libvmcu Structures */

vmcu_report_t *report = NULL;
vmcu_system_t *sys    = NULL;

/* Forward Declaration of static Functions */

static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    uint8_t led;
    atexit(cleanup);

    if((report = vmcu_analyze_ihex(TESTFILE)) == NULL)
        return EXIT_FAILURE;

    if((sys = vmcu_system_ctor(report)) == NULL)
        return EXIT_FAILURE;

    do {

        vmcu_system_step(sys);
        led = vmcu_system_read_data(sys, PORTB);

    } while(bit(led, PB5) == 0x00);

    const double f    = 16000000U;
    const double c    = sys->cycles;
    const double time = (c / f);

    assert((0.90 <= time) && (time <= 1.10));
    printf("Time between LED toggle: %lf [s]\n", time);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(sys != NULL)
        vmcu_system_dtor(sys);
}
