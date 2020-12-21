/* Output-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/opnl.h"

struct _opnl* opnl_ctor(properties_t *prop) {

    struct _opnl *opnl;
    
    if((opnl = malloc(sizeof(struct _opnl))) == NULL)
        return NULL;
        
    panel_ctor(&opnl->view, prop);
    return opnl;
}

void opnl_dtor(struct _opnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}
