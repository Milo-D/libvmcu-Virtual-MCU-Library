/* Controlflow Graph Header */

#ifndef VMCU_CFG_H
#define VMCU_CFG_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/cfg_node.h"

typedef struct vmcu_cfg {

    int32_t used;
    vmcu_cfg_node_t *node;

} vmcu_cfg_t;

extern vmcu_cfg_t* vmcu_cfg_ctor(const uint32_t capacity);
extern void vmcu_cfg_dtor(vmcu_cfg_t *this);

#endif
