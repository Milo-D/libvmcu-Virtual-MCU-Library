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

static vmcu_operand_t* instr_is_sfr_related(vmcu_instr_t *instr);
static inline bool operand_is_sfr_related(vmcu_operand_t *op, vmcu_model_t *mcu);

static VMCU_SFR get_id_by_instr(vmcu_instr_t *instr, vmcu_model_t *mcu);

static vmcu_sfr_t* get_sfrs(const uint32_t *sfr_map, uint32_t size, vmcu_model_t *mcu);
static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_sfr_t *sfr, vmcu_model_t *mcu);

/* --- Extern --- */

int vmcu_analyze_sfr(vmcu_report_t *report, vmcu_model_t *mcu) {

    uint32_t *sfr_map = malloc(mcu->sfr.section.size * sizeof(uint32_t));
    memset(sfr_map, 0, mcu->sfr.section.size * sizeof(uint32_t));

    for(uint32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];
        VMCU_SFR id = get_id_by_instr(instr, mcu);

        if(id == VMCU_SFR_NONE)
            continue;

        if(sfr_map[id] == 0)
            report->n_sfr += 1;

        sfr_map[id] += 1;
    }

    if(report->n_sfr == 0)
        goto cleanup;

    report->sfr = get_sfrs(sfr_map, report->n_sfr, mcu);

    for(uint32_t i = 0; i < report->n_sfr; i++) {

        vmcu_sfr_t *sfr = &report->sfr[i];

        sfr->n_xfrom = sfr_map[sfr->id];
        sfr->xfrom   = get_xrefs(report, sfr, mcu);
    }

cleanup:
    free(sfr_map);
    return 0;
}

/* --- Static --- */

static vmcu_operand_t* instr_is_sfr_related(vmcu_instr_t *instr) {

    switch(instr->key) {

        case VMCU_IKEY_SBIS:  return &instr->dest;
        case VMCU_IKEY_SBIC:  return &instr->dest;
        case VMCU_IKEY_CBI:   return &instr->dest;
        case VMCU_IKEY_SBI:   return &instr->dest;
        case VMCU_IKEY_OUT:   return &instr->dest;
        case VMCU_IKEY_IN:    return &instr->src;

        case VMCU_IKEY_LDS:   return &instr->src;
        case VMCU_IKEY_LDS32: return &instr->src;

        case VMCU_IKEY_STS:   return &instr->dest;
        case VMCU_IKEY_STS32: return &instr->dest;

        default:                      return NULL;
    }
}

static inline bool operand_is_sfr_related(vmcu_operand_t *op, vmcu_model_t *mcu) {

    const uint32_t section_start = mcu->sfr.section.start;
    const uint32_t section_end   = mcu->sfr.section.end;

    if((op->type == VMCU_OPTYPE_IO5) || (op->type == VMCU_OPTYPE_IO6))
        return ((op->io + section_start) >= section_start) && ((op->io + section_start) <= section_end);

    return ((op->d >= section_start) && (op->d <= section_end));
}

static VMCU_SFR get_id_by_instr(vmcu_instr_t *instr, vmcu_model_t *mcu) {

    vmcu_operand_t *op;

    if((op = instr_is_sfr_related(instr)) == NULL)
        return VMCU_SFR_NONE;

    if(operand_is_sfr_related(op, mcu) == false)
        return VMCU_SFR_NONE;

    if((op->type == VMCU_OPTYPE_D7) || (op->type == VMCU_OPTYPE_D16))
        return mcu->sfr.layout[op->d - mcu->sfr.section.start];

    return mcu->sfr.layout[op->io];
}

static vmcu_sfr_t* get_sfrs(const uint32_t *sfr_map, uint32_t size, vmcu_model_t *mcu) {

    vmcu_sfr_t *sfrs = malloc(size * sizeof(vmcu_sfr_t));

    for(uint32_t i = 0, j = 0; i < mcu->sfr.section.size; i++) {

        if(sfr_map[i] == 0)
            continue;

        sfrs[j++] = (vmcu_sfr_t) {

            .id      = i,
            .n_xfrom = 0,
            .xfrom   = NULL
        };
    }

    return sfrs;
}

static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_sfr_t *sfr, vmcu_model_t *mcu) {

    vmcu_xref_t *xrefs = malloc(sfr->n_xfrom * sizeof(vmcu_xref_t));

    for(uint32_t i = 0, j = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];

        if(get_id_by_instr(instr, mcu) != sfr->id)
            continue;

        xrefs[j++].i = instr;
    }

    return xrefs;
}

