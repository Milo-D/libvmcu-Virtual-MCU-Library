/* String Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "engine/include/analyzer/modules/string/string_analyzer.h"
#include "engine/include/analyzer/report/report.h"

#define is_printable(byte) ((0x01 <= byte) && (byte <= 0x7e))

/*
 * Definition of an ascii string in libvmcu:
 *
 * A sequence of bytes is a string if and only if
 *
 * --------------------------------------------------------------------------------------------------
 * [1] 0x01 <= bytes[i] <= 0x7e , for all i where i >= 0 ^ i < n, n = length of string (+ null byte)
 * [2] bytes[n] = 0x00 (null byte)
 * [3] n > 2 (null byte inclusive)
 * --------------------------------------------------------------------------------------------------
 *
 * We might add an extra condition in order to improve string analysis:
 *
 * [4] bytes[i] must not be visited by the controlflow graph
 *
 * A further improvement would be to iterate backwards and search for
 * printable bytes instead of null bytes.
 *
 * --- Encoding ---
 *
 * This submodule currently supports
 *
 * [x] ASCII
 * [ ] UTF16
 *
 * --- Improvements ---
 *
 * - reduce heap usage of this submodule in the future (maybe by using a slab)
 * - add support for UTF16 encoding
 *
 * */

/* Forward Declaration of static Functions */

static uint8_t* extract_bytes(const vmcu_report_t *report, uint64_t *size);
static uint64_t count_bytes(const vmcu_report_t *report);

static void mkstr(vmcu_string_t *str, uint8_t *bytes, uint64_t s, uint64_t e);

/* --- Extern --- */

int vmcu_analyze_strings(vmcu_report_t *report) {

    uint64_t size; uint64_t s = 0, slen = 0;

    uint8_t *bytes = extract_bytes(report, &size);
    report->string = malloc((size / 2 + 1) * sizeof(vmcu_string_t));

    for(uint64_t i = 0; i < size; i++) {

        if(is_printable(bytes[i]) == true) {

            s     = (slen == 0) ? i : s;
            slen += 1;

            continue;
        }

        if(bytes[i] == '\0' && slen > 1) {

            uint32_t n = report->n_string++;
            mkstr(&report->string[n], bytes, s, i);
        }

        s    = 0;
        slen = 0;
    }

    if(report->n_string == 0) {

        free(report->string);
        report->string = NULL;

        goto cleanup;
    }

    size_t total = report->n_string * sizeof(vmcu_string_t);
    report->string = realloc(report->string, total);

cleanup:
    free(bytes);
    return 0;
}

/* --- Static --- */

static uint8_t* extract_bytes(const vmcu_report_t *report, uint64_t *size) {

    *size = count_bytes(report);
    uint8_t *bytes = malloc(*size * sizeof(uint8_t));

    for(uint32_t i = 0, j = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];

        bytes[j++] = (instr->opcode & 0x000000ff) >>  0;
        bytes[j++] = (instr->opcode & 0x0000ff00) >>  8;

        if(instr->dword == false)
            continue;

        bytes[j++] = (instr->opcode & 0x00ff0000) >> 16;
        bytes[j++] = (instr->opcode & 0xff000000) >> 24;
    }

    return bytes;
}

static uint64_t count_bytes(const vmcu_report_t *report) {

    uint64_t acc = 0;

    for(uint32_t i = 0; i < report->progsize; i++)
        acc += (2 + (2 * (report->disassembly[i].dword)));

    return acc;
}

static void mkstr(vmcu_string_t *str, uint8_t *bytes, uint64_t s, uint64_t e) {

    str->bytes = malloc((e - s + 1) * sizeof(char));

    for(uint64_t i = s; i <= e; i++)
        str->bytes[i - s] = bytes[i];

    str->addr   = (s / 2);
    str->length = (e - s);
}



