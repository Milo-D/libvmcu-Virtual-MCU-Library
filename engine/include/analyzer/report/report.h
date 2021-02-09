/* Report (Data-Structure) Header */

#ifndef REPORT_H
#define REPORT_H

// Project Headers (engine)
#include "engine/include/analyzer/report/plain.h"

typedef struct _report {
    
    int progsize;
    plain_t *disassembly;

} report_t;

extern struct _report* report_ctor(const char *file);
extern void report_dtor(struct _report *this);

#endif
