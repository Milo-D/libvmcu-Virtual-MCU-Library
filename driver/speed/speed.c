/* Driver for Execution Speed Measurement */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

/*
 * This small driver measures the simulation
 * speed in MHz. This driver is only valid for
 * the test file test/m328p/misc/delay.hex
 * */

/* --- libvmcu structures --- */

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;
vmcu_system_t *sys    = NULL;

/* Forward Declaration of static Functions */

static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./speed <binary>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    report = vmcu_analyze_file(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    sys = vmcu_system_ctor(report);

    double start, end;

    clock_t timer = clock();
    start = clock();

    while(vmcu_system_get_pc(sys) != 0x0047)
        vmcu_system_step(sys);

    end = clock() - start;

    const double mhz = (16.0 / (end / CLOCKS_PER_SEC));
    printf("Simulation speed: %lf [MHz]\n", mhz);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(sys != NULL)
        vmcu_system_dtor(sys);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);
}
