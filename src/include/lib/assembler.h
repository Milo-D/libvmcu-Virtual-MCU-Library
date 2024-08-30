#ifndef VMCU_ASSEMBLER_H
#define VMCU_ASSEMBLER_H

#include <inttypes.h>
#include "instr.h"

/*
 * vmcu_asm - assembler interface
 * @write_ptr buffer to hold the assembled instruction (at least 16-bit in size)
 * @iid       numerical identifier of the desired instruction
 * @...       a variable number of operands, starting with the destination
 *            operand. Excess elements are ignored. The exact number and type of
 *            operands to provide depends on the specific instruction denoted by @iid
 * @retval    total number of resulting words (either 1 or 2), 0 on failure
 *
 * Configurable options:
 * - vmcu.assembler.expect_operand_structs
 * */
unsigned int vmcu_asm(uint16_t* write_ptr, const vmcu_iid_t iid, ...);

#endif
