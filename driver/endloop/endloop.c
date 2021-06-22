/* Small Driver to find endless loops in AVR binaries */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"

/*
 * An instruction i is an endloop if,
 *
 * [1] i is a flow control instruction except ijmp, eijmp, icall,
 *     eicall, ret, reti, cpse, sbrc, sbrs, sbic, sbis
 * [2] if relative flow change d = -1 and if absolute a - d = 0,
 *     where a is the address of i and d is the destination of the
 *     flow instruction.
 *
 * An instruction i is a conditional endloop if,
 *
 * [1] instruction i is an endloop
 * [2] and i is not a jmp, rjmp, call, rcall
 *
 * */

/* Forward Declaration of static Functions */

static bool is_endloop(vmcu_instr_t *instr);
static bool is_valid_flow_change(vmcu_instr_t *instr);
static bool is_absolute_flow_change(vmcu_instr_t *instr);
static bool is_conditional(vmcu_instr_t *instr);

static void print_instruction(const vmcu_instr_t *instr);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./endloop <file.hex>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p   = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t *report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL) {

        printf("Could not analyze binary...\n");
        return EXIT_FAILURE;
    }

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];

        if(is_endloop(instr) == false)
            continue;

        if(is_conditional(instr) == true)
            printf("conditional endloop:\t");
        else
            printf("unconditional endloop:\t");

        printf("0x%04x\t", instr->addr);
        print_instruction(instr);
    }

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static bool is_endloop(vmcu_instr_t *instr) {

    if(instr->group != VMCU_GROUP_FLOW)
        return false;

    if(is_valid_flow_change(instr) == false)
        return false;

    if(is_absolute_flow_change(instr) == true) {

        if(instr->src.p == instr->addr)
            return true;

        return false;
    }

    if(instr->src.s == -1)
        return true;

    return false;
}

static bool is_valid_flow_change(vmcu_instr_t *instr) {

    switch(instr->key) {

        case VMCU_IKEY_IJMP:   return false;
        case VMCU_IKEY_EIJMP:  return false;
        case VMCU_IKEY_ICALL:  return false;
        case VMCU_IKEY_EICALL: return false;
        case VMCU_IKEY_RET:    return false;
        case VMCU_IKEY_RETI:   return false;
        case VMCU_IKEY_CPSE:   return false;
        case VMCU_IKEY_SBRC:   return false;
        case VMCU_IKEY_SBRS:   return false;
        case VMCU_IKEY_SBIC:   return false;
        case VMCU_IKEY_SBIS:   return false;

        default: break;
    }

    return true;
}

static bool is_absolute_flow_change(vmcu_instr_t *instr) {

    switch(instr->key) {

        case VMCU_IKEY_JMP:  return true;
        case VMCU_IKEY_CALL: return true;

        default: break;
    }

    return false;
}

static bool is_conditional(vmcu_instr_t *instr) {

    switch(instr->key) {

        case VMCU_IKEY_BREQ: return true;
        case VMCU_IKEY_BRNE: return true;
        case VMCU_IKEY_BRCC: return true;
        case VMCU_IKEY_BRLO: return true;
        case VMCU_IKEY_BRMI: return true;
        case VMCU_IKEY_BRPL: return true;
        case VMCU_IKEY_BRGE: return true;
        case VMCU_IKEY_BRLT: return true;
        case VMCU_IKEY_BRHS: return true;
        case VMCU_IKEY_BRHC: return true;
        case VMCU_IKEY_BRTS: return true;
        case VMCU_IKEY_BRTC: return true;
        case VMCU_IKEY_BRVS: return true;
        case VMCU_IKEY_BRVC: return true;
        case VMCU_IKEY_BRIE: return true;
        case VMCU_IKEY_BRID: return true;

        default: break;
    }

    return false;
}

void print_instruction(const vmcu_instr_t *instr) {

    printf("%s",  instr->mnem.base);

    if(instr->dest.type != VMCU_OPTYPE_NONE)
        printf(" %s,", instr->mnem.dest);

    if(instr->src.type != VMCU_OPTYPE_NONE)
        printf(" %s", instr->mnem.src);

    printf(" %s\n", instr->mnem.comment);
}
