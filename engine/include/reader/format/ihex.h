/* Intel Hex Reader Header */

#ifndef VMCU_IHEX_H
#define VMCU_IHEX_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_binary_buffer vmcu_binary_buffer_t;

extern vmcu_binary_buffer_t * vmcu_read_ihex(const char *hex_file, uint32_t *size);

#endif
