#ifndef VMCU_DISASSEMBLER_H
#define VMCU_DISASSEMBLER_H

#include <inttypes.h>

#include "word.h"
#include "instr.h"
#include "engine.h"

/*
 * vmcu_disasm - disassembler interface #1
 * @e       engine object
 * @words   the vmcu_word_t array to disassemble
 * @n_words size of the @words array
 * @size    a pointer to the resulting size variable. NOT set to zero on failure
 * @retval  a list containing @size disassembled instructions, on failure NULL
 *
 * Configurable options:
 * - engine.disassembler.words_mutable 
 * */
vmcu_instr_t* vmcu_disasm(vmcu_engine_t* e, vmcu_word_t* words, const uint32_t n_words, uint32_t* size);

#endif
