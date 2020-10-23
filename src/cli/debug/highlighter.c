/* Syntax Highlighter Implemetation */

// C++ Header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Project Headers
#include "cli/debug/highlighter.h"
#include "cli/debug/debugwindow.h"
#include "cli/debug/panel.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/strmap.h"
#include "collections/list.h"

#define CAPACITY 128

/* Forward Declaration of private Highlighter Functions */

static void init_maps(struct _highlighter *this);
static void paint_instr(struct _highlighter *this, panel_t *p, const char *instr);
static void paint_op(panel_t *p, const char *op);

static char* get_code(const char *ln);
static char* get_comment(const char *ln);

/* --- Public --- */

extern struct _highlighter* hl_ctor(void) {

    struct _highlighter *hl;

    if((hl = malloc(sizeof(struct _highlighter))) == NULL)
        return NULL;

    hl->color[FLOW] = G;
    hl->color[MISC] = D;
    hl->color[LOGIC] = M;
    hl->color[ACCESS] = D;
    hl->color[STACKOP] = B;
    hl->color[ARITHMETIC] = R;
    hl->color[DIRECTIVE] = Y;

    init_maps(hl);
    return hl;
}

extern void hl_dtor(struct _highlighter *this) {

    for(int i = 0; i < N_MAPS; i++)
        strmap_dtor(this->maps[i]);

    free(this);    
}

extern void hl_paint(struct _highlighter *this, panel_t *p, const char *ln) {

    char *code = get_code(ln);
    char *comment = get_comment(ln);

    list_t *list = ls_ctor(NULL, NULL);
    split(code, ' ', list);

    const char *first = ls_at(list, 0);

    if(first == NULL) {

        panel_add(p, comment, R);
        goto cleanup;
    }

    if(strpos(first, ":") != -1) {

        panel_add(p, ln, C);
        goto cleanup;
    }

    for(int i = 0; i < list->size; i++) {

        const char *part = ls_at(list, i);

        switch(i) {

            case 0: paint_instr(this, p, part); break;
            case 1: paint_op(p, part);          break;
            case 2: paint_op(p, part);          break;

            default: panel_add(p, ln, D);       break;
        }
    }

    panel_add(p, comment, R);

cleanup:
    free(code);
    free(comment);
    ls_dtor(list);
}

/* --- Private --- */

static void init_maps(struct _highlighter *this) {

    const int size[N_MAPS] = { N_FLOW, N_MISC, N_LOGIC, N_ACCESS, 
                               N_STACKOP, N_ARITHMETIC, N_DIRECTIVE };

    for(int i = 0; i < N_MAPS; i++) {

        this->maps[i] = strmap_ctor(CAPACITY);

        for(int j = 0; j < size[i]; j++)
            strmap_put(this->maps[i], mnemstr[i][j]);
    }
}

static void paint_instr(struct _highlighter *this, panel_t *p, const char *instr) {

    char *output = strxcat(instr, " ");

    for(int i = 0; i < N_MAPS; i++) {

        if(strmap_get(this->maps[i], instr) < 0)
            continue;

        panel_add(p, output, this->color[i]);
        free(output);

        return;
    }

    panel_add(p, output, D);
    free(output);
}

static void paint_op(panel_t *p, const char *op) {

    const int pos = strpos(op, ",");

    char *value = substr(op, 0, pos - 1);
    char *output = strxcat(op, " ");

    if(value == NULL) {

        if(htoi(op) < 0 && get_int(op) < 0) {

            panel_add(p, output, C);
            free(output);

            return;
        }

        panel_add(p, output, B);
        free(output);

        return;
    }

    if(htoi(value) < 0 && get_int(value) < 0) {

        panel_add(p, output, C);
        nfree(2, value, output);

        return;
    }

    panel_add(p, output, B);
    nfree(2, value, output);
}

static char* get_code(const char *ln) {

    const int len = strlen(ln); int pos;

    if((pos = strpos(ln, "  ")) < 0) {

        char *err = malloc((len + 1) * sizeof(char));
        strncpy(err, ln, len);
        err[len] = '\0';

        return err;
    }

    return substr(ln, 0, pos);
}

static char* get_comment(const char *ln) {

    const int len = strlen(ln); int pos;

    if((pos = strpos(ln, "  ")) < 0) {

        char *err = malloc((len + 1) * sizeof(char));
        strncpy(err, ln, len);
        err[len] = '\0';

        return err;
    }

    return substr(ln, pos + 1, len);
}




