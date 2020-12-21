/* Right-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/rpnl.h"

struct _rpnl* rpnl_ctor(properties_t *prop) {

    struct _rpnl *rpnl;
    
    if((rpnl = malloc(sizeof(struct _rpnl))) == NULL)
        return NULL;
        
    panel_ctor(&rpnl->view, prop);
    return rpnl;
}

void rpnl_dtor(struct _rpnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}
