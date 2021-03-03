/* Label Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/labels/label_analyzer.h"
#include "engine/include/analyzer/report/report.h"

#define NXREF 10

/* Forward Declaration of static Functions */

static uint16_t* preprocess_labels(const vmcu_report_t *report, int32_t *size);
static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_label_t *lx, int32_t *size);

static bool is_branch(const VMCU_IKEY key);
static bool in_disasm(const vmcu_report_t *report, const int addr);
static bool label_exists(const uint16_t *field, const uint16_t addr, const int k);

static int get_abs_addr(const vmcu_plain_t *p);
static int cmp_u16(const void *a, const void *b);

/* --- Extern --- */

int vmcu_analyze_labels(vmcu_report_t *report) {

    uint16_t *field = preprocess_labels(report, &report->nlabels);
    qsort(field, report->nlabels, sizeof(uint16_t), cmp_u16);

    size_t bytes = report->nlabels * sizeof(vmcu_label_t);
    report->labels = malloc(bytes);

    for(int i = 0; i < report->nlabels; i++) {

        vmcu_label_t *lx = &report->labels[i];

        lx->id       = i;
        lx->addr     = field[i];
        lx->nxrefs   = 0;

        int32_t *size = &lx->nxrefs;
        lx->xrefs = get_xrefs(report, lx, size);
    }

    free(field);
    return 0;
}

/* --- Static --- */

static uint16_t* preprocess_labels(const vmcu_report_t *report, int32_t *size) {

    int addr;

    const int32_t psize = report->progsize;
    uint16_t *field = malloc(psize * sizeof(uint16_t));

    for(int i = 0; i < report->progsize; i++) {

        vmcu_plain_t *p = &report->disassembly[i];

        if(is_branch(p->key) == false)
            continue;

        if((addr = get_abs_addr(p)) < 0)
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

        vmcu_plain_t *p = &report->disassembly[i];

        if(is_branch(p->key) == false)
            continue;

        if(lx->addr != get_abs_addr(p))
            continue;

        if(*size >= nc) {

            size_t bytes = (nc * 2) * sizeof(vmcu_xref_t);
            xrefs = realloc(xrefs, bytes);

            nc *= 2;
        }

        xrefs[*size].p = p;
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

        case VMCU_RJMP:  return true;
        case VMCU_JMP:   return true;

        case VMCU_RCALL: return true;
        case VMCU_CALL:  return true;

        case VMCU_BRNE:  return true;
        case VMCU_BREQ:  return true;

        case VMCU_BRGE:  return true;
        case VMCU_BRLO:  return true;

        case VMCU_BRPL:  return true;
        case VMCU_BRMI:  return true;

        case VMCU_BRCC:  return true;
        case VMCU_BRCS:  return true;

        case VMCU_BRVS:  return true;
        case VMCU_BRVC:  return true;

        case VMCU_BRTS:  return true;
        case VMCU_BRTC:  return true;

        case VMCU_BRHS:  return true;
        case VMCU_BRHC:  return true;

        case VMCU_BRIE:  return true;
        case VMCU_BRID:  return true;

        case VMCU_BRLT:  return true;

        default:          break;
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

static int get_abs_addr(const vmcu_plain_t *p) {

    if(p->key == VMCU_CALL || p->key == VMCU_JMP)
        return p->src.value;

    if(p->src.value < 0) {

        if((p->src.value * -1) > p->addr)
            return -1;
    }

    return (p->addr + p->src.value + 1);
}

static int cmp_u16(const void *a, const void *b) {

    return (*((uint16_t*) a) - *((uint16_t*) b));
}







