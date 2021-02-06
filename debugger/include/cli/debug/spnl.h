/* SREG-Panel Header */

#ifndef SPNL_H
#define SPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _spnl {

    panel_t view;

} spnl_t;

extern struct _spnl* spnl_ctor(void);
extern void spnl_dtor(struct _spnl *this);

extern void spnl_resize(struct _spnl *this);

#endif
