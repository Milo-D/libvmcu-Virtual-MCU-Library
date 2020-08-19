/* Implementation of Disassembler */

// C++ Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "disassembler/disassembler.h"
#include "disassembler/decoder.h"
#include "disassembler/mnemonics.h"
#include "disassembler/labelmap.h"
#include "collections/array.h"
#include "collections/tuple.h"
#include "collections/queue.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "misc/ehandling.h"

/* Forward Declarations of private Functions */

static void add_labels(array_t *buffer);
static char* replace_addr(const char *line, const int lx);

/* --- Public --- */

int disassemble(const char *hex_file, array_t *buffer) {

    array_t *dump = array_ctor(1024, NULL, NULL);
    decode_hex(hex_file, dump);

    buffer->size = dump->top;

    for(int i = 0; i < dump->top; i++) {

        plain_t *p = (plain_t*) array_at(dump, i);

        char *current = (*mnemonics[p->key])(p->opcode);
        const int len = strlen(current);

        tuple_t *t = tuple_ctor(2, STR, INT);
        tuple_set(t, (void*) current, (len + 1) * sizeof(char), 0);
        tuple_set(t, (void*) &p->addr, sizeof(int), 1);

        array_push(buffer, (void*) t, sizeof(tuple_t));

        tuple_dtor(t);
        free(current);
    }

    if(buffer->size == 0)
        return -1;

    add_labels(buffer);
    array_dtor(dump);

    return 0;
}

/* --- Private --- */

static void add_labels(array_t *buffer) {

    lmap_t *lmap = lmap_ctor(); int offs = 0;

    for(int i = 0; i < buffer->size; i++) {

        tuple_t *t = array_at(buffer, i);

        const char *line = tuple_get(t, 0);
        const int lx = lmap_add(lmap, line, i);

        if(lx >= 0) {

            char *repl = replace_addr(line, lx);
            const int len = strlen(repl);

            tuple_set(t, (void*) repl, (len + 1) * sizeof(char), 0);
            free(repl);
        }
    }

    lmap_sort(lmap, 0, lmap->size - 1);
    buffer->size += ((lmap->size * 2) + 1);

    for(int i = 0; i < lmap->size; i++) {

        tuple_t *t = tuple_ctor(2, INT, STR);
        lmap_get(lmap, i, t);

        const int t_addr = *((int*) tuple_get(t, 0));
        char *t_label = (char*) tuple_get(t, 1);

        if(t_addr + offs >= buffer->size || t_addr + offs < 0) {

            tuple_dtor(t);
            continue;
        }

        const int undef = -1;
        const int len = strlen(t_label);

        tuple_t *label = tuple_ctor(2, STR, INT);

        tuple_set(label, (void*) t_label, (len + 1) * sizeof(char), 0);
        tuple_set(label, (void*) &undef, sizeof(int), 1);

        if(t_addr > 0) {

            char empty[2] = "";
            tuple_t *blank = tuple_ctor(2, STR, INT);

            tuple_set(blank, (void*) empty, 2 * sizeof(char), 0);
            tuple_set(blank, (void*) &undef, sizeof(int), 1);

            array_insert(buffer, (void*) blank, sizeof(tuple_t), t_addr + offs);

            offs += 1;
            buffer->top += 1;

            tuple_dtor(blank);
        }

        array_insert(buffer, (void*) label, sizeof(tuple_t), t_addr + offs);
        
        offs += 1;
        buffer->top += 1;

        tuple_dtor(t);
        tuple_dtor(label);
    }

    buffer->size = buffer->top;
    lmap_dtor(lmap);
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
