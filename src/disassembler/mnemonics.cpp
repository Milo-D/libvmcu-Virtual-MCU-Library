/* Mnemonic Generator Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Project Headers
#include "disassembler/mnemonics.hpp"
#include "system/mcu.hpp"
#include "misc/stringmanip.hpp"

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

    stream << "movw r" << (dest + 1) << ":r" << dest;
    stream << ", r" << (src + 1) << ":r" << src;

    stream << fill(stream.str().size());
    stream << "; R" << (dest + 1) << ":R" << dest;
    stream << " <- R" << (src + 1) << ":R" << src;

    return stream.str();
}

string muls(int opcode) {

    int dest = extract(opcode, 4, 8, 0);
    int src = extract(opcode, 0, 4, 0);

    stringstream stream;

    stream << "muls r" << (dest + 16) << ", r" << (src + 16);
    stream << fill(stream.str().size());
    stream << "; R1:R0 <- R" << (dest + 16) << " * R" << (src + 16);

    return stream.str();
}

string mulsu(int opcode) {

    int dest = extract(opcode, 4, 7, 0);
    int src = extract(opcode, 0, 3, 0);

    stringstream stream;

    stream << "mulsu r" << (dest + 16) << ", r" << (src + 16);
    stream << fill(stream.str().size());
    stream << "; R1:R0 <- " << (dest + 16) << " * R" << (src + 16);

    return stream.str();
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

string inc(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "inc r" << dest;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " + 0x01";

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

string adc(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "adc r" << dest << ", r" << src;
    stream << fill(stream.str().size());

    stream << "; R" << dest << " <- R" << dest << " + R" << src;
    stream << " + CF";

    return stream.str();
}

string sub(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "sub r" << dest << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " - R" << src;

    return stream.str();
}

string sbc(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "sbc r" << dest << ", r" << src;
    stream << fill(stream.str().size());

    stream << "; R" << dest << " <- R" << dest << " - R" << src;
    stream << " - CF";

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

string ld_xi(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "ld r" << dest << ", X+";
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- DATA[X+]";

    return stream.str();
}

string ld_dx(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "ld r" << dest << ", -X";
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- DATA[-X]";

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

string st_x(int opcode) {

    int src = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "st X" << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; DATA[X] <- R" << src;

    return stream.str();
}

string st_xi(int opcode) {

    int src = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "st X+" << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; DATA[X+] <- R" << src;

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

string brge(int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign = '+';

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "brge " << sign << offs;
    stream << fill(stream.str().size());

    stream << "; (N ^ V = 0): PC <- PC " << sign;
    stream << " 0x" << hex << offs << " + 1";

    return stream.str();
}

string brpl(int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign = '+';

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "brpl " << sign << offs;
    stream << fill(stream.str().size());

    stream << "; (N = 0): PC <- PC " << sign;
    stream << " 0x" << hex << offs << " + 1";

    return stream.str();
}

string brlo(int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign = '+';

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sign = '-';
    }

    stringstream stream;

    stream << "brlo " << sign << offs;
    stream << fill(stream.str().size());

    stream << "; (C = 1): PC <- PC " << sign;
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

string cp(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "cp r" << dest << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " - R" << src;

    return stream.str();

}

string cpi(int opcode) {

    int reg = extract(opcode, 4, 8, 0);
    uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    stringstream stream;

    stream << "cpi r" << (reg + 16) << ", 0x" << get_hex(comp);
    stream << fill(stream.str().size());
    stream << "; R" << (reg + 16) << " - 0x" << get_hex(comp);

    return stream.str();
}

string lsr(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "lsr r" << dest;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " >> 1";

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

string or_asm(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "or r" << dest << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " | R" << src;

    return stream.str();
}

string and_asm(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    stringstream stream;

    stream << "and r" << dest << ", r" << src;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- R" << dest << " & R" << src;

    return stream.str();  
}

string andi(int opcode) {

    int dest = extract(opcode, 4, 8, 0) + 16;
    int value = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    stringstream stream;

    stream << "andi r" << dest << ", 0x" << hex << value << dec;
    stream << fill(stream.str().size());

    stream << "; R" << dest << " <- R" << dest << " & 0x";
    stream << hex << value << dec;

    return stream.str();  
}

string com(int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    stringstream stream;

    stream << "com r" << dest;
    stream << fill(stream.str().size());
    stream << "; R" << dest << " <- 0xff - R" << dest;

    return stream.str();
}

string bld(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int bpos = extract(opcode, 0, 3, 0);

    stringstream stream;

    stream << "bld r" << dest << ", " << bpos;
    stream << fill(stream.str().size());
    stream << "; R" << dest << "[" << bpos << "]" << " <- TF";

    return stream.str();
}

string bst(int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int bpos = extract(opcode, 0, 3, 0);

    stringstream stream;

    stream << "bst r" << dest << ", " << bpos;
    stream << fill(stream.str().size());
    stream << "; TF <- R" << dest << "[" << bpos << "]";

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

string cls(int opcode) {

    stringstream stream;

    stream << "cls";
    stream << fill(stream.str().size());
    stream << "; SF <- 0x00";

    return stream.str();
}

string clt(int opcode) {

    stringstream stream;

    stream << "clt";
    stream << fill(stream.str().size());
    stream << "; TF <- 0x00";

    return stream.str();
}

string clv(int opcode) {

    stringstream stream;

    stream << "clv";
    stream << fill(stream.str().size());
    stream << "; VF <- 0x00";

    return stream.str();
}

string clz(int opcode) {

    stringstream stream;

    stream << "clz";
    stream << fill(stream.str().size());
    stream << "; ZF <- 0x00";

    return stream.str();
}

string clh(int opcode) {

    stringstream stream;

    stream << "clh";
    stream << fill(stream.str().size());
    stream << "; HF <- 0x00";

    return stream.str();
}

string clc(int opcode) {

    stringstream stream;

    stream << "clc";
    stream << fill(stream.str().size());
    stream << "; CF <- 0x00";

    return stream.str();
}

string cli(int opcode) {

    stringstream stream;

    stream << "cli";
    stream << fill(stream.str().size());
    stream << "; IF <- 0x00";

    return stream.str(); 
}

string cln(int opcode) {

    stringstream stream;

    stream << "cln";
    stream << fill(stream.str().size());
    stream << "; NF <- 0x00";

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
                                                dec, inc, add, adc, sub, sbc, push, pop, out, clr, ld_x, ld_xi, ld_dx, ld_y, ld_z, 
                                                st_x, st_xi, brne, breq, brge, brpl, brlo, rcall, ret, cp, cpi, lsr, ori, or_asm, and_asm, 
                                                andi, com, bld, bst, ses, set, sev, sez, seh, sec, sei, sen, cls, clt, clv, clz, clh, 
                                                clc, cli, cln, bclr };

