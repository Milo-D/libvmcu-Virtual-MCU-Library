/* DebugWindow Header */

#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#define N_PANEL 7

// Project Headers
#include "cli/color.h"

/*************************************************************************

Panels:

(I) GPNL = gpr panel
(II) SPNL = sreg panel
(III) CPNL = code panel
(IV) DPNL = data panel
(V) EPNL = eeprom panel
(VI) OPNL = output panel
(VII) RPNL = right panel

**************************************************************************/

typedef enum { GPNL, SPNL, CPNL, DPNL, EPNL, OPNL, RPNL } PANEL;

typedef struct _panel panel_t;
typedef struct _prompt prompt_t;

struct _private;

typedef struct _debugwindow {

    struct _private *p;

} debugwindow_t;

extern struct _debugwindow* dwin_ctor(const int size);
extern void dwin_dtor(struct _debugwindow *this);

extern void dwin_resize(struct _debugwindow *this);

extern void dwin_read_prompt(const struct _debugwindow *this, char *buffer);
extern void dwin_add(struct _debugwindow *this, const PANEL ptype, const char *str, const COLOR col);
extern void dwin_write(struct _debugwindow *this, const PANEL ptype, const char *str, const COLOR col);
extern void dwin_highlight(struct _debugwindow *this, const PANEL ptype, const char *str);

extern void dwin_clear_panel(struct _debugwindow *this, const PANEL ptype);
extern void dwin_clear(struct _debugwindow *this);

extern void dwin_update(struct _debugwindow *this, const PANEL ptype);
extern void dwin_update_all(struct _debugwindow *this);

extern void dwin_mv_curs(struct _debugwindow *this, const PANEL ptype, const int offs);
extern void dwin_set_curs(struct _debugwindow *this, const PANEL ptype, const int at);
extern int dwin_curs_of(struct _debugwindow *this, const PANEL ptype);

extern int dwin_height(struct _debugwindow *this, const PANEL ptype);
extern int dwin_width(struct _debugwindow *this, const PANEL ptype);
extern int dwin_y(struct _debugwindow *this, const PANEL ptype);
extern int dwin_x(struct _debugwindow *this, const PANEL ptype);

#endif