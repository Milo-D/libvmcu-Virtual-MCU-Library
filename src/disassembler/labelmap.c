/* LabelMap Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "disassembler/labelmap.h"
#include "disassembler/plain.h"
#include "collections/array.h"
#include "collections/tuple.h"
#include "collections/strmap.h"
#include "collections/queue.h"
#include "misc/stringmanip.h"
#include "misc/mnemstr.h"
#include "misc/memmanip.h"

struct _private {

    array_t *labels;
    strmap_t *map;
};

/* Forward Declaration of private Functions */

static int find_addr(const char *ln);
static int label_exists(array_t *labels, const int addr);
static char* create_label(const int lno);

/* --- Public --- */

struct _lmap* lmap_ctor(void) {

    struct _lmap *lmap;

    if((lmap = malloc(sizeof(struct _lmap))) == NULL)
        return NULL;

    if((lmap->p = malloc(sizeof(struct _private))) == NULL) {

        free(lmap);
        return NULL;
    }

    lmap->p->labels = array_ctor(256, NULL, NULL);
    lmap->p->map = strmap_ctor(N_FLOW);

    for(int i = 0; i < N_FLOW - 4; i++) {

        /* (N_FLOW - 4) excludes indirect jumps */

        const char *instr = mnemstr[FLOW][i];
        strmap_put(lmap->p->map, mnemstr[FLOW][i]);
    }

    lmap->size = 0;
    return lmap;
}

void lmap_dtor(struct _lmap *this) {

    for(int i = 0; i < this->p->labels->top; i++) {

        plain_t *p = (plain_t*) array_at(this->p->labels, i);
        free(p->mnem);
    }

    array_dtor(this->p->labels);
    strmap_dtor(this->p->map);

    free(this->p);
    free(this);
}

int lmap_add(struct _lmap *this, const char *ln, const int i) {

    int pos, addr, lx;

    if((pos = strpos(ln, " ")) < 0)
        return -1;

    char *sub = substr(ln, 0, pos - 1);

    if(strmap_get(this->p->map, sub) < 0) {

        free(sub);
        return -1;
    }

    addr = find_addr(ln) + i + 1;

    if((lx = label_exists(this->p->labels, addr)) >= 0) {

        free(sub);
        return lx;
    }

    if(this->size + 1 > this->p->labels->size)
        this->p->labels->size *= 2;

    plain_t label = {

        .opcode = 0x0000,
        .addr = addr,
        .key = 0,

        .exec = false,
        .dword = false
    };

    label.mnem = create_label(this->size);

    array_push(this->p->labels, (void*) &label, sizeof(plain_t));
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

    plain_t *p = array_at(this->p->labels, index);

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
