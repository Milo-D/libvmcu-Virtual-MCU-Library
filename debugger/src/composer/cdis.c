/* Composed Disassembly Implementation */

// C Headers
#include <stdlib.h>
#include <inttypes.h>

// Project Headers (debugger)
#include "debugger/include/composer/cdis.h"

struct _composed_disassembly* cdis_ctor(const int32_t size) {

    struct _composed_disassembly *cdis;

    if((cdis = malloc(sizeof(struct _composed_disassembly))) == NULL)
        return NULL;

    if(size <= 0) {

        cdis->line = NULL;
        cdis->size = 0;

        return cdis;
    }

    size_t bytes = size * sizeof(struct _composed_line);

    cdis->line = malloc(bytes);
    cdis->size = size;

    for(int i = 0; i < size; i++) {

        cdis->line[i].str  = NULL;
        cdis->line[i].addr = CDIS_NO_ADDR;
        cdis->line[i].opc  = CDIS_NO_OPC;
    }

    return cdis;
}

void cdis_dtor(struct _composed_disassembly *this) {

    for(int i = 0; i < this->size; i++) {

        if(this->line[i].str != NULL)
            free(this->line[i].str);
    }

    if(this->line != NULL)
        free(this->line);

    free(this);
}

void cdis_assign_line(struct _composed_line *this, int32_t addr, uint32_t opc, char *str) {

    this->addr = addr;
    this->str  = str;
    this->opc  = opc;
}