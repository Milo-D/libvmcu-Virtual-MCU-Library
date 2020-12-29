/* Report (Data-Structure) Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "analyzer/report/report.h"
#include "analyzer/report/plain.h"

struct _report* report_ctor(const int progsize) {
    
    report_t *report;
    
    if((report = malloc(sizeof(report_t))) == NULL)
        return NULL;
    
    report->progsize = progsize;
    report->plain = malloc(progsize * sizeof(plain_t));
    
    return report; 
}
