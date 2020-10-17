/* DebugWindow Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/debugwindow.h"
#include "cli/debug/panel.h"
#include "cli/debug/prompt.h"
#include "system/mcudef.h"

/* Forward Declaration of static Functions */

static void dwin_init(struct _debugwindow *this);

/* --- Extern --- */

struct _debugwindow* dwin_ctor(const int size) {

    struct _debugwindow *dwin;

    if((dwin = malloc(sizeof(struct _debugwindow))) == NULL)
        return NULL;

    dwin->panel = malloc(N_PANEL * sizeof(panel_t*));
    dwin_init(dwin);

    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int mmx = (mx - (mx / 2));
    const int gy = (scr_y - 40);

    const int dim[N_PANEL][4] = {

        { 6, mx, 0, 0                     },
        { 6, mx, 6, 0                     },
        { 13, mx, 12, 0                   },
        { 13, (mx / 2), 25, 0             },  
        { 13, mmx, 25, (mx / 2)           }, 
        { (gy - 1), mx, 38, 0             },
        { (scr_y - 3), (scr_x / 2), 0, mx }
    };

    int n_pages = (size / (scr_y - 7));
    n_pages += ((size % (scr_y - 7)) != 0);

    const int cs[N_PANEL] = { 0, 0, 0, 0x0060, (EEPROM_SIZE / 2), 
                              0, 0 };

    const int cr[N_PANEL] = { (GPR_SIZE / 8), 0, 0, (RAM_END + 1), 
                              (EEPROM_SIZE), 0, n_pages };

    for(int i = 0; i < N_PANEL; i++) {

        int h = dim[i][0]; int w = dim[i][1];
        int y = dim[i][2]; int x = dim[i][3];

        dwin->panel[i] = panel_ctor(h, w, y, x, cs[i], cr[i]);
    }

    dwin->prompt = prompt_ctor(3, scr_x, (38 + gy - 1), 0);
    return dwin;
}

void dwin_dtor(struct _debugwindow *this) {

    for(int i = 0; i < N_PANEL; i++)
        panel_dtor(this->panel[i]);

    prompt_dtor(this->prompt);

    free(this->panel);
    free(this);

    endwin();
}

void dwin_resize(struct _debugwindow *this) {

    endwin();
    dwin_init(this);

    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int mmx = (mx - (mx / 2));
    const int gy = (scr_y - 40);

    const int dim[N_PANEL][4] = {

        { 6, mx, 0, 0                     },
        { 6, mx, 6, 0                     },
        { 13, mx, 12, 0                   },
        { 13, (mx / 2), 25, 0             },  
        { 13, mmx, 25, (mx / 2)           }, 
        { (gy - 1), mx, 38, 0             },
        { (scr_y - 3), (scr_x / 2), 0, mx }
    };

    for(int i = 0; i < N_PANEL; i++) {

        int h = dim[i][0]; int w = dim[i][1];
        int y = dim[i][2]; int x = dim[i][3];

        panel_resize(this->panel[i], h, w, y, x);
    }

    prompt_resize(this->prompt, 3, scr_x, (38 + gy - 1), 0);
}

void dwin_read_prompt(const struct _debugwindow *this, char *buffer) {

    prompt_read(this->prompt, buffer);
}

void dwin_add(struct _debugwindow *this, const PANEL ptype, const char *str, const COLOR col) {

    panel_add(this->panel[ptype], str, col);
}

void dwin_write(struct _debugwindow *this, const PANEL ptype, const char *str, const COLOR col) {

    panel_write(this->panel[ptype], str, col);
}

void dwin_highlight(struct _debugwindow *this, const PANEL ptype, const char *str) {

    panel_highlight(this->panel[ptype], str);
}

void dwin_clear_panel(struct _debugwindow *this, const PANEL ptype) {

    panel_clear(this->panel[ptype]);
}

void dwin_clear(struct _debugwindow *this) {

    for(int i = 0; i < 5; i++)
        panel_clear(this->panel[i]);

    panel_clear(this->panel[RPNL]);
}

void dwin_update(struct _debugwindow *this, const PANEL ptype) {

    panel_update(this->panel[ptype]);
}

void dwin_update_all(struct _debugwindow *this) {

    for(int i = 0; i < N_PANEL; i++)
        panel_update(this->panel[i]);
}

void dwin_mv_curs(struct _debugwindow *this, const PANEL ptype, const int offs) {

    panel_mv_curs(this->panel[ptype], offs);
}

void dwin_set_curs(struct _debugwindow *this, const PANEL ptype, const int at) {

    panel_set_curs(this->panel[ptype], at);
}

int dwin_curs_of(struct _debugwindow *this, const PANEL ptype) {

    return panel_get_curs(this->panel[ptype]);
}

int dwin_height(struct _debugwindow *this, const PANEL ptype) {

    return this->panel[ptype]->height;
}

int dwin_width(struct _debugwindow *this, const PANEL ptype) {

    return this->panel[ptype]->width;
}

int dwin_y(struct _debugwindow *this, const PANEL ptype) {

    return this->panel[ptype]->y;
}

int dwin_x(struct _debugwindow *this, const PANEL ptype) {

    return this->panel[ptype]->x;
}

/* --- Static --- */

static void dwin_init(struct _debugwindow *this) {

    initscr();
    cbreak();
    refresh();
    echo();
    curs_set(0);
    start_color();
}
