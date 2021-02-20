/* Label (Datastructure) Header */

#ifndef VMCU_LABEL_H
#define VMCU_LABEL_H

// C Headers
#include <inttypes.h>

// Project Headers
#include "engine/include/analyzer/report/caller.h"

typedef struct vmcu_label {

    uint16_t id;
    uint16_t addr;

    int32_t ncallers;
    vmcu_caller_t *caller;

} vmcu_label_t;

#endif
