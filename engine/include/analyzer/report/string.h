/* String (Datastructure) Header */

#ifndef VMCU_STRING_H
#define VMCU_STRING_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_string {

    uint16_t addr;
    uint16_t length;

    char *bytes;

} vmcu_string_t;

#endif
