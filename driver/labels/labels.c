/* Driver to print potential labels and its xrefs */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libvmcu
#include "libvmcu_analyzer.h"

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./labels <file.hex>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p   = vmcu_model_ctor(VMCU_M328P);
    vmcu_report_t *report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    for(int32_t i = 0; i < report->n_label; i++) {

        vmcu_label_t *lx = &report->label[i];
        printf("0x%04x\tL%d\n", lx->addr, lx->id);

        for(int32_t j = 0; j < lx->n_xref; j++) {

            vmcu_xref_t *x = &lx->xref[j];

            printf(" xref from 0x%04x ", x->i->addr);
            printf("(%s)\n", x->i->mnem);
        }

        printf("\n");
    }

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}