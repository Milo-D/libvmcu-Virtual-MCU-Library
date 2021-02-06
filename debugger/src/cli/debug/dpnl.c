/* DATA-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/dpnl.h"
#include "system/mcudef.h"

/* Forward Declaration of static Functions */

static void dpnl_default_properties(properties_t *prop);

/* --- Extern --- */

struct _dpnl* dpnl_ctor(void) {

    struct _dpnl *dpnl;
    
    if((dpnl = malloc(sizeof(struct _dpnl))) == NULL)
        return NULL;

    properties_t prop;
    
    dpnl_default_properties(&prop);
    panel_ctor(&dpnl->view, &prop);
    
    return dpnl;
}

void dpnl_dtor(struct _dpnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}

void dpnl_resize(struct _dpnl *this) {
    
    properties_t prop;
    
    dpnl_default_properties(&prop);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void dpnl_default_properties(properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    
    *prop = (properties_t) {
        
        .window.height     = 13,
        .window.width      = mx / 2,
        .window.y          = 25,
        .window.x          = 0,
        
        .page_init  = 0x0060,
        .page_range = RAM_END + 1
    };
}






