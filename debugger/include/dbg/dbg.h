/* dbg Header */

#ifndef DBG_H
#define DBG_H

// Project Headers (engine)
#include "engine/include/analyzer/report/report.h"
#include "engine/include/system/system.h"

// Project Headers (debugger)
#include "debugger/include/composer/cdis.h"
#include "debugger/include/table/table.h"

typedef struct _dbg {

    report_t *report;
    system_t *sys;

    table_t *table;
    composed_disassembly_t *cdis;

} dbg_t;

#endif
