/* Analyzer Implementation */

// Project Headers (engine)
#include "engine/include/analyzer/analyzer.h"
#include "engine/include/analyzer/modules/sfr/sfr_analyzer.h"
#include "engine/include/analyzer/modules/labels/label_analyzer.h"
#include "engine/include/analyzer/report/report.h"

/* --- Extern --- */

report_t* analyze(const char *hex_file) {

    report_t *report = report_ctor(hex_file);

    analyze_labels(report);
    analyze_sfr(report);
    
    return report;
}





