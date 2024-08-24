#ifndef VMCU_OPCODE_TABLE_H
#define VMCU_OPCODE_TABLE_H

#include <stdbool.h>
#include <inttypes.h>

#include <lib/instr.h>

typedef struct vmcu_word vmcu_word_t;

/*
 * disassembly handler for a single instruction (todo: move this to handlers.h?)
 * */
typedef void (*disasm_handler_t)(vmcu_instr_t*, const vmcu_word_t*, const vmcu_word_t*);

/*
 * lookup the corresponding disasm_handler_t routine
 * for a given 16-bit opcode @bytes
 * */
disasm_handler_t get_disasm_handler(const uint16_t bytes);

/*
 * quick lookup whether @bytes belongs to an
 * 32-bit instruction
 * */
bool is_32bit_instruction(const uint16_t bytes);

#endif
