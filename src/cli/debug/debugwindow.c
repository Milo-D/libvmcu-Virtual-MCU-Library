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

static void calculate_panel_properties(properties_t *props, const int size);
static void calculate_prompt_properties(win_properties_t *props);

/* --- Extern --- */

struct _debugwindow* dwin_ctor(const int size) {

    struct _debugwindow *dwin;

    if((dwin = malloc(sizeof(struct _debugwindow))) == NULL)
        return NULL;

    dwin_init(dwin);

    properties_t props[N_PANEL];
    calculate_panel_properties(props, size);
    
    win_properties_t win_props;
    calculate_prompt_properties(&win_props);

    dwin->gpnl = gpnl_ctor(&props[GPNL]);
    dwin->spnl = spnl_ctor(&props[SPNL]);
    dwin->fpnl = fpnl_ctor(&props[FPNL]);
    dwin->dpnl = dpnl_ctor(&props[DPNL]);
    dwin->epnl = epnl_ctor(&props[EPNL]);
    dwin->opnl = opnl_ctor(&props[OPNL]);
    dwin->rpnl = rpnl_ctor(&props[RPNL]);
    
    dwin->panels[GPNL] = &dwin->gpnl->view;
    dwin->panels[SPNL] = &dwin->spnl->view;
    dwin->panels[FPNL] = &dwin->fpnl->view;
    dwin->panels[DPNL] = &dwin->dpnl->view;
    dwin->panels[EPNL] = &dwin->epnl->view;
    dwin->panels[OPNL] = &dwin->opnl->view;
    dwin->panels[RPNL] = &dwin->rpnl->view;

    dwin->prompt = prompt_ctor(&win_props);
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

    properties_t props[N_PANEL];
    calculate_panel_properties(props, 0);

    win_properties_t win_props;
    calculate_prompt_properties(&win_props);

    for(int i = 0; i < N_PANEL; i++)
        panel_resize(this->panels[i], &props[i]);

    prompt_resize(this->prompt, &win_props);
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

static void calculate_panel_properties(properties_t *props, const int size) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int mmx = (mx - (mx / 2));
    const int gy = (scr_y - 40);

    int n_pages = (size / (scr_y - 7));
    n_pages += ((size % (scr_y - 7)) != 0);
    
    props[GPNL] = (properties_t) {
        
        .window.height     = 6,
        .window.width      = mx,
        .window.y          = 0,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = GPR_SIZE / 8
    };
    
    props[SPNL] = (properties_t) {
        
        .window.height     = 6,
        .window.width      = mx,
        .window.y          = 6,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = 0
    };
    
    props[FPNL] = (properties_t) {
        
        .window.height     = 13,
        .window.width      = mx,
        .window.y          = 12,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = 0
    };
    
    props[DPNL] = (properties_t) {
        
        .window.height     = 13,
        .window.width      = mx / 2,
        .window.y          = 25,
        .window.x          = 0,
        
        .page_init  = 0x0060,
        .page_range = RAM_END + 1
    };
    
    props[EPNL] = (properties_t) {
        
        .window.height     = 13,
        .window.width      = mmx,
        .window.y          = 25,
        .window.x          = mx / 2,
        
        .page_init  = EEPROM_SIZE / 2,
        .page_range = EEPROM_SIZE
    };

    props[OPNL] = (properties_t) {
        
        .window.height     = gy - 1,
        .window.width      = mx,
        .window.y          = 38,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = 0
    };

    props[RPNL] = (properties_t) {
        
        .window.height     = scr_y - 3,
        .window.width      = scr_x / 2,
        .window.y          = 0,
        .window.x          = mx,
        
        .page_init  = 0,
        .page_range = n_pages
    };
}

static void calculate_prompt_properties(win_properties_t *props) {

    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int gy = (scr_y - 40);

    props->height = 3;
    props->width  = scr_x;
    props->y      = 38 + gy - 1;
    props->x      = 0;
}










