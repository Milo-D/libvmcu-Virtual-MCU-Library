/* Output-Panel Header */

#ifndef OPNL_H
#define OPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _opnl {
    
    panel_t view;
    
} opnl_t;

extern struct _opnl* opnl_ctor(properties_t *prop);
extern void opnl_dtor(struct _opnl *this);

#endif
