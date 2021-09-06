/* Controlflow Graph Driver for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define SEP "------------------------------------------------------------------------------------------------\n"

#define mnemlen(mnem) strlen(mnem->base) \
                    + strlen(mnem->src)  \
                    + strlen(mnem->dest)

/* libvmcu Structures */

vmcu_model_t  *m328p  = NULL;
vmcu_report_t *report = NULL;

/* Forward Declaration of static Functions */

static void print_instruction(vmcu_instr_t *instr);
static void print_instruction_details(vmcu_instr_t *instr);
static void print_colored_base(const char *basestr, VMCU_GROUP group);
static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype);
static void add_padding(const size_t length, const size_t max);
static void cleanup(void);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./skeleton <binary>\n");
        return EXIT_FAILURE;
    }

    atexit(cleanup);

    m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    report = vmcu_analyze_file(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;

    vmcu_cfg_t *cfg = report->cfg;

    for(uint32_t i = 0; i < cfg->used; i++) {

        vmcu_cfg_node_t *node = &cfg->node[i];

        printf("         ");
        print_instruction(node->xto.i);

        if(node->t != NULL) {

            printf("true  -> ");
            print_instruction(node->t->xto.i);
        }

        if(node->f != NULL) {

            printf("false -> ");
            print_instruction(node->f->xto.i);
        }

        printf(SEP);
    }

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_instruction(vmcu_instr_t *instr) {

    vmcu_mnemonic_t *mnem = &instr->mnem;

    print_instruction_details(instr);
    print_colored_base(mnem->base, instr->group);

    size_t sz = 0;

    if(instr->dest.type != VMCU_OPTYPE_NONE) {

        print_colored_operands(mnem->dest, instr->dest.type);
        sz += printf(", ");
    }

    if(instr->src.type != VMCU_OPTYPE_NONE) {

        print_colored_operands(mnem->src, instr->src.type);
        sz += printf(" ");
    }

    add_padding(mnemlen(mnem) + sz, 26);
    printf("%s%s%s\n", COLOR_RED, mnem->comment, COLOR_RESET);
}

static void print_instruction_details(vmcu_instr_t *instr) {

    printf("0x%04" PRIx32, instr->addr);

    const uint16_t opcl = (instr->opcode & 0x0000ffff);
    const uint16_t swpl = (opcl >> 8) | (opcl << 8);

    const uint16_t opch = ((instr->opcode & 0xffff0000) >> 16);
    const uint16_t swph = (opch >> 8) | (opch << 8);

    if(instr->dword == false)
        printf("      %s....%s ", COLOR_GREEN, COLOR_RESET);
    else
        printf("      %s%04" PRIx16 "%s ", COLOR_GREEN, swph, COLOR_RESET);

    printf("%s%04" PRIx16 "%s      ", COLOR_YELLOW, swpl, COLOR_RESET);
}

static void print_colored_base(const char *basestr, VMCU_GROUP group) {

    switch(group) {

        case VMCU_GROUP_NONE: printf("%s", COLOR_YELLOW);    break;
        case VMCU_GROUP_MATH_LOGIC: printf("%s", COLOR_RED); break;
        case VMCU_GROUP_SYS_CTRL: printf("%s", COLOR_CYAN);  break;
        case VMCU_GROUP_TRANSFER: printf("%s", COLOR_RESET); break;
        case VMCU_GROUP_FLOW: printf("%s", COLOR_GREEN);     break;
        case VMCU_GROUP_BIT: printf("%s", COLOR_MAGENTA);    break;

        default: break;
    }

    printf("%s%s ", basestr, COLOR_RESET);
}

static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype) {

    switch(optype) {

        case VMCU_OPTYPE_B:
        case VMCU_OPTYPE_K4:
        case VMCU_OPTYPE_K6:
        case VMCU_OPTYPE_K8:
        case VMCU_OPTYPE_IO5:
        case VMCU_OPTYPE_IO6:
        case VMCU_OPTYPE_D7:
        case VMCU_OPTYPE_D16:
        case VMCU_OPTYPE_P22:
        case VMCU_OPTYPE_S7:
        case VMCU_OPTYPE_S12:

            printf("%s", COLOR_BLUE);

        break;

        case VMCU_OPTYPE_R:
        case VMCU_OPTYPE_RP:
        case VMCU_OPTYPE_X:
        case VMCU_OPTYPE_Y:
        case VMCU_OPTYPE_Z:

            printf("%s", COLOR_CYAN);

        break;

        default: break;
    }

    printf("%s%s", opstr, COLOR_RESET);
}

static void add_padding(const size_t length, const size_t max) {

    if(length >= max)
        return;

    char pad[max - length + 1];

    memset(pad, ' ', (max - length));
    pad[max - length] = '\0';

    printf("%s", pad);
}

static void cleanup(void) {

    if(report != NULL)
        vmcu_report_dtor(report);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);
}
