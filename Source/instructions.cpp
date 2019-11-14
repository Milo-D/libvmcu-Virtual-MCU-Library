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

    /* in progress */
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

    /* in progress */
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

void (*instructions[INSTR_MAX]) (Sys *sys, int opcode) = { nop, movw, muls, mulsu, fmul, ldi, rjmp, mov, 
                                                           dec, push, pop, out };




