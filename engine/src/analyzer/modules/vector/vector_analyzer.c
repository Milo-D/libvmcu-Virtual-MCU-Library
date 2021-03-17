/* Vector Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/vector/vector_analyzer.h"
#include "engine/include/analyzer/report/report.h"
#include "engine/include/analyzer/util/util.h"

// Project Headers (engine utilities)
#include "engine/include/arch/model.h"
#include "engine/include/arch/enum/vect.h"

/*
 * For 16-bit vectors: An instruction i is a vector v if and only if
 *
 * -----------------------------------------------------------------------------------------------------
 * [1] (vtable.dfl.start <= i.addr <= vtable.dfl.end)
 * [2] (i.key = RJMP) or (i.key = JMP)
 * -----------------------------------------------------------------------------------------------------
 *
 * If vtable.dfl.vector_32bit = true, an extra condition has to be added to the checklist
 *
 * -----------------------------------------------------------------------------------------------------
 * [3] (i.addr - vtable.dfl.start) mod 2 = 0
 * -----------------------------------------------------------------------------------------------------
 *
 * Condition [3] makes sure that instruction i is not in between two vectors. For example,
 * given an ATmega328P, an instruction i and two vectors v0, v1 with v0 = VECT_ADC (0x002a) and
 * v1 = VECT_EE_READY (0x002c) and i.addr = 0x002b.
 *
 * Now condition [1] is met, because (vtable.dfl.start = 0x0000) <= 0x002a <= (vtable.dfl.end = 0x0032).
 * And if we choose i.key = RJMP, condition [2] is met, too. That would be a false positive for a
 * vector.
 *
 * Condition [3] shows us that i is not a vector:  (i.addr - vtable.dfl.start) mod 2
 *                                               = (0x002b - 0x0000) mod 2
 *                                               = 1
 *
 * Condition [3] is not relevant for 16-bit vector size, because the flash is addressed in
 * words (16-bit). So there is no chance that an instruction lies in between two vectors.
 *
 * Summary: These 3 conditions are enough to find vectors in a default vector table.
 * Of course a jmp/rjmp at this location does not always serve as an interrupt vector.
 * But as soon as interrupts are globally enabled, these jmp's/rjmp's might become active vectors.
 *
 * We might add an extra condition to improve the vector analysis in the future:
 *
 * [4] look at SFR configuration in disassembly. Does it enable vector related peripherals ?
 *
 * */

/* Forward Declaration of static Functions */

static bool interrupts_enabled(vmcu_report_t *report);

static vmcu_vector_t* get_vectors(vmcu_report_t *report, int32_t *size, vmcu_model_t *mcu);
static VMCU_VECT get_vector_id(vmcu_instr_t *instr, vmcu_model_t *mcu);

static void set_xref(vmcu_vector_t *vect, vmcu_report_t *report, vmcu_instr_t *instr);
static inline bool instr_in_vtable(vmcu_instr_t *instr, vmcu_model_t *mcu);

/* --- Extern --- */

int vmcu_analyze_vectors(vmcu_report_t *report, vmcu_model_t *mcu) {

    if(interrupts_enabled(report) == false)
        return -1;

    report->vector = get_vectors(report, &report->n_vector, mcu);

    if(report->n_vector == 0)
        return -1;

    return 0;
}

/* --- Static --- */

static bool interrupts_enabled(vmcu_report_t *report) {

    for(int32_t i = 0; i < report->progsize; i++) {

        if(report->disassembly[i].key == VMCU_SEI)
            return true;
    }

    return false;
}

static vmcu_vector_t* get_vectors(vmcu_report_t *report, int32_t *size, vmcu_model_t *mcu) {

    vmcu_vector_t *vectors = malloc(mcu->vtable.n_vect * sizeof(vmcu_vector_t));

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];
        VMCU_VECT vector_id = get_vector_id(instr, mcu);

        if(*size == mcu->vtable.n_vect)
            return vectors;

        if(vector_id == VMCU_VECT_NONE)
            continue;

        vectors[*size] = (vmcu_vector_t) {

            .id        = vector_id,
            .addr      = instr->addr,
            .n_xto     = 0,
            .xto       = NULL
        };

        set_xref(&vectors[*size], report, instr);
        *size += 1;
    }

    if(*size <= 0) {

        free(vectors);
        return NULL;
    }

    size_t bytes = *size * sizeof(vmcu_vector_t);
    vectors = realloc(vectors, bytes);

    return vectors;
}

static VMCU_VECT get_vector_id(vmcu_instr_t *instr, vmcu_model_t *mcu) {

    if(instr_in_vtable(instr, mcu) == false)
        return VMCU_VECT_NONE;

    if(instr->key != VMCU_RJMP && instr->key != VMCU_JMP)
        return VMCU_VECT_NONE;

    if(mcu->vtable.vector_32bit == false)
        return mcu->vtable.layout[instr->addr];

    if(((instr->addr - mcu->vtable.dfl.start) & 1) == 1)
        return VMCU_VECT_NONE;

    const int32_t k = instr->addr - mcu->vtable.dfl.start;
    return mcu->vtable.layout[k / 2];
}

static void set_xref(vmcu_vector_t *vect, vmcu_report_t *report, vmcu_instr_t *instr) {

    const int32_t isr_addr = vmcu_get_abs_addr(instr);

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *current = &report->disassembly[i];

        if(current->addr != isr_addr)
            continue;

        vect->xto    = malloc(sizeof(vmcu_xref_t));
        vect->xto->i = current;
        vect->n_xto  = 1;

        return;
    }
}

static inline bool instr_in_vtable(vmcu_instr_t *instr, vmcu_model_t *mcu) {

    return (instr->addr >= mcu->vtable.dfl.start) && (instr->addr <= mcu->vtable.dfl.end);
}

