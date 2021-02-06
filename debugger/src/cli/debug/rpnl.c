/* Right-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/rpnl.h"

/* Forward Declaration of static Functions */

static void rpnl_default_properties(properties_t *prop, const int size);

/* --- Extern --- */

struct _rpnl* rpnl_ctor(const int size) {

    struct _rpnl *rpnl;
    
    if((rpnl = malloc(sizeof(struct _rpnl))) == NULL)
        return NULL;
        
    properties_t prop;
    
    rpnl_default_properties(&prop, size);
    panel_ctor(&rpnl->view, &prop);
    
    return rpnl;
}

void rpnl_dtor(struct _rpnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}

void rpnl_resize(struct _rpnl *this) {
    
    properties_t prop;
    
    rpnl_default_properties(&prop, 0);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void rpnl_default_properties(properties_t *prop, const int size) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);

    int n_pages = (size / (scr_y - 7));
    n_pages += ((size % (scr_y - 7)) != 0);
    
    *prop = (properties_t) {
        
        .window.height     = scr_y - 3,
        .window.width      = scr_x / 2,
        .window.y          = 0,
        .window.x          = mx,
        
        .page_init  = 0,
        .page_range = n_pages
    };
}
