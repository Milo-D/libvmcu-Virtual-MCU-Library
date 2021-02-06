/* Syntax Highlighter Class */

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

// Project Headers
#include "misc/mnemstr.h"

typedef struct _panel panel_t;
typedef	struct _strmap strmap_t;

typedef struct _highlighter {

    strmap_t *maps[N_MAPS];
    int color[N_MAPS];

} highlighter_t;

extern struct _highlighter* hl_ctor(void);
extern void hl_dtor(struct _highlighter *this);

extern void hl_paint(struct _highlighter *this, panel_t *p, const char *ln);

#endif