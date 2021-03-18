/* SFR (Datastructure) Header */

#ifndef VMCU_SFR_H
#define VMCU_SFR_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/xref.h"

// Project Headers (engine utilities)
#include "engine/include/arch/enum/sfr_enum.h"

typedef struct vmcu_sfr {

    VMCU_SFR id;

    int32_t n_xfrom;
    vmcu_xref_t *xfrom;

} vmcu_sfr_t;

#endif
