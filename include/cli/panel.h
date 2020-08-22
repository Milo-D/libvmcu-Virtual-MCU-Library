/* Panel Implementation/* Panel Header */

#ifndef PANEL_H
#define PANEL_H

// Project Headers
#include "cli/color.h"

struct _private;

typedef struct _panel {

    int height;
    int width;
    int y;
    int x;

    struct _private *p;

} panel_t;

extern struct _panel* panel_ctor(int h, int w, int y, int x, int cs, int cr);
extern void panel_dtor(struct _panel *this);

extern void panel_add(struct _panel *this, const char *str, const COLOR col);
extern void panel_write(struct _panel *this, const char *str, const COLOR col);
extern void panel_highlight(struct _panel *this, const char *str);

extern void panel_resize(struct _panel *this, int h, int w, int y, int x);
extern void panel_update(struct _panel *this);
extern void panel_clear(struct _panel *this);

extern void panel_mv_curs(struct _panel *this, const int offs);
extern void panel_set_curs(struct _panel *this, const int at);
extern int panel_get_curs(struct _panel *this);

#endif