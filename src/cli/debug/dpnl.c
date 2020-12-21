/* DATA-Panel Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "cli/debug/dpnl.h"

struct _dpnl* dpnl_ctor(properties_t *prop) {

    struct _dpnl *dpnl;
    
    if((dpnl = malloc(sizeof(struct _dpnl))) == NULL)
        return NULL;
        
    panel_ctor(&dpnl->view, prop);
    return dpnl;
}

void dpnl_dtor(struct _dpnl *this) {
    
    panel_dtor(&this->view);
    free(this);
}
