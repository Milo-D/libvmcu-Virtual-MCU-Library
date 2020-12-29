/* Analyzer Implementation */

// C Headers
#include <stdlib.h>
#include <stdbool.h>

// Project Headers
#include "analyzer/analyzer.h"
#include "analyzer/report/report.h"
#include "disassembler/disassembler.h"
#include "collections/array.h"

/* Forward Declaration of static Functions */

static report_t* init_report(const char *hex_file);

/* --- Extern --- */

report_t* analyze(const char *hex_file) {

    report_t *report = init_report(hex_file);
    
    /* Future calls to Analyzer Submodules */
    /* Future calls to Analyzer Submodules */
}

/* --- Static --- */

static report_t* init_report(const char *hex_file) {
    
    array_t *buffer = array_ctor(1, plain_dtor, plain_cpy);
    disassemble(hex_file, buffer);
    
    report_t *report = report_ctor(buffer->top);
    
    for(int i = 0; i < report->progsize; i++) {
    
        plain_t *p = array_at(buffer, i);

        report->disassembly[i].opcode = p->opcode;
        report->disassembly[i].addr   = p->addr;
        report->disassembly[i].key    = p->key;
        
        report->disassembly[i].mnem   = p->mnem;
        report->disassembly[i].exec   = p->exec;
        report->disassembly[i].dword  = p->dword;
    }

    array_dtor(buffer);
    return report;
}





