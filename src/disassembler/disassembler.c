/* Implements the disassembler.h API */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <lib/disassembler.h>
#include "opcode_table.h"

static uint32_t count_32bit_instructions(vmcu_word_t* words, const uint32_t n_words) {

    uint32_t n_32bit = 0;

    for (uint32_t i = 0; i < n_words; i++) {

        if (is_32bit_instruction(words[i].raw) == true) {

            n_32bit += 1;
            i       += 1;
        }
    }

    return n_32bit;
}

static int compare_words(const void* a, const void* b) {

    return (((vmcu_word_t*) a)->addr - ((vmcu_word_t*) b)->addr);
}

/* --- Exposed --- */

vmcu_instr_t* vmcu_disasm(vmcu_engine_t* e, vmcu_word_t* words, const uint32_t n_words, uint32_t* size) {

    vmcu_instr_t* instructions;
    vmcu_word_t*  copy;

    if (e->disassembler.words_mutable == false) {

        copy = malloc(n_words * sizeof(vmcu_word_t));
        memcpy(copy, words, n_words * sizeof(vmcu_word_t));

        words = copy;
    }

    qsort(words, n_words, sizeof(vmcu_word_t), compare_words);
    *size = n_words - count_32bit_instructions(words, n_words);

    if ((instructions = malloc(*size * sizeof(vmcu_instr_t))) == NULL)
        goto vmcu_disasm_failure;

    memset(instructions, 0, *size * sizeof(vmcu_instr_t));

    for (uint32_t i = 0, instr_index = 0; i < n_words; i++) {

        const vmcu_word_t* w0 = &words[i];
        vmcu_word_t*       w1 = NULL;

        if (is_32bit_instruction(w0->raw) == true) {

            if (i + 1 >= n_words) {

                free(instructions);
                instructions = NULL;
                break;
            }

            if (words[i + 1].addr - w0->addr != 1) {

                free(instructions);
                instructions = NULL;
                break;
            }
 
            w1 = &words[i++];
        }

        disasm_handler_t disasm_handler = get_disasm_handler(w0->raw);
        disasm_handler(&instructions[instr_index++], w0, w1);
    }

vmcu_disasm_failure:
    if (e->disassembler.words_mutable == false)
        free(copy);

    return instructions; 
}
