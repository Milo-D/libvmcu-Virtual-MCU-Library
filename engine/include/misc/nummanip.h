/* Number Manipulation Header */

#ifndef VMCU_NUMMANIP_H
#define VMCU_NUMMANIP_H

// C Headers
#include <inttypes.h>

#define mod(a, b) (((((int64_t) a + (int64_t) b) % (int64_t) b) + (int64_t) b) % (int64_t) b)

#endif
