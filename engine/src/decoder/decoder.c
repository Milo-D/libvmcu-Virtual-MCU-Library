/* Instruction Decoder Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine, readers)
#include "engine/include/reader/binary_buffer.h"
#include "engine/include/reader/reader.h"

// Project Headers (engine utilities)
#include "engine/include/misc/bitmanip.h"

#define ikey_is_32bit(k) (k == VMCU_IKEY_JMP)   || (k == VMCU_IKEY_CALL)  || \
                         (k == VMCU_IKEY_STS32) || (k == VMCU_IKEY_LDS32)

/* Forward Declaration of static Decoder Functions */

static vmcu_instr_t* decode_binary_buffer(vmcu_binary_buffer_t *bb, uint32_t *size);
static vmcu_binary_buffer_t** sort_binary_buffer(vmcu_binary_buffer_t *bb, const uint32_t size, uint32_t *n);
static uint32_t count_instructions(vmcu_binary_buffer_t **map, const uint32_t map_size);
static uint32_t find_max_addr(vmcu_binary_buffer_t *bb, const uint32_t size);

static VMCU_IKEY get_16bit_ikey(const uint16_t bytes);
static VMCU_IKEY get_32bit_ikey(const uint32_t bytes);

static bool is_32bit(const uint16_t bytes);
static void initialize_empty_instr(vmcu_instr_t *instr);

/* --- Extern --- */

int vmcu_decode_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu) {

    initialize_empty_instr(instr);

    if(bytes > 0x0000ffff) {

        instr->key   = get_32bit_ikey(bytes);
        instr->dword = (instr->key != VMCU_IKEY_DATA);

    } else {

        instr->key   = get_16bit_ikey(bytes);
        instr->dword = false;
    }

    instr->addr      = 0x0000;
    instr->opcode    = big_endian32(bytes);
    instr->exec      = (instr->key != VMCU_IKEY_DATA);

    return ((instr->dword == false) && (bytes > 0xffff)) ? -1 : 0;
}

vmcu_instr_t* vmcu_decode_file(const char *file, uint32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *result;
    vmcu_binary_buffer_t *bb;

    if((bb = vmcu_read_format(file, size)) == NULL)
        return NULL;

    if((result = decode_binary_buffer(bb, size)) == NULL)
        *size = 0;

    free(bb);
    return result;
}

/* --- Static --- */

static vmcu_instr_t* decode_binary_buffer(vmcu_binary_buffer_t *bb, uint32_t *size) {

    vmcu_instr_t *result;
    vmcu_binary_buffer_t **map;

    uint32_t instr_size = 0, map_size = 0;

    if((map = sort_binary_buffer(bb, *size, &map_size)) == NULL)
        return NULL;

    if((instr_size = count_instructions(map, map_size)) == 0)
        return NULL;

    if((result = malloc(instr_size * sizeof(vmcu_instr_t))) == NULL) {

        free(map);
        return NULL;
    }

    for(uint32_t i = 0, j = 0; i < map_size; i++) {

        vmcu_binary_buffer_t *raw = map[i];

        if(map[i] == NULL)
            continue;

        if(is_32bit(raw->bytes) == false) {

            vmcu_decode_bytes(raw->bytes, &result[j], NULL);

        } else if((i + 1) > map_size - 1) {

            vmcu_decode_bytes(raw->bytes, &result[j], NULL);

        } else if(map[i + 1] == NULL) {

            vmcu_decode_bytes(raw->bytes, &result[j], NULL);

        } else {

            uint32_t opc = (raw->bytes << 16) | (map[++i]->bytes);
            vmcu_decode_bytes(opc, &result[j], NULL);
        }

        result[j++].addr = raw->addr;
    }

    *size = instr_size;
    free(map);

    return result;
}

static vmcu_binary_buffer_t** sort_binary_buffer(vmcu_binary_buffer_t *bb, const uint32_t size, uint32_t *n) {

    vmcu_binary_buffer_t **map;
    *n = find_max_addr(bb, size) + 1;

    if((map = malloc(*n * sizeof(vmcu_binary_buffer_t*))) == NULL)
        return NULL;

    memset(map, 0x00, *n * sizeof(vmcu_binary_buffer_t*));

    for(uint32_t i = 0; i < size; i++)
        map[ bb[i].addr ] = &bb[i];

    return map;
}

static uint32_t count_instructions(vmcu_binary_buffer_t **map, const uint32_t map_size) {

    uint32_t n = map_size;

    for(uint32_t i = 0; i < map_size; i++) {

        if(map[i] == NULL) {

            n -= 1;
            continue;
        }

        if(is_32bit(map[i]->bytes) == false)
            continue;

        if((i + 1) > (map_size - 1))
            continue;

        if(map[i + 1] == NULL)
            continue;

        n -= 1;
        i += 1;
    }

    return n;
}

static uint32_t find_max_addr(vmcu_binary_buffer_t *bb, const uint32_t size) {

    uint32_t max = 0;

    for(uint32_t i = 0; i < size; i++) {

        if(max < bb[i].addr)
            max = bb[i].addr;
    }

    return max;
}

static VMCU_IKEY get_16bit_ikey(const uint16_t bytes) {

    const uint16_t be = big_endian16(bytes);

    for(int i = 0; i < VMCU_SET_SIZE; i++) {

        int match = 0;

        if(ikey_is_32bit(i) == true)
            continue;

        for(int j = 0; j < VMCU_WORD; j++) {

            const int binary = bit(be, (VMCU_WORD - j - 1));

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

    return VMCU_IKEY_DATA;
}

static VMCU_IKEY get_32bit_ikey(const uint32_t bytes) {

    switch(big_endian32(bytes) & 0xfe0e0000) {

        case 0x940c0000: return VMCU_IKEY_JMP;
        case 0x940e0000: return VMCU_IKEY_CALL;

        default: break;
    }

    switch(big_endian32(bytes) & 0xfe0f0000) {

        case 0x92000000: return VMCU_IKEY_STS32;
        case 0x90000000: return VMCU_IKEY_LDS32;

        default: break;
    }

    return VMCU_IKEY_DATA;
}

static bool is_32bit(const uint16_t bytes) {

    switch(big_endian16(bytes) & 0xfe0e) {

        case 0x940c: return true;
        case 0x940e: return true;

        default: break;
    }

    switch(big_endian16(bytes) & 0xfe0f) {

        case 0x9200: return true;
        case 0x9000: return true;

        default: break;
    }

    return false;
}

static void initialize_empty_instr(vmcu_instr_t *instr) {

    instr->key       = VMCU_IKEY_DATA;
    instr->opcode    = 0xffff;
    instr->addr      = 0x0000;
    instr->exec      = false;
    instr->dword     = false;

    instr->group     = VMCU_GROUP_NONE;

    memset(&instr->writes, 0x00, sizeof(vmcu_access_t));
    memset(&instr->reads,  0x00, sizeof(vmcu_access_t));

    instr->src.type  = VMCU_OPTYPE_NONE;
    instr->dest.type = VMCU_OPTYPE_NONE;

    memset(&instr->mnem,   '\0', sizeof(vmcu_mnemonic_t));
}
