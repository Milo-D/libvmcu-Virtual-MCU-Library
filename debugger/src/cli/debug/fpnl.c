/* FLASH-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/fpnl.h"

/* Forward Declaration of static Functions */

static void fpnl_default_properties(properties_t *prop);

/* --- Extern --- */

struct _fpnl* fpnl_ctor(void) {

    struct _fpnl *fpnl;
    
    if((fpnl = malloc(sizeof(struct _fpnl))) == NULL)
        return NULL;
        
    properties_t prop;
    
    fpnl_default_properties(&prop);
    panel_ctor(&fpnl->view, &prop);
    
    return fpnl;
}

void fpnl_dtor(struct _fpnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}

void fpnl_resize(struct _fpnl *this) {
    
    properties_t prop;
    
    fpnl_default_properties(&prop);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void fpnl_default_properties(properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    
    *prop = (properties_t) {
        
        .window.height     = 13,
        .window.width      = mx,
        .window.y          = 12,
        .window.x          = 0,
        
        .page_init  = 0,
        .page_range = 0
    };
}
