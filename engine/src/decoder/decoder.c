/* Instruction Decoder Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/reader/ihex_reader.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine utilities)
#include "engine/include/misc/bitmanip.h"

/* Forward Declaration of static Decoder Functions */

static int get_opc_key(const uint32_t hex);
static bool is_dword(const uint32_t opcode);
static void copy_instr(vmcu_instr_t *dest, vmcu_instr_t *src);

/* --- Extern --- */

int vmcu_decode_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu) {

    uint32_t be = big_endian32(bytes);

    if(be > 0x0000ffff) {

        instr->key   = get_opc_key(high16(be));
        instr->dword = is_dword(high16(be));

    } else {

        instr->key   = get_opc_key(be);
        instr->dword = false;
    }

    instr->opcode    = be;
    instr->addr      = 0x0000;
    instr->mnem      = NULL;

    instr->exec      = (instr->key >= 0);

    instr->src.type  = VMCU_EMPTY;
    instr->dest.type = VMCU_EMPTY;

    return ((instr->dword == false) && (be > 0xffff)) ? -1 : 0;
}

vmcu_instr_t* vmcu_decode_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *read, *result;

    if((read = vmcu_read_ihex(hex_file, size)) == NULL)
        return NULL;

    const int32_t top = *size;
    result = malloc(top * sizeof(vmcu_instr_t));

    for(int i = 0, j = 0; i < top; i++, j++) {

        copy_instr(&result[j], &read[i]);

        if(result[j].key < 0)
            continue;

        if(result[j].dword == false)
            continue;

        if(i == top - 1) {

            free(read);
            free(result);

            return NULL;
        }

        result[j].opcode <<= 16;
        result[j].opcode |= read[++i].opcode;

        *size -= 1;
    }

    const size_t sz = sizeof(vmcu_instr_t);
    result = realloc(result, *size * sz);

    free(read);
    return result;
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

static void copy_instr(vmcu_instr_t *dest, vmcu_instr_t *src) {

    dest->opcode    = src->opcode;
    dest->addr      = src->addr;

    dest->dword     = is_dword(src->opcode);
    dest->key       = get_opc_key(src->opcode);
    dest->exec      = (dest->key >= 0);

    dest->mnem      = NULL;

    dest->src.type  = VMCU_EMPTY;
    dest->dest.type = VMCU_EMPTY;
}
