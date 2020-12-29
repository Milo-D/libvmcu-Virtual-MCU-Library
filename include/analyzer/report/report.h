/* Report (Data-Structure) Header */

#ifndef REPORT_H
#define REPORT_H

// Project Headers
#include "analyzer/report/plain.h"

typedef struct _report {
    
    int progsize;
    plain_t *disassembly;

} report_t;

extern struct _report* report_ctor(const int progsize);

#endif
