/* Report (Data-Structure) Header */

#ifndef REPORT_H
#define REPORT_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/plain.h"
#include "engine/include/analyzer/report/label.h"

typedef struct _report {
    
    int32_t progsize;
    plain_t *disassembly;

    int32_t nlabels;
    label_t *labels;

} report_t;

extern struct _report* report_ctor(const char *file);
extern void report_dtor(struct _report *this);

#endif
