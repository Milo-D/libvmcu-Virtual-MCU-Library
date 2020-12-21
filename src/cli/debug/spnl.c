/* SREG-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/spnl.h"

struct _spnl* spnl_ctor(properties_t *prop) {

    struct _spnl *spnl;
    
    if((spnl = malloc(sizeof(struct _spnl))) == NULL)
        return NULL;
        
    panel_ctor(&spnl->view, prop);
    return spnl;
}

void spnl_dtor(struct _spnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}
