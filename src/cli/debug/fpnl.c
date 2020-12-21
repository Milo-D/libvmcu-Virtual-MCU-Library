/* FLASH-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/fpnl.h"

struct _fpnl* fpnl_ctor(properties_t *prop) {

    struct _fpnl *fpnl;
    
    if((fpnl = malloc(sizeof(struct _fpnl))) == NULL)
        return NULL;
        
    panel_ctor(&fpnl->view, prop);
    return fpnl;
}

void fpnl_dtor(struct _fpnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}
