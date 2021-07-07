/* CFG Node Header */

#ifndef VMCU_CFG_NODE_H
#define VMCU_CFG_NODE_H

// Project Headers (engine)
#include "engine/include/analyzer/report/xref.h"

typedef struct vmcu_cfg_node vmcu_cfg_node_t;

typedef struct vmcu_cfg_node {

    vmcu_xref_t xto;

    vmcu_cfg_node_t *t;
    vmcu_cfg_node_t *f;

} vmcu_cfg_node_t;

#endif
