/* Assembly Instruction Implementation */

// C++ Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "instructions/instructions.h"
#include "system/mcudef.h"
#include "system/system.h"
#include "misc/bitmanip.h"

void nop(system_t *sys, const int opcode) {

    sys_move_pc(sys, 1);
    sys->cycles += 1;

    return;
}

void movw(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) * 2;
    const int src = extract(opcode, 0, 4, 0) * 2;

    const int high = sys_read_gpr(sys, src);
    const int low = sys_read_gpr(sys, src + 1);

    sys_write_gpr(sys, dest, high);
    sys_write_gpr(sys, dest + 1, low);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void mul(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const uint8_t dest_val = sys_read_gpr(sys, dest);
    const uint8_t src_val = sys_read_gpr(sys, src);

    const uint16_t result = (dest_val * src_val);

    sys_write_sreg(sys, CF, bit(result, 15));
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, 0, (result & 0x00ff));
    sys_write_gpr(sys, 1, (result & 0xff00) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void muls(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + 16;

    const int16_t result = sys_read_gpr(sys, dest) * sys_read_gpr(sys, src);

    sys_write_sreg(sys, CF, bit(result, 15));
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, 0, (result & 0x00ff));
    sys_write_gpr(sys, 1, (result & 0xff00) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void mulsu(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const uint8_t src_val = (uint8_t) sys_read_gpr(sys, src);

    const int16_t result = (dest_val * src_val);

    sys_write_sreg(sys, CF, bit(result, 15));
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, 0, (result & 0x00ff));
    sys_write_gpr(sys, 1, (result & 0xff00) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void fmul(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    const uint8_t dest_val = sys_read_gpr(sys, dest);
    const uint8_t src_val = sys_read_gpr(sys, src);

    const uint16_t result = (dest_val * src_val);
    const uint16_t res_shift = (result << 1);

    const uint8_t rl = (res_shift & 0x00ff);
    const uint8_t rh = (res_shift & 0xff00) >> 8;

    sys_write_sreg(sys, CF, bit(result, 15));
    sys_write_sreg(sys, ZF, (res_shift == 0x0000));

    sys_write_gpr(sys, 0, rl);
    sys_write_gpr(sys, 1, rh);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void fmuls(system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;
    
    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);

    const int16_t result = (dest_val * src_val);
    const int16_t res_shift = (result << 1);

    const uint8_t rl = (res_shift & 0x00ff);
    const uint8_t rh = ((uint16_t) (res_shift & 0xff00)) >> 8;
    
    sys_write_sreg(sys, CF, bit(result, 15));
    sys_write_sreg(sys, ZF, (res_shift == 0x00));
    
    sys_write_gpr(sys, 0, rl);
    sys_write_gpr(sys, 1, rh);
    
    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void fmulsu(system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;
    
    const int8_t dest_val = sys_read_gpr(sys, dest);
    const uint8_t src_val = sys_read_gpr(sys, src);
    
    const int16_t result = (dest_val * src_val);
    const int16_t res_shift = (result << 1);
    
    const uint8_t rl = (res_shift & 0x00ff);
    const uint8_t rh = ((uint16_t) (res_shift & 0xff00)) >> 8;
    
    sys_write_sreg(sys, CF, bit(result, 15));
    sys_write_sreg(sys, ZF, (res_shift == 0x00));
    
    sys_write_gpr(sys, 0, rl);
    sys_write_gpr(sys, 1, rh);
    
    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void ldi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sys_write_gpr(sys, dest + 16, src);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void rjmp(system_t *sys, const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 11) & offs) != 0x00) {

        offs = comp(offs, 12);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void jmp(system_t *sys, const int opcode) {

    const int addr = (opcode & 0x1ffff) 
                   + (opcode & 0x1f00000);

    sys_set_pc(sys, addr);
    sys->cycles += 3;
}

void ijmp(system_t *sys, const int opcode) {

    const uint8_t zl = sys_read_data(sys, ZL);
    const uint8_t zh = sys_read_data(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    sys_set_pc(sys, addr);

    sys->cycles += 2;
}

void mov(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_gpr(sys, dest, value);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void dec(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int8_t value = sys_read_gpr(sys, dest);

    const int8_t result = value - 0x01;

    int8_t vf_res = !bit(result, 7) * bit(result, 6) * bit(result, 5) * bit(result, 4);
    vf_res *= bit(result, 3) * bit(result, 2) * bit(result, 1) * bit(result, 0);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void inc(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int8_t value = sys_read_gpr(sys, dest);

    const int8_t result = value + 1;

    int8_t vf_res = bit(result, 7) * !bit(result, 6) * !bit(result, 5) * !bit(result, 4);
    vf_res *= !bit(result, 3) * !bit(result, 2) * !bit(result, 1) * !bit(result, 0);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void add(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);
    const int8_t result = dest_val + src_val;

    int8_t vf_res = bit(dest_val, 7) * bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * !bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * !bit(result, 7));
    cf_res += !bit(result, 7) * bit(dest_val, 7);

    int8_t hf_res = (bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * !bit(result, 3));
    hf_res += !bit(result, 3) * bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void adc(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);

    const bool carry = sys_read_sreg(sys, CF);
    const int8_t result = (dest_val + src_val + carry);

    int8_t vf_res = bit(dest_val, 7) * bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * !bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * !bit(result, 7));
    cf_res += !bit(result, 7) * bit(dest_val, 7);

    int8_t hf_res = (bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * !bit(result, 3));
    hf_res += !bit(result, 3) * bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void adiw(system_t *sys, const int opcode) {

    const int dest = (2 * extract(opcode, 4, 6, 0)) + 24;
    const uint8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 6, 8, 4);

    const uint8_t rdl = sys_read_gpr(sys, dest);
    const uint8_t rdh = sys_read_gpr(sys, dest + 1);

    const uint16_t result = (rdl + (rdh << 8)) + src;

    const int8_t vf_res = !bit(rdh, 7) * bit(result, 15);
    const int8_t cf_res = bit(rdh, 7) * !bit(result, 15);
    const int8_t nf_res = bit(result, 15);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x0000));

    sys_write_gpr(sys, dest, (result & 0x00ff));
    sys_write_gpr(sys, dest + 1, (result & 0xff00) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sub(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);
    const int8_t result = dest_val - src_val;

    int8_t vf_res = bit(dest_val, 7) * !bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void subi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t result = dest_val - src;

    int8_t vf_res = bit(dest_val, 7) * !bit(src, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src, 7)) + (bit(src, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src, 3)) + (bit(src, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sbc(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);

    const bool carry = sys_read_sreg(sys, CF);
    const bool zero = sys_read_sreg(sys, ZF);

    const int8_t result = (dest_val - src_val - carry);

    int8_t vf_res = bit(dest_val, 7) * !bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00) * zero);

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sbci(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const bool carry = sys_read_sreg(sys, CF);
    const bool zero = sys_read_sreg(sys, ZF);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t result = (dest_val - src - carry);

    int8_t vf_res = bit(dest_val, 7) * !bit(src, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src, 7)) + (bit(src, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src, 3)) + (bit(src, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00) * zero);

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sbiw(system_t *sys, const int opcode) {

    const int dest = (2 * extract(opcode, 4, 6, 0)) + 24;
    const uint8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 6, 8, 4);

    const uint8_t rdl = sys_read_gpr(sys, dest);
    const uint8_t rdh = sys_read_gpr(sys, dest + 1);

    const uint16_t result = (rdl + (rdh << 8)) - src;

    const int8_t vf_res = bit(result, 15) * !bit(rdh, 7);
    const int8_t cf_res = bit(result, 15) * !bit(rdh, 7);
    const int8_t nf_res = bit(result, 15);

    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x0000));

    sys_write_gpr(sys, dest, (result & 0x00ff));
    sys_write_gpr(sys, dest + 1, (result & 0xff00) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void push(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    const int8_t val = sys_read_gpr(sys, src);

    sys_push_stack(sys, val);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void pop(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int8_t val = sys_pop_stack(sys);

    sys_write_gpr(sys, dest, val);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void in(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);

    const int8_t val = sys_read_data(sys, GPR_SIZE + src);
    sys_write_gpr(sys, dest, val);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void out(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    const int src = extract(opcode, 4, 9, 0);

    const int8_t val = sys_read_gpr(sys, src);
    sys_write_data(sys, GPR_SIZE + dest, val);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sbis(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bitpos = extract(opcode, 0, 3, 0);

    const uint8_t val = sys_read_data(sys, GPR_SIZE + dest);

    if(bit(val, bitpos) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = sys_get_pc(sys);
    progmem_t *p = sys_read_instr(sys, pc + 1);

    sys_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void sbic(system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 3, 8, 0);
    const int bitpos = extract(opcode, 0, 3, 0);

    const uint8_t val = sys_read_data(sys, GPR_SIZE + dest);

    if(bit(val, bitpos) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = sys_get_pc(sys);
    progmem_t *p = sys_read_instr(sys, pc + 1);

    sys_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void sbrc(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bit = extract(opcode, 0, 3, 0);

    const uint8_t val = sys_read_gpr(sys, dest);

    if(bit(val, bit) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = sys_get_pc(sys);
    progmem_t *p = sys_read_instr(sys, pc + 1);

    sys_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void sbrs(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bit = extract(opcode, 0, 3, 0);

    const uint8_t val = sys_read_gpr(sys, dest);

    if(bit(val, bit) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = sys_get_pc(sys);
    progmem_t *p = sys_read_instr(sys, pc + 1);

    sys_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void cpse(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);

    if(dest_val != src_val) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = sys_get_pc(sys);
    progmem_t *p = sys_read_instr(sys, pc + 1);

    sys_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void eor(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const uint8_t val_dest = sys_read_gpr(sys, dest);
    const uint8_t val_src = sys_read_gpr(sys, src);

    const uint8_t result = (val_dest ^ val_src);
    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, 0x00);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ 0x00);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ld_x(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t xl = sys_read_gpr(sys, XL);
    const uint8_t xh = sys_read_gpr(sys, XH);

    const int8_t data = sys_read_data(sys, (xh << 8) + xl);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ld_xi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t xl = sys_read_gpr(sys, XL);
    const uint8_t xh = sys_read_gpr(sys, XH);

    const int8_t data = sys_read_data(sys, (xh << 8) + xl);
    const uint16_t post_x = ((xh << 8) + xl) + 0x01;

    sys_write_gpr(sys, dest, data);
    sys_write_gpr(sys, XL, (0x00ff & post_x));
    sys_write_gpr(sys, XH, (0xff00 & post_x) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void ld_dx(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t xl = sys_read_gpr(sys, XL);
    const uint8_t xh = sys_read_gpr(sys, XH);

    const uint16_t pre_x = ((xh << 8) + xl) - 0x01;
    const int8_t data = sys_read_data(sys, pre_x);

    sys_write_gpr(sys, XL, (0x00ff & pre_x));
    sys_write_gpr(sys, XH, (0xff00 & pre_x) >> 8);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void ld_y(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const int8_t data = sys_read_data(sys, (yh << 8) + yl);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ld_yi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const int8_t data = sys_read_data(sys, (yh << 8) + yl);
    const uint16_t post_y = ((yh << 8) + yl) + 0x01;

    sys_write_gpr(sys, dest, data);
    sys_write_gpr(sys, YL, (0x00ff & post_y));
    sys_write_gpr(sys, YH, (0xff00 & post_y) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void ld_dy(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const uint16_t pre_y = ((yh << 8) + yl) - 0x01;
    const int8_t data = sys_read_data(sys, pre_y);

    sys_write_gpr(sys, YL, (0x00ff & pre_y));
    sys_write_gpr(sys, YH, (0xff00 & pre_y) >> 8);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void ldd_yq(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const int8_t data = sys_read_data(sys, (yh << 8) + yl + disp);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void ldd_zq(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t data = sys_read_data(sys, (zh << 8) + zl + disp);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void ld_z(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t data = sys_read_data(sys, (zh << 8) + zl);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ld_zi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t data = sys_read_data(sys, (zh << 8) + zl);
    const uint16_t post_z = ((zh << 8) + zl) + 0x01;

    sys_write_gpr(sys, dest, data);
    sys_write_gpr(sys, ZL, (0x00ff & post_z));
    sys_write_gpr(sys, ZH, (0xff00 & post_z) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void ld_dz(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t pre_z = ((zh << 8) + zl) - 0x01;
    const int8_t data = sys_read_data(sys, pre_z);

    sys_write_gpr(sys, ZL, (0x00ff & pre_z));
    sys_write_gpr(sys, ZH, (0xff00 & pre_z) >> 8);
    sys_write_gpr(sys, dest, data);

    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void st_x(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t xl = sys_read_gpr(sys, XL);
    const uint8_t xh = sys_read_gpr(sys, XH);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_data(sys, (xh << 8) + xl, value);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_xi(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t xl = sys_read_gpr(sys, XL);
    const uint8_t xh = sys_read_gpr(sys, XH);

    const int8_t value = sys_read_gpr(sys, src);
    const uint16_t post_x = ((xh << 8) + xl) + 0x01;

    sys_write_data(sys, (xh << 8) + xl, value);
    sys_write_gpr(sys, XL, (0x00ff & post_x));
    sys_write_gpr(sys, XH, (0xff00 & post_x) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_dx(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t xl = sys_read_gpr(sys, XL);
    const uint8_t xh = sys_read_gpr(sys, XH);

    const uint16_t pre_x = ((xh << 8) + xl) - 0x01;
    const int8_t value = sys_read_gpr(sys, src);

    sys_write_data(sys, pre_x, value);
    sys_write_gpr(sys, XL, (0x00ff & pre_x));
    sys_write_gpr(sys, XH, (0xff00 & pre_x) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_y(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_data(sys, (yh << 8) + yl, value);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_yi(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const int8_t value = sys_read_gpr(sys, src);
    const uint16_t post_y = ((yh << 8) + yl) + 0x01;

    sys_write_data(sys, (yh << 8) + yl, value);
    sys_write_gpr(sys, YL, (0x00ff & post_y));
    sys_write_gpr(sys, YH, (0xff00 & post_y) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_dy(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const uint16_t pre_y = ((yh << 8) + yl) - 0x01;
    const int8_t value = sys_read_gpr(sys, src);

    sys_write_data(sys, pre_y, value);
    sys_write_gpr(sys, YL, (0x00ff & pre_y));
    sys_write_gpr(sys, YH, (0xff00 & pre_y) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void std_yq(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t yl = sys_read_gpr(sys, YL);
    const uint8_t yh = sys_read_gpr(sys, YH);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_data(sys, ((yh << 8) + yl) + disp, value);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_z(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_data(sys, (zh << 8) + zl, value);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_zi(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t value = sys_read_gpr(sys, src);
    const uint16_t post_z = ((zh << 8) + zl) + 0x01;

    sys_write_data(sys, (zh << 8) + zl, value);
    sys_write_gpr(sys, ZL, (0x00ff & post_z));
    sys_write_gpr(sys, ZH, (0xff00 & post_z) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void st_dz(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t pre_z = ((zh << 8) + zl) - 0x01;
    const int8_t value = sys_read_gpr(sys, src);

    sys_write_data(sys, pre_z, value);
    sys_write_gpr(sys, ZL, (0x00ff & pre_z));
    sys_write_gpr(sys, ZH, (0xff00 & pre_z) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void std_zq(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_data(sys, ((zh << 8) + zl) + disp, value);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void sts(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 8, 0);
    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);

    const int8_t value = sys_read_gpr(sys, src + 16);

    int addr = (~(0x10 & dest) << 3) | ((0x10 & dest) << 2) | 
                ((0x40 & dest) >> 1) | ((0x20 & dest) >> 1) |
                ((0x0f & dest));

    sys_write_data(sys, addr, value);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sts32(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 0, 16, 0);
    const int src = extract(opcode, 20, 25, 0);

    const int8_t value = sys_read_gpr(sys, src);
    sys_write_data(sys, (uint16_t) dest, value);

    sys_move_pc(sys, 2);
    sys->cycles += 2;
}

void lds(system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 4, 8, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);
    
    int addr = (~(0x10 & src) << 3) | ((0x10 & src) << 2) | 
                ((0x40 & src) >> 1) | ((0x20 & src) >> 1) |
                ((0x0f & opcode));

    const int8_t value = sys_read_data(sys, addr);
    sys_write_gpr(sys, dest + 16, value);
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void lds32(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 20, 25, 0);
    const int src = extract(opcode, 0, 16, 0);

    const int8_t value = sys_read_data(sys, src);
    sys_write_gpr(sys, dest, value);

    sys_move_pc(sys, 2);
    sys->cycles += 2;
}

void xch(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const int8_t reg_val = sys_read_gpr(sys, src);
    const int8_t data_val = sys_read_data(sys, (zh << 8) + zl);

    sys_write_data(sys, (zh << 8) + zl, reg_val);
    sys_write_gpr(sys, src, data_val);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void brne(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, ZF) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void breq(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, ZF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brge(system_t *sys, const int opcode) {

    if((sys_read_sreg(sys, NF) ^ sys_read_sreg(sys, VF)) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brpl(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, NF) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brlo(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, CF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brlt(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, SF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brcc(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, CF) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brcs(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, CF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brvs(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, VF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brts(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, TF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brtc(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, TF) == 0x01) {

        sys_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brmi(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, NF) == 0x00) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brhc(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, HF) == 0x01) {

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brhs(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, HF) == 0x00) {

        sys_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brid(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, IF) == 0x01) {

        sys_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brie(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, IF) == 0x00) {

        sys_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void brvc(system_t *sys, const int opcode) {

    if(sys_read_sreg(sys, VF) == 0x01) {

        sys_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = sys_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        sys_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    sys_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void rcall(system_t *sys, const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    const int pc = sys_get_pc(sys);

    switch(PC_BIT) {

        case 16:

            sys_push_stack(sys, (pc + 1) & 0x00ff);
            sys_push_stack(sys, ((pc + 1) & 0xff00) >> 8);

            sys->cycles += 3;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    if(((0x01 << 11) & offs) != 0x00) {

        offs = comp(offs, 12);
        sys_set_pc(sys, pc - offs + 1);
        
        return;
    }

    sys_set_pc(sys, pc + offs + 1);
}

void ret(system_t *sys, const int opcode) {

    uint8_t pcl, pch, pcm;

    switch(PC_BIT) {

        case 16:
            
            pch = sys_pop_stack(sys);
            pcl = sys_pop_stack(sys);

            sys_set_pc(sys, (pch << 8) + pcl);
            sys->cycles += 4;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }
}

void reti(system_t *sys, const int opcode) {

    uint8_t pcl, pch, pcm;

    switch(PC_BIT) {

        case 16:
            
            pch = sys_pop_stack(sys);
            pcl = sys_pop_stack(sys);

            sys_set_pc(sys, (pch << 8) + pcl);
            sys->cycles += 4;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    sys_write_sreg(sys, IF, 0x01);
}

void icall(system_t *sys, const int opcode) {

    const int pc = sys_get_pc(sys);

    switch(PC_BIT) {

        case 16:

            sys_push_stack(sys, (pc + 1) & 0x00ff);
            sys_push_stack(sys, ((pc + 1) & 0xff00) >> 8);

            sys->cycles += 3;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    const uint8_t zl = sys_read_data(sys, ZL);
    const uint8_t zh = sys_read_data(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    sys_set_pc(sys, addr);
}

void call(system_t *sys, const int opcode) {

    const int addr = (opcode & 0x1ffff) 
                   + (opcode & 0x1f00000);

    const int pc = sys_get_pc(sys);

    switch(PC_BIT) {

        case 16:

            sys_push_stack(sys, (pc + 2) & 0x00ff);
            sys_push_stack(sys, ((pc + 2) & 0xff00) >> 8);

            sys->cycles += 4;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    sys_set_pc(sys, addr);
}

void cp(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t value = sys_read_gpr(sys, dest);
    const int8_t comp = sys_read_gpr(sys, src);
    const int8_t result = value - comp; 

    int8_t cf_res = (!(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(value, 7));

    int8_t vf_res = (bit(value, 7) * !bit(comp, 7) * !bit(result, 7));
    vf_res += (!bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (!bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * !bit(value, 3));

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void cpi(system_t *sys, const int opcode) {

    const int reg = extract(opcode, 4, 8, 0);
    const uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const uint8_t value = sys_read_gpr(sys, 16 + reg);
    const int8_t result = value - comp;

    int8_t cf_res = (!(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(value, 7));

    int8_t vf_res = (bit(value, 7) * !bit(comp, 7) * !bit(result, 7));
    vf_res += (!bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (!bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * !bit(value, 3));

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void cpc(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t value = sys_read_gpr(sys, dest);
    const int8_t comp = sys_read_gpr(sys, src);
    const bool carry = sys_read_sreg(sys, CF);

    const int8_t result = (value - comp - carry);

    int8_t cf_res = (!(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(value, 7));

    int8_t vf_res = (bit(value, 7) * !bit(comp, 7) * !bit(result, 7));
    vf_res += (!bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (!bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * !bit(value, 3));

    const int8_t nf_res = bit(result, 7);
    const int8_t zf_res = (result == 0x00) ? sys_read_sreg(sys, ZF) : 0x00;
    
    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, zf_res);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void lsr(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = sys_read_gpr(sys, dest);
    const uint8_t result = (value >> 1);

    const int8_t cf_res = bit(value, 0);
    const int8_t vf_res = cf_res ^ 0;

    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, NF, 0x00);
    sys_write_sreg(sys, SF, vf_res ^ 0);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void asr(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = sys_read_gpr(sys, dest);
    const uint8_t result = (value >> 1);

    const int8_t cf_res = bit(value, 0);
    const int8_t vf_res = cf_res ^ 0;

    sys_write_sreg(sys, CF, cf_res);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, NF, 0x00);
    sys_write_sreg(sys, SF, vf_res ^ 0);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ror(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = sys_read_gpr(sys, dest);
    const bool carry = sys_read_sreg(sys, CF);
    
    const bool lsb = (value & 0x01);
    const uint8_t result = ((value >> 1) | (carry << 7));

    const bool nf_res = (result & (0x01 << 7));
    const bool vf_res = (nf_res ^ lsb);

    sys_write_sreg(sys, CF, lsb);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void swap(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = sys_read_gpr(sys, dest);
    const uint8_t result = ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ori(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const uint8_t val = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const int8_t dest_val = sys_read_gpr(sys, 16 + dest);
    const int8_t result = dest_val | val;

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, 0x00);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ 0);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, 16 + dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void or_asm(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);

    sys_write_gpr(sys, dest, dest_val | src_val);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void and_asm(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const int8_t src_val = sys_read_gpr(sys, src);
    const int8_t result = dest_val & src_val;

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, 0x00);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, (nf_res ^ 0));
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void andi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int value = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const int8_t dest_val = sys_read_gpr(sys, dest);
    const uint8_t result = dest_val & value;

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, 0x00);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, (nf_res ^ 0));
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void las(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    const uint8_t value = sys_read_gpr(sys, src);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint8_t data = sys_read_data(sys, addr);

    const uint8_t result = (value | data);

    sys_write_gpr(sys, src, data);
    sys_write_data(sys, addr, result);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void lac(system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    const uint8_t value = sys_read_gpr(sys, src);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint8_t data = sys_read_data(sys, addr);

    const uint8_t result = ((0xff - value) * data);

    sys_write_gpr(sys, src, data);
    sys_write_data(sys, addr, result);

    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void lat(system_t *sys, const int opcode) {
    
    const int src = extract(opcode, 4, 9, 0);
    const uint8_t value = sys_read_gpr(sys, src);
    
    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint8_t data = sys_read_data(sys, addr);
    
    const uint8_t result = (value ^ data);
    
    sys_write_gpr(sys, src, data);
    sys_write_data(sys, addr, result);
    
    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void com(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int8_t value = sys_read_gpr(sys, dest);
    const int8_t result = (0xff - value);

    const int8_t nf_res = bit(result, 7);

    sys_write_sreg(sys, VF, 0x00);
    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, SF, nf_res ^ 0);
    sys_write_sreg(sys, ZF, (result == 0x00));

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void neg(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = sys_read_gpr(sys, dest);
    const uint8_t result = ~value + 0x01;

    const int8_t zf_res = (result == 0x00);
    const int8_t vf_res = (result == 0x80);

    const int8_t nf_res = bit(result, 7);
    const int8_t hf_res = bit(result, 3) | bit(value, 3);

    sys_write_sreg(sys, NF, nf_res);
    sys_write_sreg(sys, VF, vf_res);
    sys_write_sreg(sys, HF, hf_res);
    sys_write_sreg(sys, CF, !zf_res);
    sys_write_sreg(sys, SF, nf_res ^ vf_res);
    sys_write_sreg(sys, ZF, zf_res);

    sys_write_gpr(sys, dest, result);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void bld(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    const bool t_flag = sys_read_sreg(sys, TF);
    const int8_t val = sys_read_gpr(sys, dest);

    if(t_flag == 0x00) {

        sys_write_gpr(sys, dest, (val & ~(0x01 << bpos)));

        sys_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    sys_write_gpr(sys, dest, (val | (0x01 << bpos)));

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void bst(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    const int8_t val = sys_read_gpr(sys, dest);
    sys_write_sreg(sys, TF, ((0x01 << bpos) & val) >> bpos);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sbi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bpos = extract(opcode, 0, 3, 0);
    
    sys_set_sfr(sys, (GPR_SIZE + dest), bpos);
    
    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void cbi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    sys_clear_sfr(sys, (GPR_SIZE + dest), bpos);
    
    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void lpm(system_t *sys, const int opcode) {

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint16_t word = sys_read_flash(sys, addr >> 1);

    const uint8_t lsb = (addr & 0x01) ? 8 : 0;
    const uint8_t byte = (word >> lsb) & 0xff;

    sys_write_gpr(sys, 0, byte);
    
    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void lpm_z(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint16_t word = sys_read_flash(sys, addr >> 1);

    const uint8_t lsb = (addr & 0x01) ? 8 : 0;
    const uint8_t byte = (word >> lsb) & 0xff;

    sys_write_gpr(sys, dest, byte);
    
    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void lpm_zi(system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = sys_read_gpr(sys, ZL);
    const uint8_t zh = sys_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint16_t word = sys_read_flash(sys, addr >> 1);

    const uint8_t lsb = (addr & 0x01) ? 8 : 0;
    const uint8_t byte = (word >> lsb) & 0xff;

    const uint16_t post_z = ((zh << 8) + zl) + 0x01;

    sys_write_gpr(sys, dest, byte);
    sys_write_gpr(sys, ZL, (0x00ff & post_z));
    sys_write_gpr(sys, ZH, (0xff00 & post_z) >> 8);

    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void eicall(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 4;
}

void eijmp(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 2;
}

void elpm(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void elpm_z(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void elpm_zi(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 3;
}

void des(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void sleep(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void wdr(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void break_asm(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void spm(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void spm_zi(system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void ses(system_t *sys, const int opcode) {

    sys_write_sreg(sys, SF, 0x01);
    sys_move_pc(sys, 1);
    
    sys->cycles += 1;
}

void set(system_t *sys, const int opcode) {

    sys_write_sreg(sys, TF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void sev(system_t *sys, const int opcode) {

    sys_write_sreg(sys, VF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void sez(system_t *sys, const int opcode) {

    sys_write_sreg(sys, ZF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void seh(system_t *sys, const int opcode) {

    sys_write_sreg(sys, HF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void sec(system_t *sys, const int opcode) {

    sys_write_sreg(sys, CF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void sei(system_t *sys, const int opcode) {

    sys_write_sreg(sys, IF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void sen(system_t *sys, const int opcode) {

    sys_write_sreg(sys, NF, 0x01);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void cls(system_t *sys, const int opcode) {

    sys_write_sreg(sys, SF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void clt(system_t *sys, const int opcode) {

    sys_write_sreg(sys, TF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void clv(system_t *sys, const int opcode) {

    sys_write_sreg(sys, VF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void clz(system_t *sys, const int opcode) {

    sys_write_sreg(sys, ZF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void clh(system_t *sys, const int opcode) {

    sys_write_sreg(sys, HF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void clc(system_t *sys, const int opcode) {

    sys_write_sreg(sys, CF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void cli(system_t *sys, const int opcode) {

    sys_write_sreg(sys, IF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void cln(system_t *sys, const int opcode) {

    sys_write_sreg(sys, NF, 0x00);
    sys_move_pc(sys, 1);

    sys->cycles += 1;
}

void bclr(system_t *sys, const int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);
    sys_write_sreg(sys, s_bit, 0x00);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void bset(system_t *sys, const int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);
    sys_write_sreg(sys, s_bit, 0x01);

    sys_move_pc(sys, 1);
    sys->cycles += 1;
}

void (*instructions[INSTR_MAX]) (system_t *sys, const int opcode) = { 

    nop, 
    movw, 
    mul,
    muls, 
    mulsu, 
    fmul, 
    fmuls,
    fmulsu,
    ldi, 
    rjmp, 
    jmp, 
    ijmp, 
    mov, 
    dec, 
    inc, 
    add, 
    adc, 
    adiw, 
    sub, 
    subi, 
    sbc, 
    sbci, 
    sbiw, 
    push, 
    pop, 
    in, 
    out, 
    sbis, 
    sbic,
    sbrc, 
    sbrs, 
    cpse, 
    eor, 
    ld_x, 
    ld_xi, 
    ld_dx, 
    ld_y, 
    ld_yi, 
    ld_dy, 
    ldd_yq, 
    ldd_zq, 
    ld_z, 
    ld_zi, 
    ld_dz,
    st_x, 
    st_xi, 
    st_dx, 
    st_y,
    st_yi, 
    st_dy,
    std_yq, 
    st_z,
    st_zi,
    st_dz,
    std_zq, 
    sts, 
    sts32, 
    lds,
    lds32, 
    xch, 
    brne, 
    breq, 
    brge, 
    brpl, 
    brlo, 
    brlt, 
    brcc, 
    brcs, 
    brvs, 
    brts, 
    brtc, 
    brmi, 
    brhc,
    brhs,
    brid,
    brie,
    brvc,
    rcall, 
    ret, 
    reti, 
    icall, 
    call, 
    cp, 
    cpi, 
    cpc, 
    lsr, 
    asr, 
    ror, 
    swap, 
    ori, 
    or_asm, 
    and_asm, 
    andi, 
    las, 
    lac, 
    lat,
    com, 
    neg, 
    bld, 
    bst, 
    sbi,
    cbi,
    lpm, 
    lpm_z, 
    lpm_zi, 
    eicall,
    eijmp,
    elpm,
    elpm_z,
    elpm_zi,
    des,
    sleep,
    wdr,
    break_asm,
    spm,
    spm_zi,
    ses, 
    set, 
    sev, 
    sez, 
    seh, 
    sec, 
    sei, 
    sen, 
    cls, 
    clt, 
    clv, 
    clz, 
    clh, 
    clc, 
    cli, 
    cln, 
    bclr, 
    bset
};




