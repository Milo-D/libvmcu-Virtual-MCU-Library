/* LabelMap Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/disassembler/labelmap.h"
#include "engine/include/analyzer/report/plain.h"

// Project Headers (shared)
#include "shared/include/misc/stringmanip.h"
#include "shared/include/misc/mnemstr.h"
#include "shared/include/misc/memmanip.h"
#include "shared/include/collections/array.h"
#include "shared/include/collections/strmap.h"
#include "shared/include/collections/queue.h"

/* Forward Declaration of private Functions */

static int find_addr(const char *ln);
static int label_exists(array_t *labels, const int addr);
static char* create_label(const int lno);

/* --- Public --- */

struct _lmap* lmap_ctor(void) {

    struct _lmap *lmap;

    if((lmap = malloc(sizeof(struct _lmap))) == NULL)
        return NULL;

    lmap->labels = array_ctor(256, NULL, NULL);
    lmap->map = strmap_ctor(N_FLOW);

    for(int i = 0; i < N_FLOW - 6; i++) {

        /* (N_FLOW - 6) excludes indirect jumps */
        strmap_put(lmap->map, mnemstr[FLOW][i]);
    }

    lmap->size = 0;
    return lmap;
}

void lmap_dtor(struct _lmap *this) {

    for(int i = 0; i < this->labels->top; i++) {

        plain_t *p = (plain_t*) array_at(this->labels, i);
        free(p->mnem);
    }

    array_dtor(this->labels);
    strmap_dtor(this->map);

    free(this);
}

int lmap_add(struct _lmap *this, const char *ln, const int i) {

    int pos, addr, lx;

    if((pos = strpos(ln, " ")) < 0)
        return -1;

    char *sub = substr(ln, 0, pos - 1);

    if(strmap_get(this->map, sub) < 0) {

        free(sub);
        return -1;
    }

    addr = find_addr(ln) + i + 1;

    if((lx = label_exists(this->labels, addr)) >= 0) {

        free(sub);
        return lx;
    }

    if(this->size + 1 > this->labels->size)
        this->labels->size *= 2;

    plain_t label = {

        .opcode = 0x0000,
        .addr = addr,
        .key = 0,

        .exec = false,
        .dword = false
    };

    label.mnem = create_label(this->size);

    array_push(this->labels, (void*) &label, sizeof(plain_t));
    this->size += 1;

    free(sub);
    return this->size - 1;
}

int lmap_get(const struct _lmap *this, const int index, plain_t *buffer) {

    buffer->opcode = 0x0000;
    buffer->addr = -1;
    buffer->key = 0;

    buffer->mnem = NULL;
    buffer->exec = false;
    buffer->dword = false;

    if(index < 0 || index > this->size - 1)
        return -1;

    plain_t *p = array_at(this->labels, index);

    const int len = strlen(p->mnem);
    buffer->mnem = malloc((len + 1) * sizeof(char));

    strncpy(buffer->mnem, p->mnem, strlen(p->mnem));
    buffer->mnem[len] = '\0';

    return p->addr;
}

/* --- Private --- */

static int find_addr(const char *ln) {

    int pos; int signum = 1;

    if((pos = strpos(ln, "+")) > 7) {

        pos = strpos(ln, "-");
        signum = -1;
    }

    char *sub = substr(ln, pos + 1, strlen(ln));
    pos = strpos(sub, " ");

    char *subsub = substr(sub, 0, pos - 1);
    int addr = get_int(subsub) * signum;

    nfree(2, sub, subsub);

    return addr; 
}

static int label_exists(array_t *labels, const int addr) {

    for(int i = 0; i < labels->top; i++) {

        plain_t *p = (plain_t*) array_at(labels, i);

        if(p->addr == addr) {

            const int pos = strpos(p->mnem, ":");
            char *sub = substr(p->mnem, 1, pos - 1);

            const int lx = get_int(sub);
            free(sub);

            return lx;
        }
    }

    return -1;
}

static char* create_label(const int lno) {

    char *id = get_str(lno);

    queue_t *stream = queue_ctor();
    queue_put(stream, 3, "L", id, ":");

    char *label = queue_str(stream);

    queue_dtor(stream);
    free(id);

    return label;
}