#ifndef VMCU_ASM_TABLE_H
#define VMCU_ASM_TABLE_H

#include <stdarg.h>
#include <inttypes.h>

#include <lib/instr.h>
#include <lib/rc.h>

/*
 * todo: move to handlers.h?
 * */
typedef vmcu_rc_t (*asm_handler_t)(uint16_t*, const vmcu_operand_t*, const vmcu_operand_t*);

/*
 * todo: move to handlers.h?
 * */
typedef vmcu_rc_t (*unpacked_asm_handler_t)(uint16_t*, const va_list);

/*
 * a single entry in the asm_table
 * */
typedef struct asm_table_entry {

    asm_handler_t          handler;
    unpacked_asm_handler_t handler_unpacked;
    unsigned int           n_operands;
    unsigned int           n_words;

} asm_table_entry_t;

/*
 * lookup asm table entry corresponding to given instruction id @iid
 * */
asm_table_entry_t* get_asm_table_entry(const vmcu_iid_t iid);

#endif
