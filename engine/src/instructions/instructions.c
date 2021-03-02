/* Assembly Instruction Implementation */

// C++ Headers
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/instructions/instructions.h"
#include "engine/include/system/system.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"
#include "engine/include/misc/bitmanip.h"

void execute_nop(vmcu_system_t *sys, const int opcode) {

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;

    return;
}

void execute_movw(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) * 2;
    const int src = extract(opcode, 0, 4, 0) * 2;

    const int high = vmcu_system_read_gpr(sys, src);
    const int low = vmcu_system_read_gpr(sys, src + 1);

    vmcu_system_write_gpr(sys, dest, high);
    vmcu_system_write_gpr(sys, dest + 1, low);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_mul(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const uint8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const uint8_t src_val = vmcu_system_read_gpr(sys, src);

    const uint16_t result = (dest_val * src_val);

    vmcu_system_write_sreg(sys, CF, bit(result, 15));
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, 0, (result & 0x00ff));
    vmcu_system_write_gpr(sys, 1, (result & 0xff00) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_muls(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + 16;

    const int16_t result = vmcu_system_read_gpr(sys, dest) * vmcu_system_read_gpr(sys, src);

    vmcu_system_write_sreg(sys, CF, bit(result, 15));
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, 0, (result & 0x00ff));
    vmcu_system_write_gpr(sys, 1, (result & 0xff00) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_mulsu(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const uint8_t src_val = (uint8_t) vmcu_system_read_gpr(sys, src);

    const int16_t result = (dest_val * src_val);

    vmcu_system_write_sreg(sys, CF, bit(result, 15));
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, 0, (result & 0x00ff));
    vmcu_system_write_gpr(sys, 1, (result & 0xff00) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_fmul(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    const uint8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const uint8_t src_val = vmcu_system_read_gpr(sys, src);

    const uint16_t result = (dest_val * src_val);
    const uint16_t res_shift = (result << 1);

    const uint8_t rl = (res_shift & 0x00ff);
    const uint8_t rh = (res_shift & 0xff00) >> 8;

    vmcu_system_write_sreg(sys, CF, bit(result, 15));
    vmcu_system_write_sreg(sys, ZF, (res_shift == 0x0000));

    vmcu_system_write_gpr(sys, 0, rl);
    vmcu_system_write_gpr(sys, 1, rh);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_fmuls(vmcu_system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;
    
    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);

    const int16_t result = (dest_val * src_val);
    const int16_t res_shift = (result << 1);

    const uint8_t rl = (res_shift & 0x00ff);
    const uint8_t rh = ((uint16_t) (res_shift & 0xff00)) >> 8;
    
    vmcu_system_write_sreg(sys, CF, bit(result, 15));
    vmcu_system_write_sreg(sys, ZF, (res_shift == 0x00));
    
    vmcu_system_write_gpr(sys, 0, rl);
    vmcu_system_write_gpr(sys, 1, rh);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_fmulsu(vmcu_system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;
    
    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const uint8_t src_val = vmcu_system_read_gpr(sys, src);
    
    const int16_t result = (dest_val * src_val);
    const int16_t res_shift = (result << 1);
    
    const uint8_t rl = (res_shift & 0x00ff);
    const uint8_t rh = ((uint16_t) (res_shift & 0xff00)) >> 8;
    
    vmcu_system_write_sreg(sys, CF, bit(result, 15));
    vmcu_system_write_sreg(sys, ZF, (res_shift == 0x00));
    
    vmcu_system_write_gpr(sys, 0, rl);
    vmcu_system_write_gpr(sys, 1, rh);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_ldi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    vmcu_system_write_gpr(sys, dest + 16, src);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_rjmp(vmcu_system_t *sys, const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 11) & offs) != 0x00) {

        offs = comp(offs, 12);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_jmp(vmcu_system_t *sys, const int opcode) {

    const int addr = (opcode & 0x1ffff) +
                    ((opcode & 0x1f00000) >> 3);

    vmcu_system_set_pc(sys, addr);
    sys->cycles += 3;
}

void execute_ijmp(vmcu_system_t *sys, const int opcode) {

    const uint8_t zl = vmcu_system_read_data(sys, ZL);
    const uint8_t zh = vmcu_system_read_data(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    vmcu_system_set_pc(sys, addr);

    sys->cycles += 2;
}

void execute_mov(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_gpr(sys, dest, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_dec(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int8_t value = vmcu_system_read_gpr(sys, dest);

    const int8_t result = value - 0x01;

    int8_t vf_res = !bit(result, 7) * bit(result, 6) * bit(result, 5) * bit(result, 4);
    vf_res *= bit(result, 3) * bit(result, 2) * bit(result, 1) * bit(result, 0);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_inc(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int8_t value = vmcu_system_read_gpr(sys, dest);

    const int8_t result = value + 1;

    int8_t vf_res = bit(result, 7) * !bit(result, 6) * !bit(result, 5) * !bit(result, 4);
    vf_res *= !bit(result, 3) * !bit(result, 2) * !bit(result, 1) * !bit(result, 0);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_add(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);
    const int8_t result = dest_val + src_val;

    int8_t vf_res = bit(dest_val, 7) * bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * !bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * !bit(result, 7));
    cf_res += !bit(result, 7) * bit(dest_val, 7);

    int8_t hf_res = (bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * !bit(result, 3));
    hf_res += !bit(result, 3) * bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_adc(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);

    const bool carry = vmcu_system_read_sreg(sys, CF);
    const int8_t result = (dest_val + src_val + carry);

    int8_t vf_res = bit(dest_val, 7) * bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * !bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * !bit(result, 7));
    cf_res += !bit(result, 7) * bit(dest_val, 7);

    int8_t hf_res = (bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * !bit(result, 3));
    hf_res += !bit(result, 3) * bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_adiw(vmcu_system_t *sys, const int opcode) {

    const int dest = (2 * extract(opcode, 4, 6, 0)) + 24;
    const uint8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 6, 8, 4);

    const uint8_t rdl = vmcu_system_read_gpr(sys, dest);
    const uint8_t rdh = vmcu_system_read_gpr(sys, dest + 1);

    const uint16_t result = (rdl + (rdh << 8)) + src;

    const int8_t vf_res = !bit(rdh, 7) * bit(result, 15);
    const int8_t cf_res = bit(rdh, 7) * !bit(result, 15);
    const int8_t nf_res = bit(result, 15);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x0000));

    vmcu_system_write_gpr(sys, dest, (result & 0x00ff));
    vmcu_system_write_gpr(sys, dest + 1, (result & 0xff00) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sub(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);
    const int8_t result = dest_val - src_val;

    int8_t vf_res = bit(dest_val, 7) * !bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_subi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t result = dest_val - src;

    int8_t vf_res = bit(dest_val, 7) * !bit(src, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src, 7)) + (bit(src, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src, 3)) + (bit(src, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sbc(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);

    const bool carry = vmcu_system_read_sreg(sys, CF);
    const bool zero = vmcu_system_read_sreg(sys, ZF);

    const int8_t result = (dest_val - src_val - carry);

    int8_t vf_res = bit(dest_val, 7) * !bit(src_val, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00) * zero);

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sbci(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const bool carry = vmcu_system_read_sreg(sys, CF);
    const bool zero = vmcu_system_read_sreg(sys, ZF);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t result = (dest_val - src - carry);

    int8_t vf_res = bit(dest_val, 7) * !bit(src, 7) * !bit(result, 7);
    vf_res += !bit(dest_val, 7) * bit(src, 7) * bit(result, 7);

    int8_t cf_res = (!bit(dest_val, 7) * bit(src, 7)) + (bit(src, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(dest_val, 7));

    int8_t hf_res = (!bit(dest_val, 3) * bit(src, 3)) + (bit(src, 3) * bit(result, 3));
    hf_res += bit(result, 3) * !bit(dest_val, 3);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00) * zero);

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sbiw(vmcu_system_t *sys, const int opcode) {

    const int dest = (2 * extract(opcode, 4, 6, 0)) + 24;
    const uint8_t src = extract(opcode, 0, 4, 0) + extract(opcode, 6, 8, 4);

    const uint8_t rdl = vmcu_system_read_gpr(sys, dest);
    const uint8_t rdh = vmcu_system_read_gpr(sys, dest + 1);

    const uint16_t result = (rdl + (rdh << 8)) - src;

    const int8_t vf_res = bit(result, 15) * !bit(rdh, 7);
    const int8_t cf_res = bit(result, 15) * !bit(rdh, 7);
    const int8_t nf_res = bit(result, 15);

    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x0000));

    vmcu_system_write_gpr(sys, dest, (result & 0x00ff));
    vmcu_system_write_gpr(sys, dest + 1, (result & 0xff00) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_push(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    const int8_t val = vmcu_system_read_gpr(sys, src);

    vmcu_system_push_stack(sys, val);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_pop(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int8_t val = vmcu_system_pop_stack(sys);

    vmcu_system_write_gpr(sys, dest, val);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_in(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);

    const int8_t val = vmcu_system_read_data(sys, GPR_SIZE + src);
    vmcu_system_write_gpr(sys, dest, val);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_out(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    const int src = extract(opcode, 4, 9, 0);

    const int8_t val = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, GPR_SIZE + dest, val);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sbis(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bitpos = extract(opcode, 0, 3, 0);

    const uint8_t val = vmcu_system_read_data(sys, GPR_SIZE + dest);

    if(bit(val, bitpos) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = vmcu_system_get_pc(sys);
    vmcu_progmem_t *p = vmcu_system_read_progmem(sys, pc + 1);

    vmcu_system_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void execute_sbic(vmcu_system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 3, 8, 0);
    const int bitpos = extract(opcode, 0, 3, 0);

    const uint8_t val = vmcu_system_read_data(sys, GPR_SIZE + dest);

    if(bit(val, bitpos) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = vmcu_system_get_pc(sys);
    vmcu_progmem_t *p = vmcu_system_read_progmem(sys, pc + 1);

    vmcu_system_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void execute_sbrc(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bit = extract(opcode, 0, 3, 0);

    const uint8_t val = vmcu_system_read_gpr(sys, dest);

    if(bit(val, bit) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = vmcu_system_get_pc(sys);
    vmcu_progmem_t *p = vmcu_system_read_progmem(sys, pc + 1);

    vmcu_system_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void execute_sbrs(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bit = extract(opcode, 0, 3, 0);

    const uint8_t val = vmcu_system_read_gpr(sys, dest);

    if(bit(val, bit) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = vmcu_system_get_pc(sys);
    vmcu_progmem_t *p = vmcu_system_read_progmem(sys, pc + 1);

    vmcu_system_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void execute_cpse(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);

    if(dest_val != src_val) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    const int pc = vmcu_system_get_pc(sys);
    vmcu_progmem_t *p = vmcu_system_read_progmem(sys, pc + 1);

    vmcu_system_set_pc(sys, pc + 2 + p->dword);
    sys->cycles += (2 + p->dword);
}

void execute_eor(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const uint8_t val_dest = vmcu_system_read_gpr(sys, dest);
    const uint8_t val_src = vmcu_system_read_gpr(sys, src);

    const uint8_t result = (val_dest ^ val_src);
    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ 0x00);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ld_x(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t xl = vmcu_system_read_gpr(sys, XL);
    const uint8_t xh = vmcu_system_read_gpr(sys, XH);

    const int8_t data = vmcu_system_read_data(sys, (xh << 8) + xl);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ld_xi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t xl = vmcu_system_read_gpr(sys, XL);
    const uint8_t xh = vmcu_system_read_gpr(sys, XH);

    const int8_t data = vmcu_system_read_data(sys, (xh << 8) + xl);
    const uint16_t post_x = ((xh << 8) + xl) + 0x01;

    vmcu_system_write_gpr(sys, dest, data);
    vmcu_system_write_gpr(sys, XL, (0x00ff & post_x));
    vmcu_system_write_gpr(sys, XH, (0xff00 & post_x) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_ld_dx(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t xl = vmcu_system_read_gpr(sys, XL);
    const uint8_t xh = vmcu_system_read_gpr(sys, XH);

    const uint16_t pre_x = ((xh << 8) + xl) - 0x01;
    const int8_t data = vmcu_system_read_data(sys, pre_x);

    vmcu_system_write_gpr(sys, XL, (0x00ff & pre_x));
    vmcu_system_write_gpr(sys, XH, (0xff00 & pre_x) >> 8);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_ld_y(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const int8_t data = vmcu_system_read_data(sys, (yh << 8) + yl);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ld_yi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const int8_t data = vmcu_system_read_data(sys, (yh << 8) + yl);
    const uint16_t post_y = ((yh << 8) + yl) + 0x01;

    vmcu_system_write_gpr(sys, dest, data);
    vmcu_system_write_gpr(sys, YL, (0x00ff & post_y));
    vmcu_system_write_gpr(sys, YH, (0xff00 & post_y) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_ld_dy(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const uint16_t pre_y = ((yh << 8) + yl) - 0x01;
    const int8_t data = vmcu_system_read_data(sys, pre_y);

    vmcu_system_write_gpr(sys, YL, (0x00ff & pre_y));
    vmcu_system_write_gpr(sys, YH, (0xff00 & pre_y) >> 8);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_ldd_yq(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const int8_t data = vmcu_system_read_data(sys, (yh << 8) + yl + disp);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_ldd_zq(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t data = vmcu_system_read_data(sys, (zh << 8) + zl + disp);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_ld_z(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t data = vmcu_system_read_data(sys, (zh << 8) + zl);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ld_zi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t data = vmcu_system_read_data(sys, (zh << 8) + zl);
    const uint16_t post_z = ((zh << 8) + zl) + 0x01;

    vmcu_system_write_gpr(sys, dest, data);
    vmcu_system_write_gpr(sys, ZL, (0x00ff & post_z));
    vmcu_system_write_gpr(sys, ZH, (0xff00 & post_z) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_ld_dz(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t pre_z = ((zh << 8) + zl) - 0x01;
    const int8_t data = vmcu_system_read_data(sys, pre_z);

    vmcu_system_write_gpr(sys, ZL, (0x00ff & pre_z));
    vmcu_system_write_gpr(sys, ZH, (0xff00 & pre_z) >> 8);
    vmcu_system_write_gpr(sys, dest, data);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_st_x(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t xl = vmcu_system_read_gpr(sys, XL);
    const uint8_t xh = vmcu_system_read_gpr(sys, XH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, (xh << 8) + xl, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_xi(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t xl = vmcu_system_read_gpr(sys, XL);
    const uint8_t xh = vmcu_system_read_gpr(sys, XH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    const uint16_t post_x = ((xh << 8) + xl) + 0x01;

    vmcu_system_write_data(sys, (xh << 8) + xl, value);
    vmcu_system_write_gpr(sys, XL, (0x00ff & post_x));
    vmcu_system_write_gpr(sys, XH, (0xff00 & post_x) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_dx(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t xl = vmcu_system_read_gpr(sys, XL);
    const uint8_t xh = vmcu_system_read_gpr(sys, XH);

    const uint16_t pre_x = ((xh << 8) + xl) - 0x01;
    const int8_t value = vmcu_system_read_gpr(sys, src);

    vmcu_system_write_data(sys, pre_x, value);
    vmcu_system_write_gpr(sys, XL, (0x00ff & pre_x));
    vmcu_system_write_gpr(sys, XH, (0xff00 & pre_x) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_y(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, (yh << 8) + yl, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_yi(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    const uint16_t post_y = ((yh << 8) + yl) + 0x01;

    vmcu_system_write_data(sys, (yh << 8) + yl, value);
    vmcu_system_write_gpr(sys, YL, (0x00ff & post_y));
    vmcu_system_write_gpr(sys, YH, (0xff00 & post_y) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_dy(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const uint16_t pre_y = ((yh << 8) + yl) - 0x01;
    const int8_t value = vmcu_system_read_gpr(sys, src);

    vmcu_system_write_data(sys, pre_y, value);
    vmcu_system_write_gpr(sys, YL, (0x00ff & pre_y));
    vmcu_system_write_gpr(sys, YH, (0xff00 & pre_y) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_std_yq(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t yl = vmcu_system_read_gpr(sys, YL);
    const uint8_t yh = vmcu_system_read_gpr(sys, YH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, ((yh << 8) + yl) + disp, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_z(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, (zh << 8) + zl, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_zi(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    const uint16_t post_z = ((zh << 8) + zl) + 0x01;

    vmcu_system_write_data(sys, (zh << 8) + zl, value);
    vmcu_system_write_gpr(sys, ZL, (0x00ff & post_z));
    vmcu_system_write_gpr(sys, ZH, (0xff00 & post_z) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_st_dz(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t pre_z = ((zh << 8) + zl) - 0x01;
    const int8_t value = vmcu_system_read_gpr(sys, src);

    vmcu_system_write_data(sys, pre_z, value);
    vmcu_system_write_gpr(sys, ZL, (0x00ff & pre_z));
    vmcu_system_write_gpr(sys, ZH, (0xff00 & pre_z) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_std_zq(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, ((zh << 8) + zl) + disp, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_sts(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 8, 0);
    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);

    const int8_t value = vmcu_system_read_gpr(sys, src + 16);

    int addr = (~(0x10 & dest) << 3) | ((0x10 & dest) << 2) | 
                ((0x40 & dest) >> 1) | ((0x20 & dest) >> 1) |
                ((0x0f & dest));

    vmcu_system_write_data(sys, addr, value);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sts32(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 0, 16, 0);
    const int src = extract(opcode, 20, 25, 0);

    const int8_t value = vmcu_system_read_gpr(sys, src);
    vmcu_system_write_data(sys, (uint16_t) dest, value);

    vmcu_system_move_pc(sys, 2);
    sys->cycles += 2;
}

void execute_lds(vmcu_system_t *sys, const int opcode) {
    
    const int dest = extract(opcode, 4, 8, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);
    
    int addr = (~(0x10 & src) << 3) | ((0x10 & src) << 2) | 
                ((0x40 & src) >> 1) | ((0x20 & src) >> 1) |
                ((0x0f & opcode));

    const int8_t value = vmcu_system_read_data(sys, addr);
    vmcu_system_write_gpr(sys, dest + 16, value);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_lds32(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 20, 25, 0);
    const int src = extract(opcode, 0, 16, 0);

    const int8_t value = vmcu_system_read_data(sys, src);
    vmcu_system_write_gpr(sys, dest, value);

    vmcu_system_move_pc(sys, 2);
    sys->cycles += 2;
}

void execute_xch(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const int8_t reg_val = vmcu_system_read_gpr(sys, src);
    const int8_t data_val = vmcu_system_read_data(sys, (zh << 8) + zl);

    vmcu_system_write_data(sys, (zh << 8) + zl, reg_val);
    vmcu_system_write_gpr(sys, src, data_val);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_brne(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, ZF) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_breq(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, ZF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brge(vmcu_system_t *sys, const int opcode) {

    if((vmcu_system_read_sreg(sys, NF) ^ vmcu_system_read_sreg(sys, VF)) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brpl(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, NF) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brlo(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, CF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brlt(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, SF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brcc(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, CF) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brcs(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, CF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brvs(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, VF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brts(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, TF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brtc(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, TF) == 0x01) {

        vmcu_system_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brmi(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, NF) == 0x00) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brhc(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, HF) == 0x01) {

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brhs(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, HF) == 0x00) {

        vmcu_system_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brid(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, IF) == 0x01) {

        vmcu_system_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brie(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, IF) == 0x00) {

        vmcu_system_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_brvc(vmcu_system_t *sys, const int opcode) {

    if(vmcu_system_read_sreg(sys, VF) == 0x01) {

        vmcu_system_move_pc(sys, 1);    
        sys->cycles += 1;

        return;
    }

    int offs = extract(opcode, 3, 10, 0);
    const int prog_counter = vmcu_system_get_pc(sys);

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);

        vmcu_system_set_pc(sys, prog_counter - offs + 1);
        sys->cycles += 2;

        return;
    }

    vmcu_system_set_pc(sys, prog_counter + offs + 1);
    sys->cycles += 2;
}

void execute_rcall(vmcu_system_t *sys, const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    const int pc = vmcu_system_get_pc(sys);

    switch(PC_BIT) {

        case 16:

            vmcu_system_push_stack(sys, (pc + 1) & 0x00ff);
            vmcu_system_push_stack(sys, ((pc + 1) & 0xff00) >> 8);

            sys->cycles += 3;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    if(((0x01 << 11) & offs) != 0x00) {

        offs = comp(offs, 12);
        vmcu_system_set_pc(sys, pc - offs + 1);
        
        return;
    }

    vmcu_system_set_pc(sys, pc + offs + 1);
}

void execute_ret(vmcu_system_t *sys, const int opcode) {

    uint8_t pcl, pch, pcm;

    switch(PC_BIT) {

        case 16:
            
            pch = vmcu_system_pop_stack(sys);
            pcl = vmcu_system_pop_stack(sys);

            vmcu_system_set_pc(sys, (pch << 8) + pcl);
            sys->cycles += 4;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }
}

void execute_reti(vmcu_system_t *sys, const int opcode) {

    uint8_t pcl, pch, pcm;

    switch(PC_BIT) {

        case 16:
            
            pch = vmcu_system_pop_stack(sys);
            pcl = vmcu_system_pop_stack(sys);

            vmcu_system_set_pc(sys, (pch << 8) + pcl);
            sys->cycles += 4;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    vmcu_system_write_sreg(sys, IF, 0x01);
}

void execute_icall(vmcu_system_t *sys, const int opcode) {

    const int pc = vmcu_system_get_pc(sys);

    switch(PC_BIT) {

        case 16:

            vmcu_system_push_stack(sys, (pc + 1) & 0x00ff);
            vmcu_system_push_stack(sys, ((pc + 1) & 0xff00) >> 8);

            sys->cycles += 3;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    const uint8_t zl = vmcu_system_read_data(sys, ZL);
    const uint8_t zh = vmcu_system_read_data(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    vmcu_system_set_pc(sys, addr);
}

void execute_call(vmcu_system_t *sys, const int opcode) {

    const int addr = (opcode & 0x1ffff) 
                   + (opcode & 0x1f00000);

    const int pc = vmcu_system_get_pc(sys);

    switch(PC_BIT) {

        case 16:

            vmcu_system_push_stack(sys, (pc + 2) & 0x00ff);
            vmcu_system_push_stack(sys, ((pc + 2) & 0xff00) >> 8);

            sys->cycles += 4;

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    vmcu_system_set_pc(sys, addr);
}

void execute_cp(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t value = vmcu_system_read_gpr(sys, dest);
    const int8_t comp = vmcu_system_read_gpr(sys, src);
    const int8_t result = value - comp; 

    int8_t cf_res = (!(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(value, 7));

    int8_t vf_res = (bit(value, 7) * !bit(comp, 7) * !bit(result, 7));
    vf_res += (!bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (!bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * !bit(value, 3));

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_cpi(vmcu_system_t *sys, const int opcode) {

    const int reg = extract(opcode, 4, 8, 0);
    const uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const uint8_t value = vmcu_system_read_gpr(sys, 16 + reg);
    const int8_t result = value - comp;

    int8_t cf_res = (!(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(value, 7));

    int8_t vf_res = (bit(value, 7) * !bit(comp, 7) * !bit(result, 7));
    vf_res += (!bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (!bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * !bit(value, 3));

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_cpc(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t value = vmcu_system_read_gpr(sys, dest);
    const int8_t comp = vmcu_system_read_gpr(sys, src);
    const bool carry = vmcu_system_read_sreg(sys, CF);

    const int8_t result = (value - comp - carry);

    int8_t cf_res = (!(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * !bit(value, 7));

    int8_t vf_res = (bit(value, 7) * !bit(comp, 7) * !bit(result, 7));
    vf_res += (!bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (!bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * !bit(value, 3));

    const int8_t nf_res = bit(result, 7);
    const int8_t zf_res = (result == 0x00) ? vmcu_system_read_sreg(sys, ZF) : 0x00;
    
    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, zf_res);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_lsr(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = vmcu_system_read_gpr(sys, dest);
    const uint8_t result = (value >> 1);

    const int8_t cf_res = bit(value, 0);
    const int8_t vf_res = cf_res ^ 0;

    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, NF, 0x00);
    vmcu_system_write_sreg(sys, SF, vf_res ^ 0);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_asr(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = vmcu_system_read_gpr(sys, dest);
    const uint8_t result = (value >> 1);

    const int8_t cf_res = bit(value, 0);
    const int8_t vf_res = cf_res ^ 0;

    vmcu_system_write_sreg(sys, CF, cf_res);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, NF, 0x00);
    vmcu_system_write_sreg(sys, SF, vf_res ^ 0);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ror(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = vmcu_system_read_gpr(sys, dest);
    const bool carry = vmcu_system_read_sreg(sys, CF);
    
    const bool lsb = (value & 0x01);
    const uint8_t result = ((value >> 1) | (carry << 7));

    const bool nf_res = (result & (0x01 << 7));
    const bool vf_res = (nf_res ^ lsb);

    vmcu_system_write_sreg(sys, CF, lsb);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_swap(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = vmcu_system_read_gpr(sys, dest);
    const uint8_t result = ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ori(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0);
    const uint8_t val = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, 16 + dest);
    const int8_t result = dest_val | val;

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ 0);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, 16 + dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_or(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);

    const int8_t result = dest_val | src_val;
    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ 0);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_and(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const int8_t src_val = vmcu_system_read_gpr(sys, src);
    const int8_t result = dest_val & src_val;

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, (nf_res ^ 0));
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_andi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int value = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    const int8_t dest_val = vmcu_system_read_gpr(sys, dest);
    const uint8_t result = dest_val & value;

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, (nf_res ^ 0));
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_las(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    const uint8_t value = vmcu_system_read_gpr(sys, src);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint8_t data = vmcu_system_read_data(sys, addr);

    const uint8_t result = (value | data);

    vmcu_system_write_gpr(sys, src, data);
    vmcu_system_write_data(sys, addr, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_lac(vmcu_system_t *sys, const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    const uint8_t value = vmcu_system_read_gpr(sys, src);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint8_t data = vmcu_system_read_data(sys, addr);

    const uint8_t result = ((0xff - value) * data);

    vmcu_system_write_gpr(sys, src, data);
    vmcu_system_write_data(sys, addr, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_lat(vmcu_system_t *sys, const int opcode) {
    
    const int src = extract(opcode, 4, 9, 0);
    const uint8_t value = vmcu_system_read_gpr(sys, src);
    
    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint8_t data = vmcu_system_read_data(sys, addr);
    
    const uint8_t result = (value ^ data);
    
    vmcu_system_write_gpr(sys, src, data);
    vmcu_system_write_data(sys, addr, result);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_com(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int8_t value = vmcu_system_read_gpr(sys, dest);
    const int8_t result = (0xff - value);

    const int8_t nf_res = bit(result, 7);

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ 0);
    vmcu_system_write_sreg(sys, ZF, (result == 0x00));

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_neg(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t value = vmcu_system_read_gpr(sys, dest);
    const uint8_t result = ~value + 0x01;

    const int8_t zf_res = (result == 0x00);
    const int8_t vf_res = (result == 0x80);

    const int8_t nf_res = bit(result, 7);
    const int8_t hf_res = bit(result, 3) | bit(value, 3);

    vmcu_system_write_sreg(sys, NF, nf_res);
    vmcu_system_write_sreg(sys, VF, vf_res);
    vmcu_system_write_sreg(sys, HF, hf_res);
    vmcu_system_write_sreg(sys, CF, !zf_res);
    vmcu_system_write_sreg(sys, SF, nf_res ^ vf_res);
    vmcu_system_write_sreg(sys, ZF, zf_res);

    vmcu_system_write_gpr(sys, dest, result);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_bld(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    const bool t_flag = vmcu_system_read_sreg(sys, TF);
    const int8_t val = vmcu_system_read_gpr(sys, dest);

    if(t_flag == 0x00) {

        vmcu_system_write_gpr(sys, dest, (val & ~(0x01 << bpos)));

        vmcu_system_move_pc(sys, 1);
        sys->cycles += 1;

        return;
    }

    vmcu_system_write_gpr(sys, dest, (val | (0x01 << bpos)));

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_bst(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    const int8_t val = vmcu_system_read_gpr(sys, dest);
    vmcu_system_write_sreg(sys, TF, ((0x01 << bpos) & val) >> bpos);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sbi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bpos = extract(opcode, 0, 3, 0);
    
    vmcu_system_set_sfr(sys, (GPR_SIZE + dest), bpos);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_cbi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    vmcu_system_clear_sfr(sys, (GPR_SIZE + dest), bpos);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_lpm(vmcu_system_t *sys, const int opcode) {

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint16_t word = vmcu_system_read_flash(sys, addr >> 1);

    const uint8_t lsb = (addr & 0x01) ? 8 : 0;
    const uint8_t byte = (word >> lsb) & 0xff;

    vmcu_system_write_gpr(sys, 0, byte);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_lpm_z(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint16_t word = vmcu_system_read_flash(sys, addr >> 1);

    const uint8_t lsb = (addr & 0x01) ? 8 : 0;
    const uint8_t byte = (word >> lsb) & 0xff;

    vmcu_system_write_gpr(sys, dest, byte);
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_lpm_zi(vmcu_system_t *sys, const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const uint8_t zl = vmcu_system_read_gpr(sys, ZL);
    const uint8_t zh = vmcu_system_read_gpr(sys, ZH);

    const uint16_t addr = ((zh << 8) + zl);
    const uint16_t word = vmcu_system_read_flash(sys, addr >> 1);

    const uint8_t lsb = (addr & 0x01) ? 8 : 0;
    const uint8_t byte = (word >> lsb) & 0xff;

    const uint16_t post_z = ((zh << 8) + zl) + 0x01;

    vmcu_system_write_gpr(sys, dest, byte);
    vmcu_system_write_gpr(sys, ZL, (0x00ff & post_z));
    vmcu_system_write_gpr(sys, ZH, (0xff00 & post_z) >> 8);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_eicall(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 4;
}

void execute_eijmp(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 2;
}

void execute_elpm(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_elpm_z(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_elpm_zi(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 3;
}

void execute_des(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_sleep(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_wdr(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_break(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_spm(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_spm_zi(vmcu_system_t *sys, const int opcode) {
    
    /* currently not supported */
    
    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_ses(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, SF, 0x01);
    vmcu_system_move_pc(sys, 1);
    
    sys->cycles += 1;
}

void execute_set(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, TF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_sev(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, VF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_sez(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, ZF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_seh(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, HF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_sec(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, CF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_sei(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, IF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_sen(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, NF, 0x01);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_cls(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, SF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_clt(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, TF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_clv(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, VF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_clz(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, ZF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_clh(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, HF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_clc(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, CF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_cli(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, IF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_cln(vmcu_system_t *sys, const int opcode) {

    vmcu_system_write_sreg(sys, NF, 0x00);
    vmcu_system_move_pc(sys, 1);

    sys->cycles += 1;
}

void execute_bclr(vmcu_system_t *sys, const int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);
    vmcu_system_write_sreg(sys, s_bit, 0x00);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void execute_bset(vmcu_system_t *sys, const int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);
    vmcu_system_write_sreg(sys, s_bit, 0x01);

    vmcu_system_move_pc(sys, 1);
    sys->cycles += 1;
}

void (*vmcu_execute[INSTR_MAX]) (vmcu_system_t *sys, const int opcode) = {

    execute_nop,
    execute_movw,
    execute_mul,
    execute_muls,
    execute_mulsu,
    execute_fmul,
    execute_fmuls,
    execute_fmulsu,
    execute_ldi,
    execute_rjmp,
    execute_jmp,
    execute_ijmp,
    execute_mov,
    execute_dec,
    execute_inc,
    execute_add,
    execute_adc,
    execute_adiw,
    execute_sub,
    execute_subi,
    execute_sbc,
    execute_sbci,
    execute_sbiw,
    execute_push,
    execute_pop,
    execute_in,
    execute_out,
    execute_sbis,
    execute_sbic,
    execute_sbrc,
    execute_sbrs,
    execute_cpse,
    execute_eor,
    execute_ld_x,
    execute_ld_xi,
    execute_ld_dx,
    execute_ld_y,
    execute_ld_yi,
    execute_ld_dy,
    execute_ldd_yq,
    execute_ldd_zq,
    execute_ld_z,
    execute_ld_zi,
    execute_ld_dz,
    execute_st_x,
    execute_st_xi,
    execute_st_dx,
    execute_st_y,
    execute_st_yi,
    execute_st_dy,
    execute_std_yq,
    execute_st_z,
    execute_st_zi,
    execute_st_dz,
    execute_std_zq,
    execute_sts,
    execute_sts32,
    execute_lds,
    execute_lds32,
    execute_xch,
    execute_brne,
    execute_breq,
    execute_brge,
    execute_brpl,
    execute_brlo,
    execute_brlt,
    execute_brcc,
    execute_brcs,
    execute_brvs,
    execute_brts,
    execute_brtc,
    execute_brmi,
    execute_brhc,
    execute_brhs,
    execute_brid,
    execute_brie,
    execute_brvc,
    execute_rcall,
    execute_ret,
    execute_reti,
    execute_icall,
    execute_call,
    execute_cp,
    execute_cpi,
    execute_cpc,
    execute_lsr,
    execute_asr,
    execute_ror,
    execute_swap,
    execute_ori,
    execute_or,
    execute_and,
    execute_andi,
    execute_las,
    execute_lac,
    execute_lat,
    execute_com,
    execute_neg,
    execute_bld,
    execute_bst,
    execute_sbi,
    execute_cbi,
    execute_lpm,
    execute_lpm_z,
    execute_lpm_zi,
    execute_eicall,
    execute_eijmp,
    execute_elpm,
    execute_elpm_z,
    execute_elpm_zi,
    execute_des,
    execute_sleep,
    execute_wdr,
    execute_break,
    execute_spm,
    execute_spm_zi,
    execute_ses,
    execute_set,
    execute_sev,
    execute_sez,
    execute_seh,
    execute_sec,
    execute_sei,
    execute_sen,
    execute_cls,
    execute_clt,
    execute_clv,
    execute_clz,
    execute_clh,
    execute_clc,
    execute_cli,
    execute_cln,
    execute_bclr,
    execute_bset
};


