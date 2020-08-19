/* Panel Implementation/* Panel Header */

#ifndef PANEL_H
#define PANEL_H

// Project Headers
#include "cli/color.h"

struct _private;

typedef struct _panel {

    struct _private *p;

} panel_t;

extern struct _panel* panel_ctor(int h, int w, int y, int x, int cs, int cr);
extern void panel_dtor(struct _panel *this);

extern void panel_init(struct _panel *this, int h, int w, int y, int x, int cs, int cr);
extern void panel_add(struct _panel *this, const char *str, const COLOR col);
extern void panel_write(struct _panel *this, const char *str, const COLOR col);
extern void panel_highlight(struct _panel *this, const char *str);

extern void panel_update(struct _panel *this);
extern void panel_clear(struct _panel *this);
extern void panel_mv_curs(struct _panel *this, const int offs);
extern void panel_set_curs(struct _panel *this, const int at);
extern int panel_get_curs(struct _panel *this);

extern int panel_height(struct _panel *this);
extern int panel_width(struct _panel *this);
extern int panel_y(struct _panel *this);
extern int panel_x(struct _panel *this);

extern void panel_destroy(struct _panel *this);

#endif