/* Format Reader Header */

#ifndef VMCU_READER_H
#define VMCU_READER_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_binary_buffer vmcu_binary_buffer_t;

extern vmcu_binary_buffer_t* vmcu_read_format(const char *file, uint32_t *size);

#endif