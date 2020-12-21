/* DebugWindow Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/debugwindow.h"
#include "cli/debug/prompt.h"
#include "cli/debug/gpnl.h"
#include "cli/debug/spnl.h"
#include "cli/debug/fpnl.h"
#include "cli/debug/dpnl.h"
#include "cli/debug/epnl.h"
#include "cli/debug/opnl.h"
#include "cli/debug/rpnl.h"
#include "cli/util/panel.h"
#include "system/mcudef.h"

/* Forward Declaration of static Functions */

static void dwin_init(struct _debugwindow *this);

/* --- Extern --- */

struct _debugwindow* dwin_ctor(const int size) {

    struct _debugwindow *dwin;

    if((dwin = malloc(sizeof(struct _debugwindow))) == NULL)
        return NULL;

    dwin_init(dwin);

    dwin->gpnl = gpnl_ctor();
    dwin->spnl = spnl_ctor();
    dwin->fpnl = fpnl_ctor();
    dwin->dpnl = dpnl_ctor();
    dwin->epnl = epnl_ctor();
    dwin->opnl = opnl_ctor();
    dwin->rpnl = rpnl_ctor(size);

    dwin->prompt = prompt_ctor();
    
    dwin->panels[GPNL] = &dwin->gpnl->view;
    dwin->panels[SPNL] = &dwin->spnl->view;
    dwin->panels[FPNL] = &dwin->fpnl->view;
    dwin->panels[DPNL] = &dwin->dpnl->view;
    dwin->panels[EPNL] = &dwin->epnl->view;
    dwin->panels[OPNL] = &dwin->opnl->view;
    dwin->panels[RPNL] = &dwin->rpnl->view;

    return dwin;
}

void dwin_dtor(struct _debugwindow *this) {

    gpnl_dtor(this->gpnl);
    spnl_dtor(this->spnl);
    fpnl_dtor(this->fpnl);
    dpnl_dtor(this->dpnl);
    epnl_dtor(this->epnl);
    opnl_dtor(this->opnl);
    rpnl_dtor(this->rpnl);

    prompt_dtor(this->prompt);
    
    free(this);
    endwin();
}

void dwin_resize(struct _debugwindow *this) {

    endwin();
    dwin_init(this);

    gpnl_resize(this->gpnl);
    spnl_resize(this->spnl);
    fpnl_resize(this->fpnl);
    dpnl_resize(this->dpnl);
    epnl_resize(this->epnl);
    opnl_resize(this->opnl);
    rpnl_resize(this->rpnl);

    prompt_resize(this->prompt);
}

void dwin_read_prompt(const struct _debugwindow *this, char *buffer) {

    prompt_read(this->prompt, buffer);
}

void dwin_add(struct _debugwindow *this, const PANEL ptype, const char *str, const COLOR col) {

    panel_add(this->panels[ptype], str, col);
}

void dwin_write(struct _debugwindow *this, const PANEL ptype, const char *str, const COLOR col) {

    panel_write(this->panels[ptype], str, col);
}

void dwin_highlight(struct _debugwindow *this, const PANEL ptype, const char *str) {

    panel_highlight(this->panels[ptype], str);
}

void dwin_clear_panel(struct _debugwindow *this, const PANEL ptype) {

    panel_clear(this->panels[ptype]);
}

void dwin_clear(struct _debugwindow *this) {

    for(int i = GPNL; i < OPNL; i++)
        panel_clear(this->panels[i]);

    panel_clear(this->panels[RPNL]);
}

void dwin_update(struct _debugwindow *this, const PANEL ptype) {

    panel_update(this->panels[ptype]);
}

void dwin_update_all(struct _debugwindow *this) {

    for(int i = 0; i < N_PANEL; i++)
        panel_update(this->panels[i]);
}

void dwin_change_page(struct _debugwindow *this, const PANEL ptype, const int offs) {

    panel_change_page(this->panels[ptype], offs);
}

void dwin_set_page(struct _debugwindow *this, const PANEL ptype, const int at) {

    panel_set_page(this->panels[ptype], at);
}

int dwin_get_page(struct _debugwindow *this, const PANEL ptype) {

    return panel_get_page(this->panels[ptype]);
}

int dwin_height(struct _debugwindow *this, const PANEL ptype) {

    return this->panels[ptype]->height;
}

int dwin_width(struct _debugwindow *this, const PANEL ptype) {

    return this->panels[ptype]->width;
}

int dwin_y(struct _debugwindow *this, const PANEL ptype) {

    return this->panels[ptype]->y;
}

int dwin_x(struct _debugwindow *this, const PANEL ptype) {

    return this->panels[ptype]->x;
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
