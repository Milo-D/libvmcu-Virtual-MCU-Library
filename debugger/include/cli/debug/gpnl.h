/* GPR-Panel Header */

#ifndef GPNL_H
#define GPNL_H

// Project Headers (debugger)
#include "debugger/include/cli/util/panel.h"

typedef struct _gpnl {

    panel_t view;

} gpnl_t;

extern struct _gpnl* gpnl_ctor(void);
extern void gpnl_dtor(struct _gpnl *this);

extern void gpnl_resize(struct _gpnl *this);

#endif
