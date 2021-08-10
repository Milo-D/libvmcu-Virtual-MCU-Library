/* Label (Datastructure) Header */

#ifndef VMCU_LABEL_H
#define VMCU_LABEL_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/xref.h"

typedef struct vmcu_label {

    uint32_t id;
    uint32_t addr;

    uint32_t n_xfrom;
    vmcu_xref_t *xfrom;

} vmcu_label_t;

#endif

