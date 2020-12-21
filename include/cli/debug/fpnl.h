/* FLASH-Panel Header */

#ifndef FPNL_H
#define FPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _fpnl {
    
    panel_t view;
    
} fpnl_t;

extern struct _fpnl* fpnl_ctor(properties_t *prop);
extern void fpnl_dtor(struct _fpnl *this);

#endif
