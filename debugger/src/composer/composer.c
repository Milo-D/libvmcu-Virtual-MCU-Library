/* Disassembly Composer Implementation */

// C Headers
#include <stdlib.h>
#include <inttypes.h>

// Project Headers
#include "engine/include/analyzer/report/report.h"

// Project Headers (debugger)
#include "debugger/include/composer/cdis.h"
#include "debugger/include/composer/composer.h"
#include "debugger/include/composer/label_composer.h"

composed_disassembly_t* compose_disassembly(const report_t *report) {

    composed_disassembly_t *cdis = cdis_ctor(report->progsize);

    if(cdis->size <= 0)
        return cdis;

    compose_labels(report, cdis);
    return cdis;
}







