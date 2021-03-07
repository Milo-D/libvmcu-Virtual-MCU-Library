/* Report (Data-Structure) Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/report.h"

vmcu_report_t* vmcu_report_ctor(const char *file) {
    
    vmcu_report_t *report;
    
    if((report = malloc(sizeof(vmcu_report_t))) == NULL)
        return NULL;

    report->progsize    = 0;
    report->disassembly = NULL;

    report->n_label = 0;
    report->label   = NULL;

    return report; 
}

void vmcu_report_dtor(vmcu_report_t *this) {
    
    for(int i = 0; i < this->progsize; i++) {

        if(this->disassembly[i].mnem != NULL)
            free(this->disassembly[i].mnem);
    }

    for(int i = 0; i < this->n_label; i++) {

        if(this->label[i].n_xref > 0)
            free(this->label[i].xref);
    }

    if(this->label != NULL)
        free(this->label);

    if(this->disassembly != NULL)
        free(this->disassembly);

    free(this);
}
