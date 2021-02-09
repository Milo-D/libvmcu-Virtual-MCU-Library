/* DATA-Panel Header */

#ifndef DPNL_H
#define DPNL_H

// Project Headers (debugger)
#include "debugger/include/cli/util/panel.h"

typedef struct _dpnl {
    
    panel_t view;

} dpnl_t;

extern struct _dpnl* dpnl_ctor(void);
extern void dpnl_dtor(struct _dpnl *this);

extern void dpnl_resize(struct _dpnl *this);

#endif
