/* Output-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers (debugger)
#include "debugger/include/cli/debug/opnl.h"

/* Forward Declaration of static Functions */

static void opnl_default_properties(properties_t *prop);

/* --- Extern --- */

struct _opnl* opnl_ctor(void) {

    struct _opnl *opnl;
    
    if((opnl = malloc(sizeof(struct _opnl))) == NULL)
        return NULL;
    
    properties_t prop;
    
    opnl_default_properties(&prop);
    panel_ctor(&opnl->view, &prop);
    
    return opnl;
}

void opnl_dtor(struct _opnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}

void opnl_resize(struct _opnl *this) {
    
    properties_t prop;

    opnl_default_properties(&prop);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void opnl_default_properties(properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int gy = (scr_y - 40);
    
    *prop = (properties_t) {
        
        .window.height     = gy - 1,
        .window.width      = mx,
        .window.y          = 38,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = 0
    };
}
