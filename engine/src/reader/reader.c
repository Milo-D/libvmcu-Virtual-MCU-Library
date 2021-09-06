/* Format Reader Implementation */

// C Headers
#include <stddef.h>

// Project Headers (engine)
#include "engine/include/reader/reader.h"

// Project Headers (engine, formats)
#include "engine/include/reader/format/ihex.h"
#include "engine/include/reader/format/srec.h"

// Project Headers (engine, reader utils)
#include "engine/include/reader/util/fmt.h"

vmcu_binary_buffer_t* vmcu_read_format(const char *file, uint32_t *size) {

    vmcu_binary_buffer_t *bb;

    switch( vmcu_format_of(file) ) {

        case VMCU_FMT_IHEX: bb = vmcu_read_ihex(file, size); break;
        case VMCU_FMT_SREC: bb = vmcu_read_srec(file, size); break;

        default:            bb = NULL;                       break;
    }

    return bb;
}