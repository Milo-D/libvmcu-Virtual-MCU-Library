/* FLASH-Panel Header */

#ifndef FPNL_H
#define FPNL_H

// Project Headers
#include "cli/util/panel.h"

typedef struct _fpnl {
    
    panel_t view;
    
} fpnl_t;

extern struct _fpnl* fpnl_ctor(void);
extern void fpnl_dtor(struct _fpnl *this);

extern void fpnl_resize(struct _fpnl *this);

#endif
