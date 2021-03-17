/* Driver to print Interrupt Vectors */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"

/* libvmcu Structures */

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;

/* Forward Declaration of static Functions */

static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./vector <hexfile>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_M328P);
    report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    for(int32_t i = 0; i < report->n_vector; i++) {

        vmcu_vector_t *vect = &report->vector[i];
        printf("Vector ID %d @ 0x%04x\n", vect->id, vect->addr);

        if(vect->n_xto == 0)
            continue;

        vmcu_instr_t  *isr  = vect->xto->i;
        printf(" interrupt service routine at 0x%04x\n\n", isr->addr);
    }

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);
}
