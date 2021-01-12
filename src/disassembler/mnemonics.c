/* Mnemonic Generator Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// Project Headers
#include "disassembler/mnemonics.h"
#include "system/mcudef.h"
#include "misc/bitmanip.h"
#include "collections/sstream.h"

#define TAB 26

/* --- Extern --- */

char* mnem_dw(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, ".dw 0x%04x", opcode);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; 0x%x", opcode);

    return sstream_alloc(&ss);
}

/* --- Static --- */

char* mnem_nop(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "nop");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; no operation");

    return sstream_alloc(&ss);
}

char* mnem_movw(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) * 2;
    const int src = extract(opcode, 0, 4, 0) * 2;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "movw r%d:r%d", dest + 1, dest);
    sstream_put(&ss, ", r%d:r%d", src + 1, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d:R%d", dest + 1, dest);
    sstream_put(&ss, " <- R%d:R%d", src + 1, src);

    return sstream_alloc(&ss);
}

char* mnem_mul(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mul r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_muls(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + 16;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "muls r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_mulsu(const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mulsu r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_fmul(const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmul r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_fmuls(const int opcode) {

    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmuls r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_fmulsu(const int opcode) {
    
    const int dest = extract(opcode, 4, 7, 0) + 16;
    const int src = extract(opcode, 0, 3, 0) + 16;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmulsu r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_ldi(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ldi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0x%02x", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_rjmp(const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    char sign[2] = "+";

    if(((0x01 << 11) & offs) != 0x00) {

        offs = comp(offs, 12);
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "rjmp %s%d", sign, offs);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_jmp(const int opcode) {

    const int addr = (opcode & 0x1ffff) 
                   + (opcode & 0x1f00000);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "jmp +%d", addr);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- 0x%x", addr);

    return sstream_alloc(&ss);
}

char* mnem_ijmp(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ijmp");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL");

    return sstream_alloc(&ss);
} 

char* mnem_mov(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "mov r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_dec(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "dec r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x01", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_inc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "inc r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + 0x01", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_add(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "add r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_adc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "adc r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + R%d + CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_adiw(const int opcode) {

    const int dest = (2 * extract(opcode, 4, 6, 0)) + 24;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 6, 8, 4);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "adiw r%d:r%d, ", dest + 1, dest);
    sstream_put(&ss, "0x%02x", src);

    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; R%d:R%d <- R%d:R%d", dest + 1, dest, dest + 1, dest);
    sstream_put(&ss, " + 0x%02x", src);

    return sstream_alloc(&ss);
}

char* mnem_sub(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sub r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_subi(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "subi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbc r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - R%d - CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbci(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbci r%d, 0x%02x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x%02x - CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbiw(const int opcode) {

    const int dest = (2 * extract(opcode, 4, 6, 0)) + 24;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 6, 8, 4);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbiw r%d:r%d, ", dest + 1, dest);
    sstream_put(&ss, "0x%02x", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; R%d:R%d <- R%d:R%d", dest + 1, dest, dest + 1, dest);
    sstream_put(&ss, " - 0x%02x", src);

    return sstream_alloc(&ss);
}

char* mnem_push(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "push r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[SP--] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_pop(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "pop r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[++SP]", dest);

    return sstream_alloc(&ss);
}

char* mnem_in(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "in r%d, 0x%02x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- IO[addr]", dest);

    return sstream_alloc(&ss);
}

char* mnem_out(const int opcode) {

    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    const int src = extract(opcode, 4, 9, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "out 0x%02x, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[addr] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_sbis(const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bit = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbis 0x%02x, %d", dest, bit);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; (IO[%02x, %d]", dest, bit);
    sstream_put(&ss, " = 1): PC <- skip");

    return sstream_alloc(&ss);
}

char* mnem_sbic(const int opcode) {
    
    const int dest = extract(opcode, 3, 8, 0);
    const int bit = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbic 0x%02x, %d", dest, bit);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; (IO[%02x, %d]", dest, bit);
    sstream_put(&ss, " = 0): PC <- skip");

    return sstream_alloc(&ss);
}

char* mnem_sbrc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bit = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbrc r%d, %d", dest, bit);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d[%d] = 0): PC <- skip", dest, bit);

    return sstream_alloc(&ss);
}

char* mnem_sbrs(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bit = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbrs r%d, %d", dest, bit);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d[%d] = 1): PC <- skip", dest, bit);

    return sstream_alloc(&ss);
}

char* mnem_cpse(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cpse r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d = R%d): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_eor(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "eor r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d xor R%d", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_x(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, X", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[X]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_xi(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, X+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[X+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_dx(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, -X", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-X]", dest);

    return sstream_alloc(&ss);
}

char* mnem_ld_y(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, Y", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y]", dest);

    return sstream_alloc(&ss);
}

char* mnem_ld_yi(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, Y+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y+]", dest);

    return sstream_alloc(&ss);
}

char* mnem_ld_dy(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, -Y", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-Y]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ldd_yq(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) 
                   + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ldd r%d, Y+%d", dest, disp);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y+%d]", dest, disp);

    return sstream_alloc(&ss);
}

char* mnem_ldd_zq(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) 
                   + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ldd r%d, Z+%d", dest, disp);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z+%d]", dest, disp);

    return sstream_alloc(&ss);
}

char* mnem_ld_z(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_zi(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, Z+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_dz(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, -Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_st_x(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st X, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[X] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_xi(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st X+, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[X+] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_dx(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st -X, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-X] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_y(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Y, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_yi(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Y+, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y+] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_dy(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st -Y, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-Y] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_std_yq(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) 
                   + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "std Y+%d, r%d", disp, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y+%d] <- R%d", disp, src);

    return sstream_alloc(&ss);
}

char* mnem_st_z(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_st_zi(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Z+, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z+] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_dz(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st -Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-Z] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_std_zq(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);

    const int disp = extract(opcode, 0, 3, 0) 
                   + extract(opcode, 10, 12, 3)
                   + extract(opcode, 13, 14, 5);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "std Z+%d, r%d", disp, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z+%d] <- R%d", disp, src);
    
    return sstream_alloc(&ss);
}

char* mnem_sts(const int opcode) {

    const int src = extract(opcode, 4, 8, 0) + 16;
    const int dest = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);

    int addr = (~(0x10 & dest) << 3) | ((0x10 & dest) << 2) | 
                ((0x40 & dest) >> 1) | ((0x20 & dest) >> 1) |
                ((0x0f & opcode));

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sts 0x%x, r%d", addr, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[0x%x] <- R%d", addr, src);

    return sstream_alloc(&ss);
}

char* mnem_sts32(const int opcode) {

    const int dest = extract(opcode, 0, 16, 0);
    const int src = extract(opcode, 20, 25, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sts 0x%04x, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_lds(const int opcode) {
    
    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 11, 4);
    
    int addr = (~(0x10 & src) << 3) | ((0x10 & src) << 2) | 
                ((0x40 & src) >> 1) | ((0x20 & src) >> 1) |
                ((0x0f & opcode));
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lds r%d, 0x%x", dest, addr);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, addr);

    return sstream_alloc(&ss);
}

char* mnem_lds32(const int opcode) {

    const int dest = extract(opcode, 20, 25, 0);
    const int src = extract(opcode, 0, 16, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lds r%d, 0x%04x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_xch(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "xch Z, r%d", src);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; DATA[Z] <- R%d", src);
    sstream_put(&ss, ", R%d <- DATA[Z]", src);

    return sstream_alloc(&ss);
}

char* mnem_brne(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brne %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (Z = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_breq(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "breq %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (Z = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brge(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brge %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N ^ V = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brpl(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brpl %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brlo(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brlo %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (C = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brlt(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brlt %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N ^ V = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brcc(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brcc %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (C = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brcs(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brcs %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (C = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brvs(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brvs %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (V = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brts(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brts %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (T = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brtc(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brtc %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (T = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brmi(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brmi %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brhc(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brhc %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (H = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brhs(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brhs %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (H = 1): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brid(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brid %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (I = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_brie(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brie %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (I = 1): PC <- PC %s 0x%x + 1", sign, offs);
    
    return sstream_alloc(&ss);
}

char* mnem_brvc(const int opcode) {

    int offs = extract(opcode, 3, 10, 0);
    char sign[2] = "+";

    if(((0x01 << 6) & offs) != 0x00) {

        offs = comp(offs, 7);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brvc %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (V = 0): PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_rcall(const int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    char sign[2] = "+";

    if(((0x01 << 11) & offs) != 0x00) {

        offs = comp(offs, 12);
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "rcall %s%d", sign, offs);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- PC %s 0x%x + 1", sign, offs);

    return sstream_alloc(&ss);
}

char* mnem_ret(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ret");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- DATA[SP]"); 

    return sstream_alloc(&ss);
}

char* mnem_reti(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "reti");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- DATA[SP]"); 

    return sstream_alloc(&ss);
}

char* mnem_icall(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "icall");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL"); 

    return sstream_alloc(&ss);
}

char* mnem_call(const int opcode) {

    const int addr = (opcode & 0x1ffff) 
                   + (opcode & 0x1f00000);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "call +%d", addr);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- 0x%x", addr);

    return sstream_alloc(&ss);
}

char* mnem_cp(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cp r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_cpi(const int opcode) {

    const int reg = extract(opcode, 4, 8, 0) + 16;
    const uint8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cpi r%d, 0x%02x", reg, comp);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - 0x%02x", reg, comp);

    return sstream_alloc(&ss);
}

char* mnem_cpc(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cpc r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - R%d - CF", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_lsr(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lsr r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_asr(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "asr r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ror(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ror r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_swap(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "swap r%d", dest);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; R%d[7:4] <- R%d[3:0]", dest, dest);
    sstream_put(&ss, ", R%d[3:0] <- R%d[7:4]", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ori(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const uint8_t val = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ori r%d, 0x%02x", dest, val);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d | 0x%02x", dest, dest, val);

    return sstream_alloc(&ss);
}

char* mnem_or_asm(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "or r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d | R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_and_asm(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "and r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d & R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_andi(const int opcode) {

    const int dest = extract(opcode, 4, 8, 0) + 16;
    const int value = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "andi r%d, 0x%02x", dest, value);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d & 0x%02x", dest, dest, value);

    return sstream_alloc(&ss);
}

char* mnem_las(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "las Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);
    
    return sstream_alloc(&ss);
}

char* mnem_lac(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lac Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- (0xff - R%d) * DATA[Z]", src);
    
    return sstream_alloc(&ss);
}

char* mnem_lat(const int opcode) {

    const int src = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lat Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);
    
    return sstream_alloc(&ss);
}

char* mnem_com(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "com r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0xff - R%d", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_neg(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "neg r%d", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0x00 - R%d", dest, dest);
    
    return sstream_alloc(&ss);
}

char* mnem_bld(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bld r%d, %d", dest, bpos);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d[%d] <- TF", dest, bpos);

    return sstream_alloc(&ss);
}

char* mnem_bst(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    const int bpos = extract(opcode, 0, 3, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bst r%d, %d", dest, bpos);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- R%d[%d]", dest, bpos);

    return sstream_alloc(&ss);
}

char* mnem_sbi(const int opcode) {

    const int dest = extract(opcode, 3, 8, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbi 0x%02x, %d", dest, bpos);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[%02x, %d] <- 0x01", dest, bpos);
    
    return sstream_alloc(&ss);
}

char* mnem_cbi(const int opcode) {
    
    const int dest = extract(opcode, 3, 8, 0);
    const int bpos = extract(opcode, 0, 3, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cbi 0x%02x, %d", dest, bpos);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[%02x, %d] <- 0x00", dest, bpos);
    
    return sstream_alloc(&ss);
}

char* mnem_lpm(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lpm");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R0 <- FLASH[Z]");
    
    return sstream_alloc(&ss);
}

char* mnem_lpm_z(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lpm r%d, Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_lpm_zi(const int opcode) {

    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lpm r%d, Z+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[Z+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_eicall(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "eicall");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");
    
    return sstream_alloc(&ss);
}

char* mnem_eijmp(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "eijmp");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");
    
    return sstream_alloc(&ss);
}

char* mnem_elpm(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "elpm");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R0 <- FLASH[RAMPZ:Z]");
    
    return sstream_alloc(&ss);
}

char* mnem_elpm_z(const int opcode) {
    
    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "elpm r%d, Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[RAMPZ:Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_elpm_zi(const int opcode) {
    
    const int dest = extract(opcode, 4, 9, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "elpm r%d, Z+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[(RAMPZ:Z)+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_des(const int opcode) {
    
    const int iter = extract(opcode, 4, 8, 0);
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "des 0x%02x", iter);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; Data Encryption Standard");
    
    return sstream_alloc(&ss);
}

char* mnem_sleep(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sleep");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; circuit sleep");
    
    return sstream_alloc(&ss);
}

char* mnem_wdr(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "wdr");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; watchdog reset");
    
    return sstream_alloc(&ss);
}

char* mnem_break_asm(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "break");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; cpu stop mode");
    
    return sstream_alloc(&ss);
}

char* mnem_spm(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "spm");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; FLASH[Z] <- (R1:R0 v 0xffff)");
    
    return sstream_alloc(&ss);
}

char* mnem_spm_zi(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "spm Z+");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; FLASH[Z+] <- (R1:R0 v 0xffff)");
    
    return sstream_alloc(&ss);
}

char* mnem_ses(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ses");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; SF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_set(const int opcode) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "set");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sev(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sev");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; VF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sez(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sez");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; ZF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_seh(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "seh");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; HF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sec(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sec");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; CF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sei(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sei");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sen(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sen");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; NF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_cls(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cls");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; SF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clt(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clt");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clv(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clv");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; VF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clz(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clz");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; ZF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clh(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clh");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; HF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clc(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clc");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; CF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_cli(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cli");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_cln(const int opcode) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cln");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; NF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_bclr(const int opcode) {

    const int s_bit = extract(opcode, 4, 7, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bclr 0x0%d", s_bit);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; %s <- 0x00", flags[s_bit]);
    
    return sstream_alloc(&ss);
}

char* mnem_bset(const int opcode) {

    const int s_bit = extract(opcode, 4, 7, 0);

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bset 0x0%d", s_bit);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; %s <- 0x01", flags[s_bit]);
    
    return sstream_alloc(&ss);
}

char* (*mnemonics[INSTR_MAX]) (const int opcode) = { 

    mnem_nop, 
    mnem_movw, 
    mnem_mul,
    mnem_muls, 
    mnem_mulsu, 
    mnem_fmul, 
    mnem_fmuls,
    mnem_fmulsu,
    mnem_ldi, 
    mnem_rjmp, 
    mnem_jmp, 
    mnem_ijmp, 
    mnem_mov, 
    mnem_dec, 
    mnem_inc, 
    mnem_add, 
    mnem_adc, 
    mnem_adiw, 
    mnem_sub, 
    mnem_subi, 
    mnem_sbc, 
    mnem_sbci, 
    mnem_sbiw, 
    mnem_push, 
    mnem_pop, 
    mnem_in, 
    mnem_out, 
    mnem_sbis, 
    mnem_sbic,
    mnem_sbrc, 
    mnem_sbrs, 
    mnem_cpse, 
    mnem_eor, 
    mnem_ld_x, 
    mnem_ld_xi, 
    mnem_ld_dx, 
    mnem_ld_y,
    mnem_ld_yi,
    mnem_ld_dy, 
    mnem_ldd_yq, 
    mnem_ldd_zq,
    mnem_ld_z,
    mnem_ld_zi, 
    mnem_ld_dz,
    mnem_st_x, 
    mnem_st_xi, 
    mnem_st_dx, 
    mnem_st_y,
    mnem_st_yi,
    mnem_st_dy, 
    mnem_std_yq,
    mnem_st_z, 
    mnem_st_zi,
    mnem_st_dz,
    mnem_std_zq, 
    mnem_sts, 
    mnem_sts32, 
    mnem_lds,
    mnem_lds32, 
    mnem_xch, 
    mnem_brne, 
    mnem_breq, 
    mnem_brge, 
    mnem_brpl, 
    mnem_brlo, 
    mnem_brlt, 
    mnem_brcc, 
    mnem_brcs, 
    mnem_brvs, 
    mnem_brts, 
    mnem_brtc, 
    mnem_brmi, 
    mnem_brhc,
    mnem_brhs,
    mnem_brid,
    mnem_brie,
    mnem_brvc,
    mnem_rcall, 
    mnem_ret, 
    mnem_reti, 
    mnem_icall, 
    mnem_call, 
    mnem_cp, 
    mnem_cpi, 
    mnem_cpc, 
    mnem_lsr, 
    mnem_asr, 
    mnem_ror, 
    mnem_swap, 
    mnem_ori, 
    mnem_or_asm, 
    mnem_and_asm, 
    mnem_andi, 
    mnem_las, 
    mnem_lac, 
    mnem_lat,
    mnem_com, 
    mnem_neg, 
    mnem_bld, 
    mnem_bst, 
    mnem_sbi,
    mnem_cbi,
    mnem_lpm, 
    mnem_lpm_z, 
    mnem_lpm_zi, 
    mnem_eicall,
    mnem_eijmp,
    mnem_elpm,
    mnem_elpm_z,
    mnem_elpm_zi,
    mnem_des,
    mnem_sleep,
    mnem_wdr,
    mnem_break_asm,
    mnem_spm,
    mnem_spm_zi,
    mnem_ses, 
    mnem_set, 
    mnem_sev, 
    mnem_sez, 
    mnem_seh, 
    mnem_sec, 
    mnem_sei, 
    mnem_sen, 
    mnem_cls, 
    mnem_clt, 
    mnem_clv, 
    mnem_clz, 
    mnem_clh, 
    mnem_clc, 
    mnem_cli, 
    mnem_cln, 
    mnem_bclr, 
    mnem_bset
};

