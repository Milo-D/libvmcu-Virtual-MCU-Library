/* EEPROM-Panel Header */

#ifndef EPNL_H
#define EPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _epnl {
    
    panel_t view;
    
} epnl_t;

extern struct _epnl* epnl_ctor(properties_t *prop);
extern void epnl_dtor(struct _epnl *this);

#endif
