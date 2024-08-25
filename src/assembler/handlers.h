#ifndef VMCU_ASM_HANDLERS_H
#define VMCU_ASM_HANDLERS_H

#include <stdarg.h>
#include <inttypes.h>

#include <lib/instr.h>
#include <lib/rc.h>

vmcu_rc_t pack_and_assemble_nop(uint16_t* write_ptr, const va_list operands);
vmcu_rc_t assemble_nop(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src);
vmcu_rc_t pack_and_assemble_movw(uint16_t* write_ptr, const va_list operands);
vmcu_rc_t assemble_movw(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src);
vmcu_rc_t pack_and_assemble_mul(uint16_t* write_ptr, const va_list operands);
vmcu_rc_t assemble_mul(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src);
vmcu_rc_t pack_and_assemble_muls(uint16_t* write_ptr, const va_list operands);
vmcu_rc_t assemble_muls(uint16_t* write_ptr, const vmcu_operand_t* dest, const vmcu_operand_t* src);

#endif
