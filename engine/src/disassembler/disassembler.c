/* Implementation of Disassembler */

// C++ Headers
#include <stdlib.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/disassembler/disassembler.h"
#include "engine/include/disassembler/mnemonics.h"
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/analyzer/report/plain.h"

// Project Headers (shared)
#include "shared/include/collections/array.h"

/* --- Public --- */

int disassemble(const char *hex_file, array_t *buffer) {
    
    if(decompose(hex_file, buffer) < 0)
        return -1;

    for(int i = 0; i < buffer->size; i++) {

        plain_t *p = (plain_t*) array_at(buffer, i);
        
        if(p->exec == false) {
            
            p->mnem = mnem_dw(p);
            continue;
        }

        p->mnem = (*mnemonics[p->key])(p);
    }

    return 0;
}
