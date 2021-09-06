/* Read-Write Access Driver for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// libvmcu
#include "libvmcu_analyzer.h"

/* libvmcu Structures */

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;

/* Forward Declaration of static Functions */

static void print_instruction(const vmcu_instr_t *instr);
static void print_rw_access(const vmcu_access_t *access);
static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./rwaccess <binary>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    report = vmcu_analyze_file(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    for(uint32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];
        print_instruction(instr);

        printf("implicit/explicit writes: ");
        print_rw_access(&instr->writes);

        printf("implicit/explicit reads: ");
        print_rw_access(&instr->reads);

        printf("\n");
    }

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_instruction(const vmcu_instr_t *instr) {

    printf("%s",  instr->mnem.base);

    if(instr->dest.type != VMCU_OPTYPE_NONE)
        printf(" %s,", instr->mnem.dest);

    if(instr->src.type != VMCU_OPTYPE_NONE)
        printf(" %s", instr->mnem.src);

    printf(" %s\n", instr->mnem.comment);
}

static void print_rw_access(const vmcu_access_t *access) {

    if(access->registers == true)
        printf("registers ");

    if(access->flash == true)
        printf("flash ");

    if(access->stack == true)
        printf("stack ");

    if(access->io == true)
        printf("i/o ");

    if(access->ds == true)
        printf("dseg ");

    if(access->sp == true)
        printf("SP ");

    if(access->pc == true)
        printf("PC ");

    if(access->c_flag == true)
        printf("CF ");

    if(access->z_flag == true)
        printf("ZF ");

    if(access->n_flag == true)
        printf("NF ");

    if(access->v_flag == true)
        printf("VF ");

    if(access->s_flag == true)
        printf("SF ");

    if(access->h_flag == true)
        printf("HF ");

    if(access->t_flag == true)
        printf("TF ");

    if(access->i_flag == true)
        printf("IF ");

    printf("\n");
}

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);
}
