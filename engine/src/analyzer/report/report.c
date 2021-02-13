/* Report (Data-Structure) Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/report.h"
#include "engine/include/analyzer/report/plain.h"
#include "engine/include/disassembler/disassembler.h"

// Project Headers (shared)
#include "shared/include/collections/array.h"

struct _report* report_ctor(const char *file) {
    
    report_t *report;
    
    if((report = malloc(sizeof(report_t))) == NULL)
        return NULL;

    array_t *buffer = array_ctor(1, plain_dtor, plain_cpy);
    disassemble(file, buffer);

    report->disassembly = malloc(buffer->top * sizeof(plain_t));
    report->labels      = NULL;

    report->progsize = buffer->top;
    report->nlabels  = 0;

    for(int i = 0; i < buffer->top; i++) {
    
        plain_t *p = array_at(buffer, i);

        const size_t len = strlen(p->mnem) + 1;
        report->disassembly[i].mnem = malloc(len * sizeof(char));
        strncpy(report->disassembly[i].mnem, p->mnem, len);

        report->disassembly[i].opcode     = p->opcode;
        report->disassembly[i].addr       = p->addr;
        report->disassembly[i].key        = p->key;
        
        report->disassembly[i].exec       = p->exec;
        report->disassembly[i].dword      = p->dword;

        report->disassembly[i].src.type   = p->src.type;
        report->disassembly[i].src.value  = p->src.value;

        report->disassembly[i].dest.type  = p->dest.type;
        report->disassembly[i].dest.value = p->dest.value;
    }

    array_dtor(buffer);    
    return report; 
}

void report_dtor(struct _report *this) {
    
    for(int i = 0; i < this->progsize; i++)
        free(this->disassembly[i].mnem);

    for(int i = 0; i < this->nlabels; i++) {

        if(this->labels[i].ncallers > 0)
            free(this->labels[i].caller);
    }

    if(this->labels != NULL)
        free(this->labels);

    free(this->disassembly);
    free(this);
}
