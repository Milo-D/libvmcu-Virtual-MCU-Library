/* dbg Header */

#ifndef DBG_H
#define DBG_H

// Project Headers
#include "analyzer/report/report.h"
#include "table/table.h"
#include "system/system.h"

typedef struct _dbg {

    report_t *report;
    table_t *table;
    system_t *sys;
    
} dbg_t;

#endif
