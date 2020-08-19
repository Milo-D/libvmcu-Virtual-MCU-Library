/* Instruction Decoder Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "disassembler/decoder.h"
#include "disassembler/opcode.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "collections/array.h"

#define FILE_INVALID_ERR "Could not open Hex File."
#define WRONG_FORM_ERR "File contains wrong Hex Format."
#define HEX_DATA_ERR "Could not manipulate Hex Data."
#define BYTE_INVALID_ERR "Found invalid Byte Count."
#define ILLEGAL_SYM_ERR "Found illegal symbols in Hex File."

/* Forward Declaration of Private Decoder Functions */

static int get_key_from_op(const int hex);
static int decode_hex_line(const char *line, array_t *buffer);
static int decode_eep_line(const char *line, array_t *buffer);
static int validate_hex(const char *line);

/* Public Functions of Decoder */

int decode_hex(const char *hex_file, array_t *buffer) {

    FILE *file;

    if((file = fopen(hex_file, "r")) == NULL)
        print_status(FILE_INVALID_ERR, true);

    size_t len;
    ssize_t bytes_read;

    char *line = NULL;

    while((bytes_read = getline(&line, &len, file)) != -1) {

        if(validate_hex(line) < 0)
            print_status(WRONG_FORM_ERR, true);

        decode_hex_line(line, buffer);
    }

    free(line);
    fclose(file);

    return 0;
}

int decode_eep(const char *hex_file, array_t *buffer) {

    FILE *file;

    if((file = fopen(hex_file, "r")) == NULL)
        return -1;

    size_t len;
    ssize_t bytes_read;

    char *line = NULL;

    while((bytes_read = getline(&line, &len, file)) != -1) {

        if(validate_hex(line) < 0)
            return -1;

        decode_eep_line(line, buffer);
    }

    free(line);
    fclose(file);

    return 0;
}

/* Private Functions of Decoder */

static int get_key_from_op(const int hex) {

    for(int i = 0; i < SET_SIZE; i++) {

        int match = 0;

        for(int j = 0; j < WORD; j++) {

            const int binary = ((0x01 << (WORD - j - 1)) & hex) >> (WORD - j - 1);

            if(!(opcode[i][j] == 0 || opcode[i][j] == 1)) {

                match += 1;

                if(match == WORD)
                    return i;

                continue;
            }

            if(binary != opcode[i][j])
                break;
            else
                match += 1;

            if(match == WORD)
                return i;
        }
    }

    return -1;
}

static int decode_hex_line(const char *line, array_t *buffer) {

    if(line[8] != '0')
        return -1;

    char *hex_line; char *bytes; char *addr;
    const int hex_len = strlen(line);

    if((hex_line = substr(line, 1, hex_len)) == NULL)
        print_status(HEX_DATA_ERR, true);

    if((bytes = substr(hex_line, 0, 1)) == NULL)
        print_status(HEX_DATA_ERR, true);

    if((addr = substr(hex_line, 2, 5)) == NULL)
        print_status(HEX_DATA_ERR, true);

    const int byte_count = htoi(bytes);
    const int s_addr = htoi(addr) / 2;

    int n = 0;

    for(int i = 0; i < (byte_count / 2); i++) {

        int instr, found;

        char current[5];
        swap_bytes(current, hex_line, i);

        if(strcmp(current, "") == 0)
            print_status(BYTE_INVALID_ERR, true);

        if((instr = htoi(current)) < 0)
            print_status(ILLEGAL_SYM_ERR, true);

        if((found = get_key_from_op(instr)) < 0) {

            char err[19 + 5] = "Could not decode 0x";
            strncat(err, current, 4);

            print_status(err, true);
        }

        if(buffer->top == buffer->size)
            buffer->size *= 2;

        struct _plain p = {

            .opcode = instr,
            .key = found,
            .addr = (s_addr + i)
        };

        array_push(buffer, (void*) &p, sizeof(plain_t));
    }

    free(hex_line);
    free(bytes);
    free(addr);

    return 0;
}

static int decode_eep_line(const char *line, array_t *buffer) {

    if(line[8] != '0')
        return -1;

    char *hex_line; char *bytes; char *addr;
    const int hex_len = strlen(line);

    if((hex_line = substr(line, 1, hex_len)) == 0)
        print_status(HEX_DATA_ERR, true);

    if((bytes = substr(hex_line, 0, 1)) == 0)
        print_status(HEX_DATA_ERR, true);

    if((addr = substr(hex_line, 2, 5)) == 0)
        print_status(HEX_DATA_ERR, true);

    const int byte_count = htoi(bytes);
    const int s_addr = htoi(addr);

    int n = 0;

    for(int i = 0; i < byte_count; i++) {

        const int low_nibble = (8 + (i * 2) + 1);
        const int high_nibble = (8 + (i * 2) + 0);

        if(low_nibble >= (hex_len - 1))
            return -1;

        char current[3];

        current[0] = hex_line[high_nibble];
        current[1] = hex_line[low_nibble];
        current[2] = '\0';

        const int eep_data = htoi(current);

        if(eep_data < 0)
            return -1;

        if(buffer->top == buffer->size)
            buffer->size *= 2;

        struct _eep e = {

            .value = eep_data,
            .addr = (s_addr + i)
        };

        array_push(buffer, (void*) &e, sizeof(eep_t));
    }

    free(hex_line);
    free(bytes);
    free(addr);

    return 0;
}

static int validate_hex(const char *line) {

    char *c;

    if(strlen(line) < 11)
        return -1;

    if((c = strchr(line, ':')) == NULL)
        return -1;

    return 0;
}
