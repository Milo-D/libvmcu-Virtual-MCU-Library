/* Motorola SREC Reader Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine, readers)
#include "engine/include/reader/format/srec.h"
#include "engine/include/reader/binary_buffer.h"

// Project Headers (engine utilities)
#include "engine/include/misc/filemanip.h"
#include "engine/include/misc/stringmanip.h"

#define SREC_MIN_LENGTH 10
#define SREC_MAX_BYTE_COUNT 249

/* Forward Declaration of static Members */

typedef enum {

    SREC_RECORD_S0,
    SREC_RECORD_S1,
    SREC_RECORD_S2,
    SREC_RECORD_S3,
    SREC_RECORD_S4,
    SREC_RECORD_S5,
    SREC_RECORD_S6,
    SREC_RECORD_S7,
    SREC_RECORD_S8,
    SREC_RECORD_S9

} SREC_RECORD;

typedef enum {

    SREC_ERROR_OK,
    SREC_ERROR_FATAL,
    SREC_ERROR_NOT_SUPPORTED

} SREC_ERROR;

typedef struct srec_properties {

    uint32_t address;
    SREC_RECORD record_type;

    uint8_t data_count;
    uint8_t data[SREC_MAX_BYTE_COUNT];

} srec_properties_t;

/* Forward Declaration of static Functions */

static bool verify_srec_line(const char *line);
static uint32_t estimate_buffer_size(const char *srec_file);

static int32_t read_srec_file(const char *srec_file, vmcu_binary_buffer_t *bb, uint32_t *size);
static int32_t read_srec_line(const char *line, vmcu_binary_buffer_t *bb, uint32_t *size);

static SREC_ERROR get_srec_properties(const char *line, srec_properties_t *prop);
static SREC_ERROR collect_data(const char *line, uint32_t index, srec_properties_t *prop);

/* --- Extern --- */

vmcu_binary_buffer_t* vmcu_read_srec(const char *srec_file, uint32_t *size) {

    uint32_t n;
    vmcu_binary_buffer_t *bb;

    if((n = estimate_buffer_size(srec_file)) == 0)
        return NULL;

    if((bb = malloc(n * sizeof(vmcu_binary_buffer_t))) == NULL)
        return NULL;

    if(read_srec_file(srec_file, bb, size) < 0) {

        free(bb);
        return NULL;
    }

    if(*size == 0) {

        free(bb);
        return NULL;
    }

    return bb;
}

/* --- Static --- */

static uint32_t estimate_buffer_size(const char *srec_file) {

    long bytes;

    if((bytes = vmcu_fbytes(srec_file)) < 4)
        return 0;

    return (bytes / 4);
}

static bool verify_srec_line(const char *line) {

    if(strlen(line) < SREC_MIN_LENGTH)
        return false;

    if(line[0] != 'S')
        return false;

    return true;
}

static int32_t read_srec_file(const char *srec_file, vmcu_binary_buffer_t *bb, uint32_t *size) {

    FILE *f = NULL;
    size_t len; char *line = NULL;

    if((f = fopen(srec_file, "r")) == NULL)
        return -1;

    while(getline(&line, &len, f) != -1) {

        if(read_srec_line(line, bb, size) < 0) {

            *size = 0;

            free(line);
            fclose(f);

            return -1;
        }
    }

    free(line);
    fclose(f);

    return 0;
}

static int32_t read_srec_line(const char *line, vmcu_binary_buffer_t *bb, uint32_t *size) {

    srec_properties_t prop; SREC_ERROR err;

    if((err = get_srec_properties(line, &prop)) == SREC_ERROR_FATAL)
        return -1;

    if(err == SREC_ERROR_NOT_SUPPORTED)
        return 0;

    for(uint32_t i = 0; i < (prop.data_count / 2); i++) {

        const uint8_t high = prop.data[ (i * 2) + 0 ];
        const uint8_t low  = prop.data[ (i * 2) + 1 ];

        bb[*size] = (vmcu_binary_buffer_t) {

            .bytes  = (high << 8) | low,
            .addr   = (prop.address / 2) + i
        };

        *size += 1;
    }

    return 0;
}

static SREC_ERROR get_srec_properties(const char *line, srec_properties_t *prop) {

    uint32_t index_data;
    uint8_t byte_count, address_nbytes;

    if(verify_srec_line(line) == false)
        return SREC_ERROR_FATAL;

    char byte_count_str[3] = { line[2], line[3], '\0' };
    prop->record_type = line[1] - '0';

    if((byte_count = vmcu_htoi(byte_count_str)) > SREC_MAX_BYTE_COUNT)
        return SREC_ERROR_FATAL;

    char addr_str[9];
    memset(addr_str, 0, 9);

    switch(prop->record_type) {

        case SREC_RECORD_S1:

            memcpy(addr_str, line + 4, 4);
            address_nbytes = 2;
            index_data = 8;

        break;

        case SREC_RECORD_S2:

            memcpy(addr_str, line + 4, 6);
            address_nbytes = 3;
            index_data = 10;

        break;

        case SREC_RECORD_S3:

            memcpy(addr_str, line + 4, 8);
            address_nbytes = 4;
            index_data = 12;

        break;

        case SREC_RECORD_S0:
        case SREC_RECORD_S4:
        case SREC_RECORD_S5:
        case SREC_RECORD_S6:
        case SREC_RECORD_S7:
        case SREC_RECORD_S8:
        case SREC_RECORD_S9:

            /* not supported, skip line */
            return SREC_ERROR_NOT_SUPPORTED;

        default: return SREC_ERROR_FATAL;
    }

    prop->data_count = byte_count - (address_nbytes + 1);
    prop->address    = vmcu_htoi(addr_str);

    return collect_data(line, index_data, prop);
}

static SREC_ERROR collect_data(const char *line, uint32_t index, srec_properties_t *prop) {

    const size_t len = strlen(line);

    for(uint32_t i = 0; i < prop->data_count; i++) {

        if((index + 1) >= len)
            return SREC_ERROR_FATAL;

        char data_str[3] = {

            line[index],
            line[index + 1],
            '\0'
        };

        prop->data[i] = vmcu_htoi(data_str);
        index += 2;
    }

    return SREC_ERROR_OK;
}