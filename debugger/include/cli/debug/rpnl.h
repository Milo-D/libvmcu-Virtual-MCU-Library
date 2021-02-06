/* Right-Panel Header */

#ifndef RPNL_H
#define RPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _rpnl {

    panel_t view;

} rpnl_t;

extern struct _rpnl* rpnl_ctor(const int size);
extern void rpnl_dtor(struct _rpnl *this);

extern void rpnl_resize(struct _rpnl *this);

#endif
