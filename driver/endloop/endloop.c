/* Small Driver to find endless loops in AVR binaries */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./endloop <file.hex>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p   = vmcu_model_ctor(VMCU_M328P);
    vmcu_report_t *report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL) {

        printf("Could not analyze binary...\n");
        return EXIT_FAILURE;
    }

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_plain_t *p = &report->disassembly[i];

        if(p->opcode == 0xcfff)
            printf("Endless loop at address 0x%04x\n", p->addr);
    }

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}
