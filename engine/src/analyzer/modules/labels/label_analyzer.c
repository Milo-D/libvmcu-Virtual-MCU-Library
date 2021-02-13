/* Label Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/labels/label_analyzer.h"
#include "engine/include/analyzer/report/report.h"

#define NCALL 10

/* Forward Declaration of static Functions */

static uint16_t* preprocess_labels(const report_t *report, int32_t *size);
static caller_t* get_callers(const report_t *report, label_t *lx, int32_t *size);

static bool is_branch(const IKEY key);
static bool in_disasm(const report_t *report, const int addr);
static bool label_exists(const uint16_t *field, const uint16_t addr, const int k);

static int get_abs_addr(const plain_t *p);
static int cmp_u16(const void *a, const void *b);

/* --- Extern --- */

int analyze_labels(report_t *report) {

    uint16_t *field = preprocess_labels(report, &report->nlabels);
    qsort(field, report->nlabels, sizeof(uint16_t), cmp_u16);

    report->labels = malloc(report->nlabels * sizeof(label_t));

    for(int i = 0; i < report->nlabels; i++) {

        label_t *lx = &report->labels[i];

        lx->id       = i;
        lx->addr     = field[i];
        lx->ncallers = 0;

        int32_t *size = &lx->ncallers;
        lx->caller = get_callers(report, lx, size);
    }

    free(field);
    return 0;
}

/* --- Static --- */

static uint16_t* preprocess_labels(const report_t *report, int32_t *size) {

    int addr;

    const int32_t psize = report->progsize;
    uint16_t *field = malloc(psize * sizeof(uint16_t));

    for(int i = 0; i < report->progsize; i++) {

        plain_t *p = &report->disassembly[i];

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

static caller_t* get_callers(const report_t *report, label_t *lx, int32_t *size) {

    int32_t nc = NCALL;
    caller_t *callers = malloc(nc * sizeof(caller_t));

    for(int i = 0; i < report->progsize; i++) {

        plain_t *p = &report->disassembly[i];

        if(is_branch(p->key) == false)
            continue;

        if(lx->addr != get_abs_addr(p))
            continue;

        if(*size >= nc) {

            size_t bytes = (nc * 2) * sizeof(caller_t);
            callers = realloc(callers, bytes);

            nc *= 2;
        }

        callers[*size].addr = p->addr;
        *size += 1;
    }

    if(*size == 0) {

        free(callers);
        return NULL;
    }

    size_t bytes = *size * sizeof(caller_t);
    callers = realloc(callers, bytes);

    return callers;
}

static bool is_branch(const IKEY key) {

    switch(key) {

        case RJMP:  return true;
        case JMP:   return true;

        case RCALL: return true;
        case CALL:  return true;

        case BRNE:  return true;
        case BREQ:  return true;

        case BRGE:  return true;
        case BRLO:  return true;

        case BRPL:  return true;
        case BRMI:  return true;

        case BRCC:  return true;
        case BRCS:  return true;

        case BRVS:  return true;
        case BRVC:  return true;

        case BRTS:  return true;
        case BRTC:  return true;

        case BRHS:  return true;
        case BRHC:  return true;

        case BRIE:  return true;
        case BRID:  return true;

        case BRLT:  return true;

        default:          break;
    }

    return false;
}

static bool in_disasm(const report_t *report, const int addr) {

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

static int get_abs_addr(const plain_t *p) {

    if(p->key == CALL || p->key == JMP)
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







