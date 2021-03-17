/* SFR (Datastructure) Header */

#ifndef VMCU_SFR_H
#define VMCU_SFR_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/xref.h"

// Project Headers (engine utilities)
#include "engine/include/arch/enum/sfregister.h"

typedef struct vmcu_sfr {

    VMCU_SFREGISTER id;

    int32_t n_xref;
    vmcu_xref_t *xref;

} vmcu_sfr_t;

#endif
