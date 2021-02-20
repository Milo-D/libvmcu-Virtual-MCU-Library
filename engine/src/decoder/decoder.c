/* Instruction Decoder Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine utilities)
#include "engine/include/misc/memmanip.h"
#include "engine/include/misc/stringmanip.h"
#include "engine/include/misc/bitmanip.h"
#include "engine/include/misc/filemanip.h"
#include "engine/include/collections/array.h"

#define RECORD 8
#define DATA_RECORD '0'

#define base(i) RECORD + (i * 4)

/* Forward Declaration of static Decoder Members */

typedef struct ihex_properties {

    int s_addr;
    int byte_count;

    char *line;

} ihex_properties_t;

/* Forward Declaration of static Decoder Functions */

static array_t* preprocess(const char *hex_file);

static int preprocess_line(const char *line, array_t *buffer);
static int decode_eep_line(const char *line, array_t *buffer);

static int get_ihex_properties(const char *line, ihex_properties_t *buffer);

static int get_opc_key(const uint32_t hex);
static void create_plain(vmcu_plain_t *dest, const vmcu_plain_t *src);

static bool is_dword(const uint32_t opcode);

static int validate_hex(const char *line);

/* --- Extern --- */

int vmcu_decode_bytes(const uint32_t bytes, vmcu_plain_t *p) {

    uint32_t be = big_endian32(bytes);

    if(be > 0x0000ffff) {

        p->key   = get_opc_key(high16(be));
        p->dword = is_dword(high16(be));

    } else {

        p->key   = get_opc_key(be);
        p->dword = false;
    }

    p->opcode    = be;
    p->addr      = 0x0000;
    p->mnem      = NULL;

    p->exec      = (p->key >= 0);

    p->src.type  = VMCU_EMPTY;
    p->dest.type = VMCU_EMPTY;

    return ((p->dword == false) && (be > 0xffff)) ? -1 : 0;
}

vmcu_plain_t* vmcu_decode_ihex(const char *hex_file, int32_t *size) {

    array_t *temp;

    if((temp = preprocess(hex_file)) == NULL)
        return NULL;

    size_t sz = temp->top * sizeof(vmcu_plain_t);
    vmcu_plain_t *decoded = malloc(sz);

    *size = temp->top;

    for(int i = 0, j = 0; i < temp->top; i++, j++) {

        vmcu_plain_t *p = array_at(temp, i);
        create_plain(&decoded[j], p);

        if(decoded[j].key < 0)
            continue;

        if((decoded[j].dword == true) && (i == temp->top - 1)) {

            array_dtor(temp);
            free(decoded);

            return NULL;
        }

        if(decoded[j].dword == false)
            continue;

        vmcu_plain_t *post = array_at(temp, ++i);
        decoded[j].opcode = (p->opcode << 16) | post->opcode;

        *size -= 1;
    }

    sz = *size * sizeof(vmcu_plain_t);
    decoded = realloc(decoded, sz);

    array_dtor(temp);
    return decoded;
}

int vmcu_decode_eep(const char *hex_file, array_t *buffer) {

    FILE *file;

    if((file = fopen(hex_file, "r")) == NULL)
        return -1;

    size_t len;
    char *line = NULL;

    while(getline(&line, &len, file) != -1) {

        if(validate_hex(line) < 0)
            return -1;

        if(line[RECORD] != DATA_RECORD)
            continue;

        if(decode_eep_line(line, buffer) < 0)
            return -1;
    }

    free(line);
    fclose(file);

    return 0;
}

/* --- Static --- */

static array_t* preprocess(const char *hex_file) {

    size_t len;
    char *line = NULL; long bytes;

    if((bytes = fbytes(hex_file)) <= 0)
        return NULL;

    FILE *file = fopen(hex_file, "r");
    array_t *buffer = array_ctor((bytes / 4), NULL, NULL);

    while(getline(&line, &len, file) != -1) {

        if(validate_hex(line) < 0) {

            array_dtor(buffer);
            fclose(file);
            free(line);

            return NULL;
        }

        if(line[RECORD] != DATA_RECORD)
            continue;

        if(preprocess_line(line, buffer) < 0) {

            array_dtor(buffer);
            fclose(file);
            free(line);

            return NULL;
        }
    }

    free(line);
    fclose(file);

    if(buffer->top <= 0) {

        array_dtor(buffer);
        return NULL;
    }

    return buffer;
}

static int preprocess_line(const char *line, array_t *buffer) {

    ihex_properties_t ihex;

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

        vmcu_plain_t p = {

            .opcode = big_endian16(instr),
            .addr   = ((ihex.s_addr / 2) + i)
        };

        size_t sz = sizeof(vmcu_plain_t);
        array_push(buffer, (void*) &p, sz);
    }

    free(ihex.line);
    return 0;

error:
    free(ihex.line);
    return -1;
}

static int decode_eep_line(const char *line, array_t *buffer) {

    ihex_properties_t ihex;
    const size_t hex_len = strlen(line);

    if(get_ihex_properties(line, &ihex) < 0)
        return -1;

    for(int i = 0; i < ihex.byte_count; i++) {

        const int low_nibble  = (8 + (i * 2) + 1);
        const int high_nibble = (8 + (i * 2) + 0);

        if(low_nibble >= (hex_len - 1))
            goto error;

        char current[3];

        current[0] = ihex.line[high_nibble];
        current[1] = ihex.line[low_nibble];
        current[2] = '\0';

        const int eep_data = htoi(current);

        if(eep_data < 0)
            goto error;

        if(buffer->top == buffer->size)
            buffer->size *= 2;

        vmcu_eep_t e = {

            .value = eep_data,
            .addr = (ihex.s_addr + i)
        };

        size_t sz = sizeof(vmcu_eep_t);
        array_push(buffer, (void*) &e, sz);
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

static int get_opc_key(const uint32_t hex) {

    for(int i = 0; i < VMCU_SET_SIZE; i++) {

        int match = 0;

        for(int j = 0; j < VMCU_WORD; j++) {

            const int binary = bit(hex, (VMCU_WORD - j - 1));

            if(!(vmcu_opcode[i][j] == 0 || vmcu_opcode[i][j] == 1)) {

                match += 1;

                if(match == VMCU_WORD)
                    return i;

                continue;
            }

            if(binary != vmcu_opcode[i][j])
                break;
            else
                match += 1;

            if(match == VMCU_WORD)
                return i;
        }
    }

    return -1;
}

static void create_plain(vmcu_plain_t *dest, const vmcu_plain_t *src) {

    dest->opcode    = src->opcode;
    dest->addr      = src->addr;

    dest->dword     = is_dword(src->opcode);
    dest->key       = get_opc_key(src->opcode);
    dest->exec      = (dest->key >= 0);

    dest->mnem      = NULL;

    dest->src.type  = VMCU_EMPTY;
    dest->dest.type = VMCU_EMPTY;
}

static bool is_dword(const uint32_t opcode) {

    switch(opcode & 0xfe0e) {

        case 0x940c: return true;   /* 32-bit jmp  */
        case 0x940e: return true;   /* 32-bit call */

        default: break;
    }

    switch(opcode & 0xfe0f) {

        case 0x9200: return true;   /* 32-bit sts */
        case 0x9000: return true;   /* 32-bit lds */

        default: break;
    }

    return false;
}

static int validate_hex(const char *line) {

    if(strlen(line) < 11)
        return -1;

    if(strchr(line, ':') == NULL)
        return -1;

    return 0;
}
