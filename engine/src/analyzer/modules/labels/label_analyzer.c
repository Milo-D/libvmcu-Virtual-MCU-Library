/* Label Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/labels/label_analyzer.h"
#include "engine/include/analyzer/report/report.h"
#include "engine/include/analyzer/util/util.h"

#define NXREF 10

/* Forward Declaration of static Functions */

static uint16_t* preprocess_labels(const vmcu_report_t *report, int32_t *size);
static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_label_t *lx, int32_t *size);

static bool is_branch(const VMCU_IKEY key);
static bool in_disasm(const vmcu_report_t *report, const int addr);
static bool label_exists(const uint16_t *field, const uint16_t addr, const int k);

static int cmp_u16(const void *a, const void *b);

/* --- Extern --- */

int vmcu_analyze_labels(vmcu_report_t *report) {

    uint16_t *field = preprocess_labels(report, &report->n_label);
    qsort(field, report->n_label, sizeof(uint16_t), cmp_u16);

    if(report->n_label <= 0)
        goto cleanup;

    size_t bytes = report->n_label * sizeof(vmcu_label_t);
    report->label = malloc(bytes);

    for(int i = 0; i < report->n_label; i++) {

        vmcu_label_t *lx = &report->label[i];

        lx->id       = i;
        lx->addr     = field[i];
        lx->n_xfrom  = 0;

        int32_t *size = &lx->n_xfrom;
        lx->xfrom = get_xrefs(report, lx, size);
    }

cleanup:
    free(field);
    return 0;
}

/* --- Static --- */

static uint16_t* preprocess_labels(const vmcu_report_t *report, int32_t *size) {

    int addr;

    const int32_t psize = report->progsize;
    uint16_t *field = malloc(psize * sizeof(uint16_t));

    for(int i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];

        if(is_branch(instr->key) == false)
            continue;

        if((addr = vmcu_get_abs_addr(instr)) < 0)
            continue;

        if(in_disasm(report, addr) == false)
            continue;

        if(label_exists(field, addr, *size))
            continue;

        field[*size] = addr;
        *size += 1;
    }

    return field;
}

static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_label_t *lx, int32_t *size) {

    int32_t nc = NXREF;
    vmcu_xref_t *xrefs = malloc(nc * sizeof(vmcu_xref_t));

    for(int i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];

        if(is_branch(instr->key) == false)
            continue;

        if(lx->addr != vmcu_get_abs_addr(instr))
            continue;

        if(*size >= nc) {

            size_t bytes = (nc * 2) * sizeof(vmcu_xref_t);
            xrefs = realloc(xrefs, bytes);

            nc *= 2;
        }

        xrefs[*size].i = instr;
        *size += 1;
    }

    if(*size == 0) {

        free(xrefs);
        return NULL;
    }

    size_t bytes = *size * sizeof(vmcu_xref_t);
    xrefs = realloc(xrefs, bytes);

    return xrefs;
}

static bool is_branch(const VMCU_IKEY key) {

    switch(key) {

        case VMCU_IKEY_RJMP:  return true;
        case VMCU_IKEY_JMP:   return true;

        case VMCU_IKEY_RCALL: return true;
        case VMCU_IKEY_CALL:  return true;

        case VMCU_IKEY_BRNE:  return true;
        case VMCU_IKEY_BREQ:  return true;

        case VMCU_IKEY_BRGE:  return true;
        case VMCU_IKEY_BRLO:  return true;

        case VMCU_IKEY_BRPL:  return true;
        case VMCU_IKEY_BRMI:  return true;

        case VMCU_IKEY_BRVS:  return true;
        case VMCU_IKEY_BRVC:  return true;

        case VMCU_IKEY_BRTS:  return true;
        case VMCU_IKEY_BRTC:  return true;

        case VMCU_IKEY_BRHS:  return true;
        case VMCU_IKEY_BRHC:  return true;

        case VMCU_IKEY_BRIE:  return true;
        case VMCU_IKEY_BRID:  return true;

        case VMCU_IKEY_BRLT:  return true;
        case VMCU_IKEY_BRCC:  return true;

        default:                    break;
    }

    return false;
}

static bool in_disasm(const vmcu_report_t *report, const int addr) {

    for(int i = 0; i < report->progsize; i++) {

        if(addr == report->disassembly[i].addr)
            return true;
    }

    return false;
}

static bool label_exists(const uint16_t *field, const uint16_t addr, const int k) {

    for(int i = 0; i < k; i++) {

        if(field[i] == addr)
            return true;
    }

    return false;
}

static int cmp_u16(const void *a, const void *b) {

    return (*((uint16_t*) a) - *((uint16_t*) b));
}






