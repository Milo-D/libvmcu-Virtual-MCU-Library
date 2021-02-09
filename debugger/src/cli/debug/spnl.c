/* SREG-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers (debugger)
#include "debugger/include/cli/debug/spnl.h"

/* Forward Declaration of static Functions */

static void spnl_default_properties(properties_t *prop);

/* --- Extern --- */

struct _spnl* spnl_ctor(void) {

    struct _spnl *spnl;
    
    if((spnl = malloc(sizeof(struct _spnl))) == NULL)
        return NULL;
        
    properties_t prop;
    
    spnl_default_properties(&prop);
    panel_ctor(&spnl->view, &prop);
    
    return spnl;
}

void spnl_dtor(struct _spnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}

void spnl_resize(struct _spnl *this) {
    
    properties_t prop;
    
    spnl_default_properties(&prop);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void spnl_default_properties(properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int mmx = (mx - (mx / 2));
    const int gy = (scr_y - 40);
    
    *prop = (properties_t) {
        
        .window.height     = 6,
        .window.width      = mx,
        .window.y          = 6,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = 0
    };
}
