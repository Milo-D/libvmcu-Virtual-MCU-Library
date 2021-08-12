/* Format Reader Implementation */

// C Headers
#include <stddef.h>

// Project Headers (engine)
#include "engine/include/reader/reader.h"

// Project Headers (engine, formats)
#include "engine/include/reader/format/ihex.h"

vmcu_binary_buffer_t* vmcu_read_format(const VMCU_FMT fmt, const char *file, uint32_t *size) {

    vmcu_binary_buffer_t *bb;

    switch(fmt) {

        case VMCU_FMT_IHEX: bb = vmcu_read_ihex(file, size); break;
        default:            bb = NULL;                       break;
    }

    return bb;
}