/* Intel Hex Reader Header */

#ifndef VMCU_IHEX_READER_H
#define VMCU_IHEX_READER_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_instr vmcu_instr_t;

extern vmcu_instr_t* vmcu_read_ihex(const char *hex_file, int32_t *size);

#endif
