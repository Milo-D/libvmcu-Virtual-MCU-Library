/* GPR-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/gpnl.h"

struct _gpnl* gpnl_ctor(properties_t *prop) {

    struct _gpnl *gpnl;
    
    if((gpnl = malloc(sizeof(struct _gpnl))) == NULL)
        return NULL;
    
    panel_ctor(&gpnl->view, prop);
    return gpnl; 
}

void gpnl_dtor(struct _gpnl *this) {

    panel_dtor(&this->view);
    free(this);
}
