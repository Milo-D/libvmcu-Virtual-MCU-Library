/* dbg Header */

#ifndef DBG_H
#define DBG_H

// Project Headers (engine)
#include "engine/include/analyzer/report/report.h"
#include "engine/include/system/system.h"

// Project Headers (debugger)
#include "debugger/include/table/table.h"

typedef struct _dbg {

    report_t *report;
    table_t *table;
    system_t *sys;
    
} dbg_t;

#endif
