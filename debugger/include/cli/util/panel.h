/* Panel Header */

#ifndef PANEL_H
#define PANEL_H

// Project Headers (debugger)
#include "debugger/include/cli/util/properties.h"
#include "debugger/include/cli/debug/color.h"

typedef struct _win_st WINDOW;
typedef struct _highlighter highlighter_t;

typedef struct _panel {

    WINDOW *win;
    highlighter_t *hl;

    int page;
    int range;

    int height;
    int width;
    int y;
    int x;

} panel_t;

extern void panel_ctor(struct _panel *this, properties_t *prop);
extern void panel_dtor(struct _panel *this);

extern void panel_add(struct _panel *this, const char *str, const COLOR col);
extern void panel_write(struct _panel *this, const char *str, const COLOR col);
extern void panel_highlight(struct _panel *this, const char *str);

extern void panel_resize(struct _panel *this, properties_t *prop);
extern void panel_update(struct _panel *this);
extern void panel_clear(struct _panel *this);

extern void panel_change_page(struct _panel *this, const int offs);
extern void panel_set_page(struct _panel *this, const int at);
extern int panel_get_page(struct _panel *this);

#endif
