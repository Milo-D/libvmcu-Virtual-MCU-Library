/* Intel Hex Reader Implementation */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include <utils/utils.h>
#include <lib/engine.h>
#include <lib/word.h>
#include "ihex.h"

#define RECORD 8
#define DATA_RECORD '0'
#define IHEX_MIN_LENGTH 11

#define base(i) RECORD + (i * 4)

/* Forward Declaration of static Members */

typedef struct ihex_properties {

    uint32_t byte_count;
    uint32_t s_addr;
    uint8_t  record;
    char*    line;

} ihex_properties_t;

/* Forward Declaration of static Functions */

static bool verify_ihex_line(const char* line);
static uint32_t calculate_buffer_size(FILE* stream);
static int read_ihex_stream(FILE* stream, vmcu_word_t* words, uint32_t* size);
static int read_ihex_line(char* line, vmcu_word_t* words, uint32_t* size);
static int get_ihex_properties(char* line, ihex_properties_t* prop);

/* --- Extern --- */

vmcu_word_t* read_ihex(FILE* stream, uint32_t* size) {

    uint32_t     n;
    vmcu_word_t* words;

    if ((n = calculate_buffer_size(stream)) == 0)
        return NULL;

    if ((words = vmcu.cb.alloc(n * sizeof(vmcu_word_t))) == NULL)
        return NULL;

    if (read_ihex_stream(stream, words, size) < 0) {

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

    size_t     len;
    uint32_t   size = 0;
    char*      line = NULL;
    const long pos  = ftell(stream);

    while (getline(&line, &len, stream) != -1) {

        ihex_properties_t prop;

        if (get_ihex_properties(line, &prop) < 0) {

            size = 0;
            goto cleanup;
        }

        if (prop.record == DATA_RECORD)
            size += (prop.byte_count / 2); 
    }

cleanup:
    fseek(stream, pos, SEEK_SET);
    free(line);

    return size;
}

static bool verify_ihex_line(const char* line) {

    if (strlen(line) < IHEX_MIN_LENGTH)
        return false;

    if (strchr(line, ':') == NULL)
        return false;

    return true;
}

static int read_ihex_stream(FILE* stream, vmcu_word_t* words, uint32_t* size) {

    size_t len;
    char*  line = NULL;

    while (getline(&line, &len, stream) != -1) {

        if (read_ihex_line(line, words, size) < 0) {

            *size = 0;
            free(line);

            return -1;
        }
    }

    free(line);
    return 0;
}

static int read_ihex_line(char* line, vmcu_word_t* words, uint32_t* size) {

    ihex_properties_t prop;

    if (get_ihex_properties(line, &prop) < 0)
        return -1;

    if (prop.record != DATA_RECORD)
        return 0;

    for (uint32_t i = 0; i < (prop.byte_count / 2); i++) {

        int  error;
        char current[5] = { '\0' };

        if ((base(i) + 3) >= strlen(prop.line))
            return -1;

        if (vmcu.reader.endianness == VMCU_ENDIAN_LITTLE) {

            current[0] = prop.line[base(i) + 2];
            current[1] = prop.line[base(i) + 3];
            current[2] = prop.line[base(i) + 0];
            current[3] = prop.line[base(i) + 1];

        } else {

            strncpy(current, prop.line + base(i), 4);
        }

        words[*size] = (vmcu_word_t) {

            .raw  = htou64(current, &error),
            .addr = ((prop.s_addr / 2) + i)
        };

        if (error != 0)
            return -1;

        *size += 1;
    }

    return 0;
}

static int get_ihex_properties(char* line, ihex_properties_t* prop) {

    int  error1;
    int  error2;
    char bytes[3] = { '\0' };
    char addr [5] = { '\0' };

    if (verify_ihex_line(line) == false)
        return -1;

    strncpy(bytes, line + 1, 2);
    strncpy(addr,  line + 3, 4);

    *prop = (ihex_properties_t) {

        .byte_count = htou64(bytes, &error1),
        .s_addr     = htou64(addr,  &error2),
        .record     = line[RECORD],
        .line       = line + 1
    };

    return (error1 == 0 && error2 == 0) ? 0 : -1;
}

