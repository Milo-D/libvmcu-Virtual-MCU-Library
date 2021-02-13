/* Label (Datastructure) Header */

#ifndef LABEL_H
#define LABEL_H

// C Headers
#include <inttypes.h>

// Project Headers
#include "engine/include/analyzer/report/caller.h"

typedef struct _label {

    uint16_t id;
    uint16_t addr;

    int32_t ncallers;
    caller_t *caller;

} label_t;

#endif
