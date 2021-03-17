/* Vector (Datastructure) Header */

#ifndef VMCU_VECTOR_H
#define VMCU_VECTOR_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/xref.h"

// Project Headers (engine utilities)
#include "engine/include/arch/enum/vect.h"

typedef struct vmcu_vector {

    VMCU_VECT id;
    uint16_t addr;

    int32_t n_xto;
    vmcu_xref_t *xto;

} vmcu_vector_t;

#endif
