/* Intel Hex Reader Header */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/reader/ihex_reader.h"
#include "engine/include/analyzer/report/plain.h"

// Project Headers (engine utilities)
#include "engine/include/misc/filemanip.h"
#include "engine/include/misc/bitmanip.h"
#include "engine/include/misc/memmanip.h"
#include "engine/include/misc/stringmanip.h"

#define RECORD 8
#define DATA_RECORD '0'
#define IHEX_MINLEN 11

#define base(i) RECORD + (i * 4)

/* Forward Declaration of static Members */

typedef struct ihex_properties {

    int s_addr;
    int byte_count;

    char *line;

} ihex_properties_t;

/* Forward Declaration of static Functions */

static bool check_ihex(const char *line);
static int read_ihex_line(char *line, vmcu_plain_t *buffer, int32_t *size);
static int get_ihex_properties(const char *line, ihex_properties_t *buffer);

/* --- Extern --- */

vmcu_plain_t* vmcu_read_ihex(const char *hex_file, int32_t *size) {

    long bytes; *size = 0;

    if((bytes = fbytes(hex_file)) < 4)
        return NULL;

    FILE *file = fopen(hex_file, "r");
    size_t len; char *line = NULL;

    const size_t sz = sizeof(vmcu_plain_t);
    vmcu_plain_t *buffer = malloc((bytes / 4) * sz);

    while(getline(&line, &len, file) != -1) {

        if(check_ihex(line) == false)
            goto err;

        if(line[RECORD] != DATA_RECORD)
            continue;

        if(read_ihex_line(line, buffer, size) < 0)
            goto err;
    }

    if(*size == 0)
        goto err;

    /* adjusting size */
    buffer = realloc(buffer, *size * sz);

    free(line);
    fclose(file);

    return buffer;

err:
    fclose(file);
    free(buffer);
    free(line);

    return NULL;
}

/* --- Static --- */

static bool check_ihex(const char *line) {

    if(strlen(line) < IHEX_MINLEN)
        return false;

    if(strchr(line, ':') == NULL)
        return false;

    return true;
}

static int read_ihex_line(char *line, vmcu_plain_t *buffer, int32_t *size) {

    ihex_properties_t ihex; int32_t top = *size;

    if(get_ihex_properties(line, &ihex) < 0)
        return -1;

    for(int i = 0; i < (ihex.byte_count / 2); i++) {

        int instr;

        if((base(i) + 3) >= strlen(ihex.line))
            goto error;

        char current[5] = {

            ihex.line[base(i) + 0],
            ihex.line[base(i) + 1],
            ihex.line[base(i) + 2],
            ihex.line[base(i) + 3],
            '\0'
        };

        if(strcmp(current, "") == 0)
            goto error;

        if((instr = htoi(current)) < 0)
            goto error;

        buffer[top + i] = (vmcu_plain_t) {

            .opcode = big_endian16(instr),
            .addr   = ((ihex.s_addr / 2 ) + i)
        };

        *size += 1;
    }

    free(ihex.line);
    return 0;

error:
    free(ihex.line);
    return -1;
}

static int get_ihex_properties(const char *line, ihex_properties_t *buffer) {

    char *bytes; char *addr;
    const size_t hex_len = strlen(line);

    if((buffer->line = substr(line, 1, hex_len)) == NULL)
        return -1;

    if((bytes = substr(buffer->line, 0, 1)) == NULL) {

        free(buffer->line);
        return -1;
    }

    if((addr = substr(buffer->line, 2, 5)) == NULL) {

        nfree(2, buffer->line, bytes);
        return -1;
    }

    buffer->s_addr = htoi(addr);
    buffer->byte_count = htoi(bytes);

    nfree(2, bytes, addr);
    return 0;
}










