/* Analyzer Implementation */

// C Headers
#include <string.h>

// Project Headers (engine)
#include "engine/include/analyzer/analyzer.h"
#include "engine/include/analyzer/modules/sfr/sfr_analyzer.h"
#include "engine/include/analyzer/modules/controlflow/controlflow_analyzer.h"
#include "engine/include/analyzer/modules/labels/label_analyzer.h"
#include "engine/include/analyzer/modules/vector/vector_analyzer.h"
#include "engine/include/analyzer/modules/string/string_analyzer.h"
#include "engine/include/analyzer/report/report.h"
#include "engine/include/disassembler/disassembler.h"

// Project Headers (engine utilities)
#include "engine/include/arch/model.h"

/* --- Extern --- */

vmcu_report_t* vmcu_analyze_file(const char *file, vmcu_model_t *mcu) {

    vmcu_report_t *report = vmcu_report_ctor();
    report->disassembly = vmcu_disassemble_file(file, &report->progsize, mcu);

    if(report->disassembly == NULL) {

        vmcu_report_dtor(report);
        return NULL;
    }

    /* --- architecture specific analysis --- */

    vmcu_analyze_control_flow(report, mcu);
    vmcu_analyze_sfr(report, mcu);
    vmcu_analyze_vectors(report, mcu);

    /* --- architecture unspecific analysis --- */

    vmcu_analyze_labels(report);
    vmcu_analyze_strings(report);
    
    return report;
}





