/* GPR-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers (engine) (GPR_SIZE needed)
#include "engine/include/system/mcudef.h"

// Project Headers (debugger)
#include "debugger/include/cli/debug/gpnl.h"

/* Forward Declaration of static Functions */

static void gpnl_default_properties(properties_t *prop);

/* --- Extern --- */

struct _gpnl* gpnl_ctor(void) {

    struct _gpnl *gpnl;
    
    if((gpnl = malloc(sizeof(struct _gpnl))) == NULL)
        return NULL;
    
    properties_t prop;
    
    gpnl_default_properties(&prop);
    panel_ctor(&gpnl->view, &prop);

    return gpnl; 
}

void gpnl_dtor(struct _gpnl *this) {

    panel_dtor(&this->view);
    free(this);
}

void gpnl_resize(struct _gpnl *this) {
    
    properties_t prop;
    
    gpnl_default_properties(&prop);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void gpnl_default_properties(properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);

    *prop = (properties_t) {

        .window.height     = 6,
        .window.width      = mx,
        .window.y          = 0,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = GPR_SIZE / 8
    };
}
