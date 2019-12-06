/* Mnemonic Generator Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Project Headers
#include "mnemonics.hpp"
#include "stringmanip.hpp"
#include "mcu.hpp"

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

    string fill(int len) {

        string space = "";

        for(int i = 0; i < 24 - len; i++)
            space += " ";

        return space;
    }
};

string nop(int opcode) {

    stringstream stream;

    stream << "nop";
    stream << fill(stream.str().size());
    stream << "; no operation";

    return stream.str();
}

string movw(int opcode) {

    int dest = extract(opcode, 4, 8, 0) * 2;
    int src = extract(opcode, 0, 4, 0) * 2;

    stringstream stream;

    stream << "movw r" << dest << ":r" << (dest - 1);
    stream << ", r" << src << ":r" << (src - 1);

    stream << fill(stream.str().size());
    stream << "; R" << dest << ":R" << (dest - 1);
    stream << " <- R" << src << ":R" << (src - 1);

    return stream.str();
}

string muls(int opcode) {

    /* in progress */
    return "currently not supported";
}

string mulsu(int opcode) {

    /* in progress */
    return "currently not supported";
}

string fmul(int opcode) {

    /* in progress */
    return "currently not supported";
}

string ldi(int opcode) {

    int dest = extract(opcode, 4, 8, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    stringstream stream;

    stream << "ldi r" << (dest + 16) << ", 0x" << get_hex(src);
    stream << fill(stream.str().size());
    stream << "; R" << (dest + 16) << " <- 0x" << get_hex(src);

    return stream.str();
}

string rjmp(int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    char sign = '+';

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "rjmp " << sign << offs;
    stream << fill(stream.str().size());
    stream << "; PC <- PC " << sign << " 0x" << hex << offs << " + 1";

    return stream.str();
} 

string mov(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "mov r" << dest << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << src;

    return stream.str();
}

string dec(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "dec r" << dest;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " - 0x01";

    return stream.str();
}

string add(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "add r" << dest << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " + R" << src;

    return stream.str();
}

string push(int opcode) {

    int src = extract(opcode, 4, 9, 0);
    
    stringstream stream;

    stream << "push r" << src;
    stream << fill(stream.str().size());
    stream << "; DATA[SP] <- R" << src;

    return stream.str();
}

string pop(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
        
    stringstream stream;

    stream << "pop r" << dest;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- DATA[--SP]";

    return stream.str();
}

string out(int opcode) {

    int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    int src = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "out 0x" << hex << dest << dec << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; DATA[addr] <- R" << src;

    return stream.str(); 
}

string clr(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "clr r" << dest;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " xor " << "R" << dest;

    return stream.str();
}

string ld_x(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "ld r" << dest << ", X";
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- DATA[X]";

    return stream.str();
}

string ld_y(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "ld r" << dest << ", Y";
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- DATA[Y]";

    return stream.str();
}

string ld_z(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "ld r" << dest << ", Z";
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- DATA[Z]";

    return stream.str();
}

string brne(int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign = '+';

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "brne " << sign << offs;
    stream << fill(stream.str().size());

    stream << "; (Z = 0): PC <- PC " << sign;
    stream << " 0x" << hex << offs << " + 1";

    return stream.str();
}

string breq(int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign = '+';

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "breq " << sign << offs;
    stream << fill(stream.str().size());

    stream << "; (Z = 1): PC <- PC " << sign;
    stream << " 0x" << hex << offs << " + 1";

    return stream.str();
}

string rcall(int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    char sign = '+';

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "rcall " << sign << offs;
    stream << fill(stream.str().size());

    stream << "; PC <- PC " << sign;
    stream << " 0x" << hex << offs << " + 1";

    return stream.str();
}

string ret(int opcode) {

    stringstream stream;

    stream << "ret";
    stream << fill(stream.str().size());
    stream << "; PC <- PC + DATA[SP]";

    return stream.str();
}

string cpi(int opcode) {

    int reg = extract(opcode, 4, 8, 0);
    uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    stringstream stream;

    stream << "cpi r" << (reg + 16) << ", 0x" << get_hex(comp);
    stream << fill(stream.str().size());
    stream << "; R" << (reg + 16) << " - R" << reg;

    return stream.str();
}

string ori(int opcode) {

    int dest = extract(opcode, 4, 8, 0);
    uint8_t const_val = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    stringstream stream;

    stream << "ori r" << (dest + 16) << ", 0x" << get_hex(const_val);
    stream << fill(stream.str().size());
    stream << "; R" << (dest + 16) << " <- " << "R" << (dest + 16);
    stream << " | 0x" << get_hex(const_val);

    return stream.str();
}

string ses(int opcode) {

    stringstream stream;

    stream << "ses";
    stream << fill(stream.str().size());
    stream << "; SF <- 0x01";

    return stream.str();
}

string set(int opcode) {

    stringstream stream;

    stream << "set";
    stream << fill(stream.str().size());
    stream << "; TF <- 0x01";

    return stream.str();
}

string sev(int opcode) {

    stringstream stream;

    stream << "sev";
    stream << fill(stream.str().size());
    stream << "; VF <- 0x01";

    return stream.str();
}

string sez(int opcode) {

    stringstream stream;

    stream << "sez";
    stream << fill(stream.str().size());
    stream << "; ZF <- 0x01";

    return stream.str();
}

string seh(int opcode) {

    stringstream stream;

    stream << "seh";
    stream << fill(stream.str().size());
    stream << "; HF <- 0x01";

    return stream.str();
}

string sec(int opcode) {

    stringstream stream;

    stream << "sec";
    stream << fill(stream.str().size());
    stream << "; CF <- 0x01";

    return stream.str();
}

string sei(int opcode) {

    stringstream stream;

    stream << "sei";
    stream << fill(stream.str().size());
    stream << "; IF <- 0x01";

    return stream.str();
}

string sen(int opcode) {

    stringstream stream;

    stream << "sen";
    stream << fill(stream.str().size());
    stream << "; NF <- 0x01";

    return stream.str();
}

string bclr(int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);

    stringstream stream;

    stream << "bclr 0x0" << s_bit;
    stream << fill(stream.str().size());
    stream << "; " << flags[s_bit] << " <- 0x00";

    return stream.str();
}

string (*mnemonics[INSTR_MAX]) (int opcode) = { nop, movw, muls, mulsu, fmul, ldi, rjmp, mov, 
                                                dec, add, push, pop, out, clr, ld_x, ld_y, ld_z, brne, 
                                                breq, rcall, ret, cpi, ori, ses, set, sev, sez, seh, sec,
                                                sei, sen, bclr };
