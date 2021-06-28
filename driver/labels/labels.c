/* Driver to print potential labels and its xrefs */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libvmcu
#include "libvmcu_analyzer.h"

/* Forward Declaration of static Functions */

static void print_instruction(const vmcu_instr_t *instr);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./labels <file.hex>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p   = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t *report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL) {

        vmcu_model_dtor(m328p);
        return EXIT_FAILURE;
    }

    for(int32_t i = 0; i < report->n_label; i++) {

        vmcu_label_t *lx = &report->label[i];
        printf("0x%04x\tL%d\n", lx->addr, lx->id);

        for(int32_t j = 0; j < lx->n_xfrom; j++) {

            vmcu_xref_t *x = &lx->xfrom[j];

            printf(" xref from 0x%04x ", x->i->addr);
            print_instruction(x->i);
        }

        printf("\n");
    }

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}

/* --- Static --- */

void print_instruction(const vmcu_instr_t *instr) {

    printf("%s",  instr->mnem.base);

    if(instr->dest.type != VMCU_OPTYPE_NONE)
        printf(" %s,", instr->mnem.dest);

    if(instr->src.type != VMCU_OPTYPE_NONE)
        printf(" %s", instr->mnem.src);

    printf(" %s\n", instr->mnem.comment);
}