/* Motorola SREC Reader Implementation */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include <utils/utils.h>
#include <lib/engine.h>
#include <lib/word.h>
#include "srec.h"

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

} srec_record_t;

typedef enum {

    SREC_ERROR_OK,
    SREC_ERROR_FATAL,
    SREC_ERROR_NOT_SUPPORTED

} srec_error_t;

typedef struct srec_properties {

    srec_record_t record_type;
    uint32_t      address;
    uint32_t      data_start;
    uint8_t       data_count;
    uint8_t       data[SREC_MAX_BYTE_COUNT];

} srec_properties_t;

/* Forward Declaration of static Functions */

static bool verify_srec_line(const char* line);
static uint32_t calculate_buffer_size(FILE* stream);

static int32_t read_srec_stream(FILE* stream, vmcu_word_t* words, uint32_t* size);
static int32_t read_srec_line(const char* line, vmcu_word_t* words, uint32_t* size);

static srec_error_t get_srec_properties(const char* line, srec_properties_t* prop);
static srec_error_t populate_data(const char* line, srec_properties_t* prop);

/* --- Extern --- */

vmcu_word_t* read_srec(FILE* stream, uint32_t* size) {

    uint32_t     n;
    vmcu_word_t* words;

    if ((n = calculate_buffer_size(stream)) == 0)
        return NULL;

    if ((words = vmcu.cb.alloc(n * sizeof(vmcu_word_t))) == NULL)
        return NULL;

    if (read_srec_stream(stream, words, size) < 0) {

        vmcu.cb.free(words);
        return NULL;
    }

    if (*size == 0) {

        vmcu.cb.free(words);
        return NULL;
    }

    return words;
}

/* --- Static --- */

static uint32_t calculate_buffer_size(FILE* stream) {

    size_t       len;
    uint32_t     size = 0;
    char*        line = NULL;
    const long   pos  = ftell(stream);
    srec_error_t error;

    while (getline(&line, &len, stream) != -1) {

        srec_properties_t prop;

        if ((error = get_srec_properties(line, &prop)) == SREC_ERROR_FATAL) {

            size = 0;
            goto cleanup;
        }

        if (error != SREC_ERROR_NOT_SUPPORTED)
            size += (prop.data_count / 2); 
    }

cleanup:
    fseek(stream, pos, SEEK_SET);
    free(line);

    return size;
}

static bool verify_srec_line(const char* line) {

    if (strlen(line) < SREC_MIN_LENGTH)
        return false;

    if (line[0] != 'S')
        return false;

    return true;
}

static int32_t read_srec_stream(FILE* stream, vmcu_word_t* words, uint32_t* size) {

    size_t len;
    char*  line = NULL;

    while (getline(&line, &len, stream) != -1) {

        if (read_srec_line(line, words, size) < 0) {

            *size = 0;
            free(line);

            return -1;
        }
    }

    free(line);
    return 0;
}

static int32_t read_srec_line(const char* line, vmcu_word_t* words, uint32_t* size) {

    srec_properties_t prop;
    srec_error_t      err;

    if ((err = get_srec_properties(line, &prop)) == SREC_ERROR_FATAL)
        return -1;

    if (err == SREC_ERROR_NOT_SUPPORTED)
        return 0;

    if (populate_data(line, &prop) != SREC_ERROR_OK)
        return -1;

    for (uint32_t i = 0; i < (prop.data_count / 2); i++) {

        uint8_t high, low;

        if (vmcu.reader.endianness == VMCU_ENDIAN_LITTLE) {

            low  = prop.data[ (i * 2) + 0 ];
            high = prop.data[ (i * 2) + 1 ];

        } else {

            high = prop.data[ (i * 2) + 0 ];
            low  = prop.data[ (i * 2) + 1 ];
        }

        words[*size] = (vmcu_word_t) {

            .raw  = (high << 8) | low,
            .addr = (prop.address / 2) + i
        };

        *size += 1;
    }

    return 0;
}

static srec_error_t get_srec_properties(const char* line, srec_properties_t* prop) {

    uint8_t byte_count;
    uint8_t address_nbytes;
    int     error;

    if (verify_srec_line(line) == false)
        return SREC_ERROR_FATAL;

    prop->record_type = line[1] - '0';

    char byte_count_str[3] = { line[2], line[3], '\0' };
    byte_count = htou64(byte_count_str, &error);

    if (error != 0)
        return SREC_ERROR_FATAL;

    if (byte_count > SREC_MAX_BYTE_COUNT)
        return SREC_ERROR_FATAL;

    char addr_str[9];
    memset(addr_str, 0, 9);

    switch (prop->record_type) {

        case SREC_RECORD_S1:

            memcpy(addr_str, line + 4, 4);
            address_nbytes   = 2;
            prop->data_start = 8;

        break;

        case SREC_RECORD_S2:

            memcpy(addr_str, line + 4, 6);
            address_nbytes   = 3;
            prop->data_start = 10;

        break;

        case SREC_RECORD_S3:

            memcpy(addr_str, line + 4, 8);
            address_nbytes   = 4;
            prop->data_start = 12;

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
    prop->address    = htou64(addr_str, &error);

    return (error == 0) ? SREC_ERROR_OK : SREC_ERROR_FATAL; 
}

static srec_error_t populate_data(const char* line, srec_properties_t* prop) {

    const size_t len   = strlen(line);
    uint32_t     index = prop->data_start;

    for (uint32_t i = 0; i < prop->data_count; i++) {

        int error;

        if ((index + 1) >= len)
            return SREC_ERROR_FATAL;

        char data_str[3] = {

            line[index],
            line[index + 1],
            '\0'
        };

        prop->data[i] = htou64(data_str, &error);

        if (error != 0)
            return SREC_ERROR_FATAL;

        index += 2;
    }

    return SREC_ERROR_OK;
}
