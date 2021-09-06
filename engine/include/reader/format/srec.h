/* Motorola SREC Reader Header */

#ifndef VMCU_SREC_H
#define VMCU_SREC_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_binary_buffer vmcu_binary_buffer_t;

extern vmcu_binary_buffer_t * vmcu_read_srec(const char *srec_file, uint32_t *size);

#endif
