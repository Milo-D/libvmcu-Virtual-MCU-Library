/* Registerpair Header */

#ifndef VMCU_REGISTERPAIR_H
#define VMCU_REGISTERPAIR_H

// Project Headers (engine utilities)
#include "engine/include/arch/enum/register.h"

typedef struct vmcu_registerpair {

    VMCU_REGISTER low;
    VMCU_REGISTER high;

} vmcu_registerpair_t;

#endif
