/* Find groups of instructions in disassembly */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"

#define USAGE "Usage: ./findgroup <file.hex> <group>\n"

/*
 * Accepted group argument:
 *
 * - math
 * - flow
 * - sysctrl
 * - transfer
 * - bit
 *
 * Any other argument will filter
 * by instr.group = VMCU_GROUP_NONE
 * */

/* libvmcu Structures */

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;

/* Forward Declaration of static Functions */

static VMCU_GROUP get_filter(const char *arg);
static void print_instruction(const vmcu_instr_t *instr);
static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 3) {

        printf("Usage: ./findgroup <file.hex> <group>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    VMCU_GROUP filter = get_filter(argv[2]);

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];

        if(instr->group != filter)
            continue;

        printf("0x%04x\t", instr->addr);
        print_instruction(instr);
    }

    return EXIT_SUCCESS;
}

/* --- Static --- */

static VMCU_GROUP get_filter(const char *arg) {

    if(strcmp(arg, "math") == 0)
        return VMCU_GROUP_MATH_LOGIC;

    if(strcmp(arg, "flow") == 0)
        return VMCU_GROUP_FLOW;

    if(strcmp(arg, "sysctrl") == 0)
        return VMCU_GROUP_SYS_CTRL;

    if(strcmp(arg, "transfer") == 0)
        return VMCU_GROUP_TRANSFER;

    if(strcmp(arg, "bit") == 0)
        return VMCU_GROUP_BIT;

    return VMCU_GROUP_NONE;
}

void print_instruction(const vmcu_instr_t *instr) {

    printf("%s",  instr->mnem.base);

    if(instr->dest.type != VMCU_OPTYPE_NONE)
        printf(" %s,", instr->mnem.dest);

    if(instr->src.type != VMCU_OPTYPE_NONE)
        printf(" %s", instr->mnem.src);

    printf(" %s\n", instr->mnem.comment);
}

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);
}
