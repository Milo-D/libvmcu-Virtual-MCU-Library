/* Intel Hex Reader Header */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine, readers)
#include "engine/include/reader/format/ihex.h"
#include "engine/include/reader/binary_buffer.h"

// Project Headers (engine utilities)
#include "engine/include/misc/filemanip.h"
#include "engine/include/misc/stringmanip.h"

#define RECORD 8
#define DATA_RECORD '0'
#define IHEX_MIN_LENGTH 11

#define base(i) RECORD + (i * 4)

/* Forward Declaration of static Members */

typedef struct ihex_properties {

    uint32_t byte_count;
    uint32_t s_addr;
    uint8_t record;

    char *line;

} ihex_properties_t;

/* Forward Declaration of static Functions */

static bool verify_ihex_line(const char *line);
static uint32_t estimate_buffer_size(const char *hex_file);

static int32_t read_ihex_file(const char *hex_file, vmcu_binary_buffer_t *bb, uint32_t *size);
static int32_t read_ihex_line(char *line, vmcu_binary_buffer_t *bb, uint32_t *size);

static int32_t get_ihex_properties(char *line, ihex_properties_t *prop);

/* --- Extern --- */

vmcu_binary_buffer_t * vmcu_read_ihex(const char *hex_file, uint32_t *size) {

    uint32_t n;
    vmcu_binary_buffer_t *bb;

    if((n = estimate_buffer_size(hex_file)) == 0)
        return NULL;

    if((bb = malloc(n * sizeof(vmcu_binary_buffer_t))) == NULL)
        return NULL;

    if(read_ihex_file(hex_file, bb, size) < 0) {

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

static uint32_t estimate_buffer_size(const char *hex_file) {

    long bytes;

    if((bytes = vmcu_fbytes(hex_file)) < 4)
        return 0;

    return (bytes / 4);
}

static bool verify_ihex_line(const char *line) {

    if(strlen(line) < IHEX_MIN_LENGTH)
        return false;

    if(strchr(line, ':') == NULL)
        return false;

    return true;
}

static int32_t read_ihex_file(const char *hex_file, vmcu_binary_buffer_t *bb, uint32_t *size) {

    FILE *f = NULL;
    size_t len; char *line = NULL;

    if((f = fopen(hex_file, "r")) == NULL)
        return -1;

    while(getline(&line, &len, f) != -1) {

        if(read_ihex_line(line, bb, size) < 0) {

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

static int32_t read_ihex_line(char *line, vmcu_binary_buffer_t *bb, uint32_t *size) {

    ihex_properties_t prop;

    if(get_ihex_properties(line, &prop) < 0)
        return -1;

    if(prop.record != DATA_RECORD)
        return 0;

    for(uint32_t i = 0; i < (prop.byte_count / 2); i++) {

        int64_t hex_bytes;

        if((base(i) + 3) >= strlen(prop.line))
            return -1;

        char current[5] = {

            prop.line[base(i) + 0],
            prop.line[base(i) + 1],
            prop.line[base(i) + 2],
            prop.line[base(i) + 3],
            '\0'
        };

        if(strcmp(current, "") == 0)
            return -1;

        if((hex_bytes = vmcu_htoi(current)) < 0)
            return -1;

        bb[*size] = (vmcu_binary_buffer_t) {

            .bytes  = hex_bytes,
            .addr   = ((prop.s_addr / 2) + i)
        };

        *size += 1;
    }

    return 0;
}

static int32_t get_ihex_properties(char *line, ihex_properties_t *prop) {

    if(verify_ihex_line(line) == false)
        return -1;

    char bytes[3] = { line[1], line[2], '\0' };
    char addr [5] = { line[3], line[4], line[5], line[6], '\0' };

    *prop = (ihex_properties_t) {

        .byte_count = vmcu_htoi(bytes),
        .s_addr     = vmcu_htoi(addr),
        .record     = line[RECORD],
        .line       = line + 1
    };

    return 0;
}










