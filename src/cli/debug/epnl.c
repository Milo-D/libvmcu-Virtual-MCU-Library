/* EEPROM-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/epnl.h"

struct _epnl* epnl_ctor(properties_t *prop) {

    struct _epnl *epnl;
    
    if((epnl = malloc(sizeof(struct _epnl))) == NULL)
        return NULL;

    panel_ctor(&epnl->view, prop);
    return epnl;
}

void epnl_dtor(struct _epnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}
