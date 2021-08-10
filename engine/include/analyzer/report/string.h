/* String (Datastructure) Header */

#ifndef VMCU_STRING_H
#define VMCU_STRING_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_string {

    uint32_t addr;
    uint64_t length;

    char *bytes;

} vmcu_string_t;

#endif
