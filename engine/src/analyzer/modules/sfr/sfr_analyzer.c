/* SFR-Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/sfr/sfr_analyzer.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine utilities)
#include "engine/include/arch/model.h"

/* Forward Declaration of static Functions */

static vmcu_operand_t* instr_is_sfr_related(vmcu_plain_t *p);
static inline bool operand_is_sfr_related(vmcu_operand_t *op, vmcu_model_t *mcu);

static VMCU_SFREGISTER get_id_by_instr(vmcu_plain_t *p, vmcu_model_t *mcu);

static vmcu_sfr_t* get_sfrs(const int32_t *sfr_map, int32_t size, vmcu_model_t *mcu);
static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_sfr_t *sfr, vmcu_model_t *mcu);

/* --- Extern --- */

int vmcu_analyze_sfr(vmcu_report_t *report, vmcu_model_t *mcu) {

    int32_t *sfr_map = malloc(mcu->sfr.section.size * sizeof(int32_t));
    memset(sfr_map, 0, mcu->sfr.section.size * sizeof(int32_t));

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_plain_t *p = &report->disassembly[i];
        VMCU_SFREGISTER id = get_id_by_instr(p, mcu);

        if(id == VMCU_SFREGISTER_NONE)
            continue;

        if(sfr_map[id] == 0)
            report->n_sfr += 1;

        sfr_map[id] += 1;
    }

    if(report->n_sfr <= 0)
        goto cleanup;

    report->sfr = get_sfrs(sfr_map, report->n_sfr, mcu);

    for(int32_t i = 0; i < report->n_sfr; i++) {

        vmcu_sfr_t *sfr = &report->sfr[i];

        sfr->n_xref = sfr_map[sfr->id];
        sfr->xref   = get_xrefs(report, sfr, mcu);
    }

cleanup:
    free(sfr_map);
    return 0;
}

/* --- Static --- */

static vmcu_operand_t* instr_is_sfr_related(vmcu_plain_t *p) {

    switch(p->key) {

        case VMCU_SBIS:  return &p->dest;
        case VMCU_SBIC:  return &p->dest;
        case VMCU_CBI:   return &p->dest;
        case VMCU_SBI:   return &p->dest;
        case VMCU_OUT:   return &p->dest;
        case VMCU_IN:    return &p->src;

        case VMCU_LDS:   return &p->src;
        case VMCU_LDS32: return &p->src;

        case VMCU_STS:   return &p->dest;
        case VMCU_STS32: return &p->dest;

        default:         return NULL;
    }
}

static inline bool operand_is_sfr_related(vmcu_operand_t *op, vmcu_model_t *mcu) {

    const uint32_t section_start = mcu->sfr.section.start;
    const uint32_t section_end   = mcu->sfr.section.end;

    const int32_t offs = ((op->type == VMCU_IODIRECT) * section_start);
    return ((op->value + offs) >= section_start && (op->value + offs) <= section_end);
}

static VMCU_SFREGISTER get_id_by_instr(vmcu_plain_t *p, vmcu_model_t *mcu) {

    vmcu_operand_t *op;

    if((op = instr_is_sfr_related(p)) == NULL)
        return VMCU_SFREGISTER_NONE;

    if(operand_is_sfr_related(op, mcu) == false)
        return VMCU_SFREGISTER_NONE;

    int32_t offs = ((op->type != VMCU_IODIRECT) * mcu->sfr.section.start);
    return mcu->sfr.layout[op->value - offs];
}

static vmcu_sfr_t* get_sfrs(const int32_t *sfr_map, int32_t size, vmcu_model_t *mcu) {

    vmcu_sfr_t *sfrs = malloc(size * sizeof(vmcu_sfr_t));

    for(int32_t i = 0, j = 0; i < mcu->sfr.section.size; i++) {

        if(sfr_map[i] == 0)
            continue;

        sfrs[j++] = (vmcu_sfr_t) {

            .id     = i,
            .n_xref = 0,
            .xref   = NULL
        };
    }

    return sfrs;
}

static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_sfr_t *sfr, vmcu_model_t *mcu) {

    vmcu_xref_t *xrefs = malloc(sfr->n_xref * sizeof(vmcu_xref_t));

    for(int32_t i = 0, j = 0; i < report->progsize; i++) {

        vmcu_plain_t *p = &report->disassembly[i];

        if(get_id_by_instr(p, mcu) != sfr->id)
            continue;

        xrefs[j++].p = p;
    }

    return xrefs;
}

