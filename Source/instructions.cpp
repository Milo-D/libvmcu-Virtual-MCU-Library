/* Assembly Instruction Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "instructions.hpp"
#include "mcu.hpp"
#include "sys.hpp"

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

    /* in progress */
}

void mulsu(Sys *sys, int opcode) {

    /* in progress */
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

    /* in progress */
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

    int8_t xl = sys->read_gpr(XL);
    int8_t xh = sys->read_gpr(XH);

    int8_t data = sys->read_data((xh << 8) + xl);
    sys->write_gpr(dest, data);
}

void ld_y(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t yl = sys->read_gpr(YL);
    int8_t yh = sys->read_gpr(YH);

    int8_t data = sys->read_data((yh << 8) + yl);
    sys->write_gpr(dest, data);
}

void ld_z(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t zl = sys->read_gpr(ZL);
    int8_t zh = sys->read_gpr(ZH);

    int8_t data = sys->read_data((zh << 8) + zl);
    sys->write_gpr(dest, data);
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

void (*instructions[INSTR_MAX]) (Sys *sys, int opcode) = { nop, movw, muls, mulsu, fmul, ldi, rjmp, mov, 
                                                           dec, push, pop, out, clr, ld_x, ld_y, ld_z, ses,
                                                           set, sev };




