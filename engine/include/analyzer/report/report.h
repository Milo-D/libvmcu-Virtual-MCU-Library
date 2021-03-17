/* Report (Data-Structure) Header */

#ifndef VMCU_REPORT_H
#define VMCU_REPORT_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/instr.h"
#include "engine/include/analyzer/report/sfr.h"
#include "engine/include/analyzer/report/label.h"
#include "engine/include/analyzer/report/vector.h"

typedef struct vmcu_report {
    
    int32_t progsize;
    vmcu_instr_t *disassembly;

    int32_t n_sfr;
    vmcu_sfr_t *sfr;

    int32_t n_label;
    vmcu_label_t *label;

    int32_t n_vector;
    vmcu_vector_t *vector;

} vmcu_report_t;

extern vmcu_report_t* vmcu_report_ctor(void);
extern void vmcu_report_dtor(vmcu_report_t *this);

#endif
