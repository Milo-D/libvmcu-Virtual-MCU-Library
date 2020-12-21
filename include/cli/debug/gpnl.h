/* GPR-Panel Header */

#ifndef GPNL_H
#define GPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _gpnl {

    panel_t view;

} gpnl_t;

extern struct _gpnl* gpnl_ctor(properties_t *prop);
extern void gpnl_dtor(struct _gpnl *this);

#endif
