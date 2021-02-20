/* Report (Data-Structure) Header */

#ifndef VMCU_REPORT_H
#define VMCU_REPORT_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/plain.h"
#include "engine/include/analyzer/report/label.h"

typedef struct vmcu_report {
    
    int32_t progsize;
    vmcu_plain_t *disassembly;

    int32_t nlabels;
    vmcu_label_t *labels;

} vmcu_report_t;

extern vmcu_report_t* vmcu_report_ctor(const char *file);
extern void vmcu_report_dtor(vmcu_report_t *this);

#endif
