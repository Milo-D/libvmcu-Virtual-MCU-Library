/* Syntax Highlighter Class */

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

typedef struct _panel panel_t;
struct _private;

typedef struct _highlighter {

    struct _private *p;

} highlighter_t;

extern struct _highlighter* hl_ctor(void);
extern void hl_dtor(struct _highlighter *this);

extern void hl_paint(struct _highlighter *this, panel_t *p, const char *ln);

#endif