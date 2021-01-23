/* Implementation of Disassembler */

// C++ Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "disassembler/disassembler.h"
#include "disassembler/mnemonics.h"
#include "disassembler/labelmap.h"
#include "decomposer/decomposer.h"
#include "decoder/decoder.h"
#include "analyzer/report/plain.h"
#include "collections/array.h"
#include "collections/queue.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "misc/ehandling.h"

/* Forward Declarations of private Functions */

static void add_labels(array_t *buffer);

static int translate_addr(array_t *buffer, const int addr);
static char* replace_addr(const char *line, const int lx);

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

    add_labels(buffer);
    return 0;
}

/* --- Private --- */

static void add_labels(array_t *buffer) {

    lmap_t *lmap = lmap_ctor();

    for(int i = 0; i < buffer->size; i++) {

        plain_t *p = (plain_t*) array_at(buffer, i);

        const int addr = (!p->dword) ? p->addr : -1; 
        const int lx = lmap_add(lmap, p->mnem, addr);

        if(lx >= 0) {

            char *temp = p->mnem;
            p->mnem = replace_addr(p->mnem, lx);

            free(temp);
        }
    }

    buffer->size += ((lmap->size * 2) + 1);

    for(int i = 0; i < lmap->size; i++) {

        plain_t label; int offs = 0;

        const int addr = lmap_get(lmap, i, &label);
        const int index = translate_addr(buffer, addr);

        if(index < 0 || index >= buffer->size) {

            free(label.mnem);
            continue;
        }

        if(index > 0) {

            plain_t blank = {

                .opcode = 0x0000,
                .addr = -1,
                .key = 0,

                .mnem = "",
                .exec = false,
                .dword = false
            };

            array_insert(buffer, (void*) &blank, sizeof(plain_t), index);

            offs += 1;
            buffer->top += 1;
        }

        array_insert(buffer, (void*) &label, sizeof(plain_t), index + offs);
        buffer->top += 1;

        free(label.mnem);
    }

    buffer->size = buffer->top;
    lmap_dtor(lmap);
}

static int translate_addr(array_t *buffer, const int addr) {

    for(int i = 0; i < buffer->top; i++) {

        plain_t *p = (plain_t*) array_at(buffer, i);

        if(addr == p->addr)
            return i;
    }

    return -1;
}

static char* replace_addr(const char *line, const int lx) {

    const int pos = strpos(line, " ");
    const int len = strlen(line);

    char *mnemonic = substr(line, 0, pos - 1);
    char *trash = substr(line, pos + 1, len);
    char *lxstr = get_str(lx);

    const int tpos = strpos(trash, " ");
    char *comment = substr(trash, tpos + 1, len);
    char *label = strxcat(" L", lxstr);

    int total = (len - (strlen(mnemonic) + strlen(comment)));
    total -= (strlen(label) - 1);

    queue_t *stream = queue_ctor();
    queue_put(stream, 2, mnemonic, label);

    for(int i = 0; i < total - 1; i++)
        queue_put(stream, 1, " ");

    queue_put(stream, 1, comment);
    char *repl = queue_str(stream);

    queue_dtor(stream);
    nfree(5, mnemonic, trash, lxstr, comment, label);

    return repl;
}
