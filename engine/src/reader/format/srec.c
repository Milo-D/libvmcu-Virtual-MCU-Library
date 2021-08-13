/* Intel Hex Reader Header */

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

#define RECORD_MAX_LENGTH_BYTES (1+1+2+255)
#define SREC_MAX_BYTE_COUNT 249

#define ERR_RECORD_NOT_SUPPORTED -2
#define ERR_FATAL -1

/* Forward Declaration of static Members */

typedef enum srec_record_type {

    S0_HEADER = 0,
    S1_DATA,
    S2_DATA,
    S3_DATA,
    S4_RESERVED,
    S5_COUNT,
    S6_COUNT,
    S7_START_ADDR,
    S8_START_ADDR,
    S9_START_ADDR

} srec_record_type_t;

typedef struct srec_properties {

    srec_record_type_t record_type;
    uint32_t address;

    uint8_t data_count;
    uint8_t data[SREC_MAX_BYTE_COUNT];

} srec_properties_t;

/* --- static --- */
static uint32_t estimate_buffer_size(const char* srec_file);

static int8_t read_srec_line(const char* line, vmcu_binary_buffer_t* buffer, uint32_t* size);

static int8_t get_srec_properties(const char* line, srec_properties_t* prop);
/* --- Extern --- */

vmcu_binary_buffer_t * vmcu_read_srec(const char *srec_file, uint32_t *size) {

    FILE* file = fopen(srec_file, "r");

    if(file == NULL)
        return NULL;

    uint32_t n = estimate_buffer_size(srec_file);

    vmcu_binary_buffer_t* buffer = malloc(sizeof(vmcu_binary_buffer_t)*n);

    char* line = NULL;
    size_t len;

    while(getline(&line, &len, file) != -1){

        int8_t status = read_srec_line(line, buffer, size);

        if(status == ERR_FATAL){

            *size = 0;

            free(line);
            fclose(file);

            return NULL;
        }
    }

    free(line);
    fclose(file);
    return buffer;
}

static uint32_t estimate_buffer_size(const char* srec_file){

    FILE* file = fopen(srec_file, "r");

    long bytes = vmcu_fbytes(srec_file);

    fclose(file);

    return (bytes / 4);
}

static int8_t read_srec_line(const char* line, vmcu_binary_buffer_t* buffer, uint32_t* size){

    srec_properties_t prop;

    int8_t status = get_srec_properties(line, &prop);

    if(status < 0)
        return status;

    for(uint32_t i = 0; i < (prop.data_count / 2); i++) {

        uint16_t srec_bytes = (prop.data[i*2 + 0] << 8)| prop.data[i*2 + 1];

        buffer[*size] = (vmcu_binary_buffer_t) {

            .bytes  = srec_bytes,
            .addr   = (prop.address/2) + i
        };

        *size += 1;
    }

    return 0;
}

static int8_t get_srec_properties(const char* line, srec_properties_t* prop){

    uint32_t len = strlen(line);

    if(len < 10)
        return ERR_FATAL;

    if(line[0] != 'S')
        return ERR_FATAL;

    prop->record_type = line[1] - '0';

    char byte_count_str[3] = { line[2], line[3], '\0' };

    uint8_t byte_count  = vmcu_htoi(byte_count_str);

    if(byte_count > SREC_MAX_BYTE_COUNT)
        return ERR_FATAL;

    char addr_str[9] = {};
    memset(addr_str, 0, 9);

    uint32_t index_data;
    uint8_t  address_nbytes;

    switch (prop->record_type) {
        case S0_HEADER:
            return ERR_RECORD_NOT_SUPPORTED;

        case S1_DATA:
            memcpy(addr_str, line+4, 4);
            address_nbytes = 2;
            index_data = 8;
            break;
        case S2_DATA:
            memcpy(addr_str, line+4, 6);
            address_nbytes = 3;
            index_data = 10;
            break;
        case S3_DATA:
            memcpy(addr_str, line+4, 8);
            address_nbytes = 4;
            index_data = 12;
            break;

        case S4_RESERVED:
            return ERR_FATAL;

        case S5_COUNT:
        case S6_COUNT:
        case S7_START_ADDR:
        case S8_START_ADDR:
        case S9_START_ADDR:
            return ERR_RECORD_NOT_SUPPORTED;
    }

    prop->data_count = byte_count - (address_nbytes + 1);

    prop->address = vmcu_htoi(addr_str);

    for(uint32_t i = 0; i < prop->data_count; i++){

        char data_str[3] = {
                line[index_data],
                line[index_data + 1],
                '\0'
        };
        index_data   += 2;
        prop->data[i] = vmcu_htoi(data_str);
    }

    return 0;
}
