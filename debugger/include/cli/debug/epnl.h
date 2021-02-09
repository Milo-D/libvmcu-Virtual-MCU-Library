/* EEPROM-Panel Header */

#ifndef EPNL_H
#define EPNL_H

// Project Headers (debugger)
#include "debugger/include/cli/util/panel.h"

typedef struct _epnl {
    
    panel_t view;
    
} epnl_t;

extern struct _epnl* epnl_ctor(void);
extern void epnl_dtor(struct _epnl *this);

extern void epnl_resize(struct _epnl *this);

#endif
