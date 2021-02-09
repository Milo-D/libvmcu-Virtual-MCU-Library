/* Output-Panel Header */

#ifndef OPNL_H
#define OPNL_H

// Project Headers (debugger)
#include "debugger/include/cli/util/panel.h"

typedef struct _opnl {
    
    panel_t view;
    
} opnl_t;

extern struct _opnl* opnl_ctor(void);
extern void opnl_dtor(struct _opnl *this);

extern void opnl_resize(struct _opnl *this);

#endif
