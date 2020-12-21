/* SREG-Panel Header */

#ifndef SPNL_H
#define SPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _spnl {

    panel_t view;

} spnl_t;

extern struct _spnl* spnl_ctor(properties_t *prop);
extern void spnl_dtor(struct _spnl *this);

#endif
