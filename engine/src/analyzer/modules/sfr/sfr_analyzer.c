/* SFR-Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/sfr/sfr_analyzer.h"
#include "engine/include/analyzer/report/report.h"
#include "engine/include/system/mcudef.h"

// Project Headers (shared)
#include "shared/include/misc/stringmanip.h"

/* Forward Declaration of static Functions */

static operand_t* instr_is_sfr_related(plain_t *p);
static inline bool operand_is_sfr_related(operand_t *op);

static void add_sfr_annotation(plain_t *p, const char *annotation);

/* --- Extern --- */

int analyze_sfr(report_t *report) {

    for(int i = 0; i < report->progsize; i++) {

        operand_t *op = NULL;
        plain_t *p = &report->disassembly[i];

        if((op = instr_is_sfr_related(p)) == NULL)
            continue;

        if(operand_is_sfr_related(op) == false)
            continue;

        const int offs = ((op->type != IODIRECT) * GPR_SIZE);
        add_sfr_annotation(p, sfreg[op->value - offs]);
    }

    return 0;
}

/* --- Static --- */

static operand_t* instr_is_sfr_related(plain_t *p) {

    switch(p->key) {

        case SBIS:  return &p->dest;
        case SBIC:  return &p->dest;
        case CBI:   return &p->dest;
        case SBI:   return &p->dest;
        case OUT:   return &p->dest;
        case IN:    return &p->src;

        case LDS:   return &p->src;
        case LDS32: return &p->src;

        case STS:   return &p->dest;
        case STS32: return &p->dest;

        default:    return NULL;
    }
}

static inline bool operand_is_sfr_related(operand_t *op) {

    const int offs = ((op->type == IODIRECT) * GPR_SIZE);
    return ((op->value + offs) >= SFR_START && (op->value + offs) <= SFR_END);
}

static void add_sfr_annotation(plain_t *p, const char *annotation) {

    const size_t sfr_len = strlen(annotation);
    const int pos = strpos(p->mnem, ";");

    if(pos < 0)
        return;

    const size_t total = ((pos + 2) + sfr_len);
    p->mnem = realloc(p->mnem, (total + 1) * sizeof(char));

    for(int i = pos + 2; i <= total; i++)
        p->mnem[i] = annotation[i - (pos + 2)];
}
