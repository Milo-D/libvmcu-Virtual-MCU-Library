/* DATA-Panel Header */

#ifndef DPNL_H
#define DPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _dpnl {
    
    panel_t view;

} dpnl_t;

extern struct _dpnl* dpnl_ctor(properties_t *prop);
extern void dpnl_dtor(struct _dpnl *this);

#endif
