/* Assembly Instruction Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "instructions/instructions.hpp"
#include "system/mcu.hpp"
#include "system/sys.hpp"

#define bit(value, i) (((0x01 << i) & value) >> i)

using namespace std;

namespace {

    int extract(int opcode, int from, int to, int offs) {

        int res = 0;

        for(int i = from; i < to; i++) {

            int bit = (((0x01 << i) & opcode) >> i);
            res += (bit << (i - from) + offs);
        }

        return res;
    }
};

void nop(Sys *sys, int opcode) {

    return;
}

void movw(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0) * 2;
    int src = extract(opcode, 0, 4, 0) * 2;

    sys->write_gpr(dest, sys->read_gpr(src));
    sys->write_gpr(dest + 1, sys->read_gpr(src + 1));
}

void muls(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0) + 16;
    int src = extract(opcode, 0, 4, 0) + 16;

    int16_t result = sys->read_gpr(dest) * sys->read_gpr(src);

    sys->write_sreg(CF, bit(result, 15));
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(0, (result & 0x00ff));
    sys->write_gpr(1, (result & 0xff00) >> 8);
}

void mulsu(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 7, 0) + 16;
    int src = extract(opcode, 0, 3, 0) + 16;

    int16_t result = sys->read_gpr(dest) * (uint8_t) sys->read_gpr(src);

    sys->write_sreg(CF, bit(result, 15));
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(0, (result & 0x00ff));
    sys->write_gpr(1, (result & 0xff00) >> 8);
}

void fmul(Sys *sys, int opcode) {

    /* in progress */
}

void ldi(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sys->write_gpr(dest + 16, src);
}

void rjmp(Sys *sys, int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
} 

void mov(Sys *sys, int opcode) {

    int	dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sys->write_gpr(dest, sys->read_gpr(src));
}

void dec(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int8_t value = sys->read_gpr(dest);

    int8_t result = value - 0x01;

    int8_t vf_res = ~bit(result, 7) * bit(result, 6) * bit(result, 5) * bit(result, 4);
    vf_res *= bit(result, 3) * bit(result, 2) * bit(result, 1) * bit(result, 0);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void inc(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int8_t value = sys->read_gpr(dest);

    int8_t result = value + 1;

    int8_t vf_res = bit(result, 7) * ~bit(result, 6) * ~bit(result, 5) * ~bit(result, 4);
    vf_res *= ~bit(result, 3) * ~bit(result, 2) * ~bit(result, 1) * ~bit(result, 0);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void add(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t dest_val = sys->read_gpr(dest);
    int8_t src_val = sys->read_gpr(src);
    int8_t result = dest_val + src_val;

    int8_t vf_res = bit(dest_val, 7) * bit(src_val, 7) * ~bit(result, 7);
    vf_res += ~bit(dest_val, 7) * ~bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * ~bit(result, 7));
    cf_res += ~bit(result, 7) * bit(dest_val, 7);

    int8_t hf_res = (bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * ~bit(result, 3));
    cf_res += ~bit(result, 3) * bit(dest_val, 3);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(CF, cf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void adc(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t dest_val = sys->read_gpr(dest);
    int8_t src_val = sys->read_gpr(src);

    bool carry = sys->read_sreg(CF);
    int8_t result = (dest_val + src_val + carry);

    int8_t vf_res = bit(dest_val, 7) * bit(src_val, 7) * ~bit(result, 7);
    vf_res += ~bit(dest_val, 7) * ~bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * ~bit(result, 7));
    cf_res += ~bit(result, 7) * bit(dest_val, 7);

    int8_t hf_res = (bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * ~bit(result, 3));
    cf_res += ~bit(result, 3) * bit(dest_val, 3);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(CF, cf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void sub(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t dest_val = sys->read_gpr(dest);
    int8_t src_val = sys->read_gpr(src);
    int8_t result = dest_val - src_val;

    int8_t vf_res = bit(dest_val, 7) * ~bit(src_val, 7) * ~bit(result, 7);
    vf_res += ~bit(dest_val, 7) * bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (~bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * ~bit(dest_val, 7));

    int8_t hf_res = (~bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * bit(result, 3));
    hf_res += bit(result, 3) * ~bit(dest_val, 3);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(CF, cf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void sbc(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t dest_val = sys->read_gpr(dest);
    int8_t src_val = sys->read_gpr(src);

    bool carry = sys->read_sreg(CF);
    int8_t result = (dest_val - src_val - carry);

    int8_t vf_res = bit(dest_val, 7) * ~bit(src_val, 7) * ~bit(result, 7);
    vf_res += ~bit(dest_val, 7) * bit(src_val, 7) * bit(result, 7);

    int8_t cf_res = (~bit(dest_val, 7) * bit(src_val, 7)) + (bit(src_val, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * ~bit(dest_val, 7));

    int8_t hf_res = (~bit(dest_val, 3) * bit(src_val, 3)) + (bit(src_val, 3) * bit(result, 3));
    hf_res += bit(result, 3) * ~bit(dest_val, 3);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(CF, cf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00) * sys->read_sreg(ZF));

    sys->write_gpr(dest, result);
}

void push(Sys *sys, int opcode) {

    int src = extract(opcode, 4, 9, 0);
    int8_t val = sys->read_gpr(src);

    sys->push_stack(val);
}

void pop(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int8_t val = sys->pop_stack();

    sys->write_gpr(dest, val);
}

void out(Sys *sys, int opcode) {

    int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    int src = extract(opcode, 4, 9, 0);

    int8_t val = sys->read_gpr(src);

    sys->write_data(dest, val);
}

void clr(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t val_dest = sys->read_gpr(dest);
    int8_t val_src = sys->read_gpr(src);

    sys->write_gpr(dest, (val_dest ^ val_src));

    sys->write_sreg(SF, 0x00);
    sys->write_sreg(VF, 0x00);
    sys->write_sreg(NF, 0x00);
    sys->write_sreg(ZF, 0x01);
}

void ld_x(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    uint8_t xl = sys->read_gpr(XL);
    uint8_t xh = sys->read_gpr(XH);

    int8_t data = sys->read_data((xh << 8) + xl);
    sys->write_gpr(dest, data);
}

void ld_xi(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    uint8_t xl = sys->read_gpr(XL);
    uint8_t xh = sys->read_gpr(XH);

    int8_t data = sys->read_data((xh << 8) + xl);
    int16_t post_x = ((xh << 8) + xl) + 0x01;

    sys->write_gpr(dest, data);
    sys->write_gpr(XL, (0x00ff & post_x));
    sys->write_gpr(XH, (0xff00 & post_x) >> 8);
}

void ld_dx(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    uint8_t xl = sys->read_gpr(XL);
    uint8_t xh = sys->read_gpr(XH);

    int16_t pre_x = ((xh << 8) + xl) - 0x01;
    int8_t data = sys->read_data(pre_x);

    sys->write_gpr(XL, (0x00ff & pre_x));
    sys->write_gpr(XH, (0xff00 & pre_x) >> 8);
    sys->write_gpr(dest, data);
}

void ld_y(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    uint8_t yl = sys->read_gpr(YL);
    uint8_t yh = sys->read_gpr(YH);

    int8_t data = sys->read_data((yh << 8) + yl);
    sys->write_gpr(dest, data);
}

void ld_z(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    uint8_t zl = sys->read_gpr(ZL);
    uint8_t zh = sys->read_gpr(ZH);

    int8_t data = sys->read_data((zh << 8) + zl);
    sys->write_gpr(dest, data);
}

void st_x(Sys *sys, int opcode) {

    int src = extract(opcode, 4, 9, 0);

    uint8_t xl = sys->read_gpr(XL);
    uint8_t xh = sys->read_gpr(XH);
    int8_t value = sys->read_gpr(src);

    sys->write_data((xh << 8) + xl, value);
}

void st_xi(Sys *sys, int opcode) {

    int src = extract(opcode, 4, 9, 0);

    uint8_t xl = sys->read_gpr(XL);
    uint8_t xh = sys->read_gpr(XH);

    int8_t value = sys->read_gpr(src);
    int16_t post_x = ((xh << 8) + xl) + 0x01;

    sys->write_data((xh << 8) + xl, value);
    sys->write_gpr(XL, (0x00ff & post_x));
    sys->write_gpr(XH, (0xff00 & post_x) >> 8);
}

void brne(Sys *sys, int opcode) {

    if(sys->read_sreg(ZF) == 0x01)
        return;

    int offs = extract(opcode, 3, 10, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
}

void breq(Sys *sys, int opcode) {

    if(sys->read_sreg(ZF) == 0x00)
        return;

    int offs = extract(opcode, 3, 10, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
}

void brge(Sys *sys, int opcode) {

    if(sys->read_sreg(NF) ^ sys->read_sreg(VF) == 0x01)
        return;

    int offs = extract(opcode, 3, 10, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
}

void brpl(Sys *sys, int opcode) {

    if(sys->read_sreg(NF) == 0x01)
        return;

    int offs = extract(opcode, 3, 10, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
}

void brlo(Sys *sys, int opcode) {

    if(sys->read_sreg(CF) == 0x00)
        return;

    int offs = extract(opcode, 3, 10, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
}

void rcall(Sys *sys, int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    int pc = sys->get_pc();

    switch(PC_BIT) {

        case 16:

            sys->push_stack((pc + 1) & 0x00ff);
            sys->push_stack(((pc + 1) & 0xff00) >> 8);

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sys->set_pc(pc - offs + 1);

        return;
    }

    sys->set_pc(pc + offs + 1);
}

void ret(Sys *sys, int opcode) {

    uint8_t pcl, pch, pcm;

    switch(PC_BIT) {

        case 16:
            
            pch = sys->pop_stack();
            pcl = sys->pop_stack();

            sys->set_pc((pch << 8) + pcl);

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }
}

void cp(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t value = sys->read_gpr(dest);
    int8_t comp = sys->read_gpr(src);
    int8_t result = value - comp; 

    int8_t cf_res = (~(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * ~bit(value, 7));

    int8_t vf_res = (bit(value, 7) * ~bit(comp, 7) * ~bit(result, 7));
    vf_res += (~bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (~bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * ~bit(value, 3));

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(CF, cf_res);
    sys->write_sreg(VF, vf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));
}

void cpi(Sys *sys, int opcode) {

    int reg = extract(opcode, 4, 8, 0);
    uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    uint8_t value = sys->read_gpr(16 + reg);
    int8_t result = value - comp;

    int8_t cf_res = (~(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * ~bit(value, 7));

    int8_t vf_res = (bit(value, 7) * ~bit(comp, 7) * ~bit(result, 7));
    vf_res += (~bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (~bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * ~bit(value, 3));

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(CF, cf_res);
    sys->write_sreg(VF, vf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));
}

void lsr(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t value = sys->read_gpr(dest);
    int8_t result = (value >> 1);

    int8_t cf_res = bit(value, 0);
    int8_t vf_res = cf_res ^ 0;

    sys->write_sreg(CF, cf_res);
    sys->write_sreg(VF, vf_res);
    sys->write_sreg(NF, 0x00);
    sys->write_sreg(SF, vf_res ^ 0);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void ori(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0);
    uint8_t const_val = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    int8_t dest_val = sys->read_gpr(16 + dest);
    int8_t result = dest_val | const_val;

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, 0x00);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ 0);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(16 + dest, result);
}

void or_asm(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t dest_val = sys->read_gpr(dest);
    int8_t src_val = sys->read_gpr(src);

    sys->write_gpr(dest, dest_val | src_val);
}

void and_asm(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t dest_val = sys->read_gpr(dest);
    int8_t src_val = sys->read_gpr(src);
    int8_t result = dest_val & src_val;

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, 0x00);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, (nf_res ^ 0));
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void andi(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0) + 16;
    int value = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    int8_t dest_val = sys->read_gpr(dest);
    uint8_t result = dest_val & value;

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, 0x00);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, (nf_res ^ 0));
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void com(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t value = sys->read_gpr(dest);
    int8_t result = (0xff - value);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, 0x00);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ 0);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void bld(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int bpos = extract(opcode, 0, 3, 0);

    bool t_flag = sys->read_sreg(TF);
    int8_t val = sys->read_gpr(dest);

    if(t_flag == 0x00) {

        sys->write_gpr(dest, (val &= ~(0x01 << bpos)));
        return;
    }

    sys->write_gpr(dest, (val |= (0x01 << bpos)));
}

void bst(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int bpos = extract(opcode, 0, 3, 0);

    int8_t val = sys->read_gpr(dest);
    sys->write_sreg(TF, ((0x01 << bpos) & val) >> bpos);
}

void ses(Sys *sys, int opcode) {

    sys->write_sreg(SF, 0x01);
}

void set(Sys *sys, int opcode) {

    sys->write_sreg(TF, 0x01);
}

void sev(Sys *sys, int opcode) {

    sys->write_sreg(VF, 0x01);
}

void sez(Sys *sys, int opcode) {

    sys->write_sreg(ZF, 0x01);
}

void seh(Sys *sys, int opcode) {

    sys->write_sreg(HF, 0x01);
}

void sec(Sys *sys, int opcode) {

    sys->write_sreg(CF, 0x01);
}

void sei(Sys *sys, int opcode) {

    sys->write_sreg(IF, 0x01);
}

void sen(Sys *sys, int opcode) {

    sys->write_sreg(NF, 0x01);
}

void cls(Sys *sys, int opcode) {

    sys->write_sreg(SF, 0x00);
}

void clt(Sys *sys, int opcode) {

    sys->write_sreg(TF, 0x00);
}

void clv(Sys *sys, int opcode) {

    sys->write_sreg(VF, 0x00);
}

void clz(Sys *sys, int opcode) {

    sys->write_sreg(ZF, 0x00);
}

void clh(Sys *sys, int opcode) {

    sys->write_sreg(HF, 0x00);
}

void clc(Sys *sys, int opcode) {

    sys->write_sreg(CF, 0x00);
}

void cli(Sys *sys, int opcode) {

    sys->write_sreg(IF, 0x00);
}

void cln(Sys *sys, int opcode) {

    sys->write_sreg(NF, 0x00);
}

void bclr(Sys *sys, int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);

    sys->write_sreg(s_bit, 0x00);
}

void (*instructions[INSTR_MAX]) (Sys *sys, int opcode) = { nop, movw, muls, mulsu, fmul, ldi, rjmp, mov, 
                                                           dec, inc, add, adc, sub, sbc, push, pop, out, clr, ld_x, ld_xi, ld_dx, ld_y, ld_z, 
                                                           st_x, st_xi, brne, breq, brge, brpl, brlo, rcall, ret, cp, cpi, lsr, ori, or_asm, and_asm, 
                                                           andi, com, bld, bst, ses, set, sev, sez, seh, sec, sei, sen, cls, clt, clv, clz, clh, 
                                                           clc, cli, cln, bclr };




