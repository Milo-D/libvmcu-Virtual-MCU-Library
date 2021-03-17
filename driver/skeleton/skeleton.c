/* Driver Skeleton for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

/* libvmcu Structures */

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;
vmcu_system_t *sys    = NULL;

/* Forward Declaration of static Functions */

static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./skeleton <hexfile>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    sys = vmcu_system_ctor(report);

    /* Do something... */
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
