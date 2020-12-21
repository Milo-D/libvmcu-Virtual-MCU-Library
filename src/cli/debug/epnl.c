/* EEPROM-Panel Implementation */

// C Headers
#include <stdlib.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/epnl.h"
#include "system/mcudef.h"

/* Forward Declaration of static Functions */

static void epnl_default_properties(properties_t *prop);

/* --- Extern --- */

struct _epnl* epnl_ctor(void) {

    struct _epnl *epnl;
    
    if((epnl = malloc(sizeof(struct _epnl))) == NULL)
        return NULL;

    properties_t prop;

    epnl_default_properties(&prop);
    panel_ctor(&epnl->view, &prop);
    
    return epnl;
}

void epnl_dtor(struct _epnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}

void epnl_resize(struct _epnl *this) {
 
    properties_t prop;

    epnl_default_properties(&prop);
    panel_resize(&this->view, &prop);
}

/* --- Static --- */

static void epnl_default_properties(properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int mmx = (mx - (mx / 2));
    
    *prop = (properties_t) {
        
        .window.height     = 13,
        .window.width      = mmx,
        .window.y          = 25,
        .window.x          = mx / 2,
        
        .page_init  = EEPROM_SIZE / 2,
        .page_range = EEPROM_SIZE
    };
}

