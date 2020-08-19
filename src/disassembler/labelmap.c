/* LabelMap Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "disassembler/labelmap.h"
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
static int partition(array_t *a, const int l, const int h);

/* --- Public --- */

struct _lmap* lmap_ctor(void) {

    struct _lmap *lmap;

    if((lmap = malloc(sizeof(struct _lmap))) == NULL)
        return NULL;

    if((lmap->p = malloc(sizeof(struct _private))) == NULL) {

        free(lmap);
        return NULL;
    }

    lmap->p->labels = array_ctor(256, tuple_dtor, tuple_cpy);
    lmap->p->map = strmap_ctor(N_FLOW);

    for(int i = 0; i < N_FLOW; i++) {

        const char *instr = mnemstr[FLOW][i];

        if(strcmp(instr, "ret") == 0)
            continue;

        strmap_put(lmap->p->map, mnemstr[FLOW][i]);
    }

    lmap->size = 0;
    return lmap;
}

void lmap_dtor(struct _lmap *this) {

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

    char *label = create_label(this->size);
    const int len = strlen(label);

    tuple_t *t = tuple_ctor(2, INT, STR);

    tuple_set(t, (void*) &addr, sizeof(int), 0);
    tuple_set(t, (void*) label, (len + 1) * sizeof(char), 1);

    array_push(this->p->labels, (void*) t, sizeof(tuple_t));
    this->size += 1;

    tuple_dtor(t);
    nfree(2, sub, label);

    return this->size - 1;
}

void lmap_get(const struct _lmap *this, const int index, tuple_t *buffer) {

    if(index < 0 || index > this->size - 1) {

        char *empty = "";

        const int undef = -1;
        const int len = strlen(empty);

        tuple_set(buffer, (void*) &undef, sizeof(int), 0);
        tuple_set(buffer, (void*) empty, (len + 1) * sizeof(char), 1);

        return;
    }

    tuple_t *entry = array_at(this->p->labels, index);

    const int addr = *((int*) tuple_get(entry, 0));
    char *label = (char*) tuple_get(entry, 1);
    
    const int len = strlen(label);

    tuple_set(buffer, (void*) &addr, sizeof(int), 0);
    tuple_set(buffer, (void*) label, (len + 1) * sizeof(char), 1); 
}

extern int lmap_sort(struct _lmap *this, const int l, const int h) {

    if(l < h) {

        const int p = partition(this->p->labels, l, h);
        lmap_sort(this, l, p - 1);
        lmap_sort(this, p + 1, h);
    }
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

        tuple_t *t = (tuple_t*) array_at(labels, i);

        if(*((int*) tuple_get(t, 0)) == addr) {

            char *label = (char*) tuple_get(t, 1);

            const int pos = strpos(label, ":");
            char *sub = substr(label, 1, pos - 1);

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

static int partition(array_t *a, const int l, const int h) {

    tuple_t *piv = (tuple_t*) array_at(a, h);

    const int x = *((int*) tuple_get(piv, 0));
    int i = (l - 1);

    for(int j = l; j <= h - 1; j++) {

        tuple_t *t = (tuple_t*) array_at(a, j);
        const int y = *((int*) tuple_get(t, 0));

        if(y <= x)
            array_swap(a, ++i, j);
    }

    array_swap(a, i + 1, h);
    return (i + 1);
}