/* Instruction Decoder Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "disassembler/decoder.h"
#include "disassembler/opcode.h"
#include "disassembler/plain.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "misc/bitmanip.h"
#include "misc/filemanip.h"
#include "collections/array.h"

#define FILE_INVALID_ERR "Could not open Hex File."
#define WRONG_FORM_ERR "File contains wrong Hex Format."
#define HEX_DATA_ERR "Could not manipulate Hex Data."
#define BYTE_INVALID_ERR "Found invalid Byte Count."
#define ILLEGAL_SYM_ERR "Found illegal symbols in Hex File."
#define DWORD_MISSING_ERR "Detected 32-bit opcode but found 16-bit."

#define RECORD 8
#define DATA_RECORD '0'

/* Forward Declaration of Private Decoder Functions */

static int preprocess(const char *hex_file, array_t *buffer);
static int preprocess_line(const char *line, array_t *buffer);
static int decode_eep_line(const char *line, array_t *buffer);

static int get_opc_key(const int hex);
static bool is_dword(const int opcode);
static int validate_hex(const char *line);
static char* create_err(const int opcode);

/* Public Functions of Decoder */

int decode_hex(const char *hex_file, array_t *buffer) {

    long bytes;

    if((bytes = fbytes(hex_file)) < 0)
        print_status(FILE_INVALID_ERR, true);

    array_t *temp = array_ctor((bytes / 4), NULL, NULL);

    if(preprocess(hex_file, temp) < 0)
        print_status(WRONG_FORM_ERR, true);

    for(int i = 0; i < (buffer->size = temp->top); i++) {

        plain_t *p = (plain_t*) array_at(temp, i);
        p->dword = is_dword(p->opcode);

        if((p->key = get_opc_key(p->opcode)) < 0) {

            char *err = create_err(p->opcode);
            print_status(err, true);
        }

        if((p->dword == true) && (i == temp->top - 1))
            print_status(DWORD_MISSING_ERR, true);

        if(p->dword == true) {

            plain_t *post = (plain_t*) array_at(temp, ++i);
            p->opcode = ((p->opcode << 16) | post->opcode);
        }

        p->exec = true;
        array_push(buffer, (void*) p, sizeof(plain_t));
    }

    buffer->size = buffer->top;
    array_dtor(temp);
    
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

        if(line[RECORD] != DATA_RECORD)
            continue;

        decode_eep_line(line, buffer);
    }

    free(line);
    fclose(file);

    return 0;
}

/* Private Functions of Decoder */

static int preprocess(const char *hex_file, array_t *buffer) {

    size_t len;
    ssize_t bytes_read;

    char *line = NULL;

    FILE *file = fopen(hex_file, "r");

    while((bytes_read = getline(&line, &len, file)) != -1) {

        if(validate_hex(line) < 0)
            print_status(WRONG_FORM_ERR, true);

        if(line[RECORD] != DATA_RECORD)
            continue;

        preprocess_line(line, buffer);
    }

    free(line);
    fclose(file);
}

static int preprocess_line(const char *line, array_t *buffer) {

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

    for(int i = 0; i < (byte_count / 2); i++) {

        char current[5]; int instr;
        swap_bytes(current, hex_line, i);

        if(strcmp(current, "") == 0)
            print_status(BYTE_INVALID_ERR, true);

        if((instr = htoi(current)) < 0)
            print_status(ILLEGAL_SYM_ERR, true);

        struct _plain p = {

            .opcode = instr,
            .addr = (s_addr + i),
            .key = 0,

            .mnem = NULL,
            .exec = false,
            .dword = false
        };

        array_push(buffer, (void*) &p, sizeof(plain_t));
    }

    free(hex_line);
    free(bytes);
    free(addr);

    return 0;
}

static int decode_eep_line(const char *line, array_t *buffer) {

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

static int get_opc_key(const int hex) {

    for(int i = 0; i < SET_SIZE; i++) {

        int match = 0;

        for(int j = 0; j < WORD; j++) {

            const int binary = bit(hex, (WORD - j - 1));

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

static bool is_dword(const int opcode) {

    if((opcode & 0xfe0e) == 0x940c)
        return true;

    return false;
}

static int validate_hex(const char *line) {

    char *c;

    if(strlen(line) < 11)
        return -1;

    if((c = strchr(line, ':')) == NULL)
        return -1;

    return 0;
}

static char* create_err(const int opcode) {

    char high[3];
    to_hex((opcode & 0x00ff), high);

    char low[3];
    to_hex(((opcode & 0xff00) >> 8), low);

    char *err = malloc((19 + 5) * sizeof(char));
    sprintf(err, "Could not decode 0x%s%s", high, low);

    return err;
}
