/* Analyzer Implementation */

// C Headers
#include <stdlib.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/analyzer.h"
#include "engine/include/analyzer/modules/sfr/sfr_analyzer.h"
#include "engine/include/analyzer/report/report.h"

/* --- Extern --- */

report_t* analyze(const char *hex_file) {

    report_t *report = report_ctor(hex_file);
    
    /* Future calls to Analyzer Submodules */

    analyze_sfr(report);

    /* Future calls to Analyzer Submodules */
    
    return report;
}




