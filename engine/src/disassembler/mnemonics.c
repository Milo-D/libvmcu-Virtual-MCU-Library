/* Mnemonic Generator Implementation */

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/disassembler/mnemonics.h"
#include "engine/include/analyzer/report/plain.h"
#include "engine/include/system/mcudef.h"

// Project Headers (shared)
#include "shared/include/collections/sstream.h"

#define TAB 26

/* --- Extern --- */

char* mnem_dw(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, ".dw 0x%04x", p->src.value);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; 0x%x", p->src.value);

    return sstream_alloc(&ss);
}

/* --- Static --- */

char* mnem_nop(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "nop");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; no operation");

    return sstream_alloc(&ss);
}

char* mnem_movw(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "movw r%d:r%d", dest + 1, dest);
    sstream_put(&ss, ", r%d:r%d", src + 1, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d:R%d", dest + 1, dest);
    sstream_put(&ss, " <- R%d:R%d", src + 1, src);

    return sstream_alloc(&ss);
}

char* mnem_mul(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mul r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_muls(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "muls r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_mulsu(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mulsu r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_fmul(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmul r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_fmuls(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmuls r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_fmulsu(plain_t *p) {
    
    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmulsu r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_ldi(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ldi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0x%02x", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_rjmp(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "rjmp %s%d", sign, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_jmp(plain_t *p) {

    const int addr = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "jmp +%d", addr);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- 0x%x", addr);

    return sstream_alloc(&ss);
}

char* mnem_ijmp(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ijmp");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL");

    return sstream_alloc(&ss);
} 

char* mnem_mov(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "mov r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_dec(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "dec r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x01", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_inc(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "inc r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + 0x01", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_add(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "add r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_adc(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "adc r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + R%d + CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_adiw(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "adiw r%d:r%d, ", dest + 1, dest);
    sstream_put(&ss, "0x%02x", src);

    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; R%d:R%d <- R%d:R%d", dest + 1, dest, dest + 1, dest);
    sstream_put(&ss, " + 0x%02x", src);

    return sstream_alloc(&ss);
}

char* mnem_sub(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sub r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_subi(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "subi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbc(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbc r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - R%d - CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbci(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbci r%d, 0x%02x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x%02x - CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbiw(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbiw r%d:r%d, ", dest + 1, dest);
    sstream_put(&ss, "0x%02x", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; R%d:R%d <- R%d:R%d", dest + 1, dest, dest + 1, dest);
    sstream_put(&ss, " - 0x%02x", src);

    return sstream_alloc(&ss);
}

char* mnem_push(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "push r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[SP--] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_pop(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "pop r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[++SP]", src);

    return sstream_alloc(&ss);
}

char* mnem_in(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "in r%d, 0x%02x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- IO[addr]", dest);

    return sstream_alloc(&ss);
}

char* mnem_out(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "out 0x%02x, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[addr] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_sbis(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbis 0x%02x, %d", dest, src);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; (IO[%02x, %d]", dest, src);
    sstream_put(&ss, " = 1): PC <- skip");

    return sstream_alloc(&ss);
}

char* mnem_sbic(plain_t *p) {
    
    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbic 0x%02x, %d", dest, src);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; (IO[%02x, %d]", dest, src);
    sstream_put(&ss, " = 0): PC <- skip");

    return sstream_alloc(&ss);
}

char* mnem_sbrc(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbrc r%d, %d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d[%d] = 0): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbrs(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbrs r%d, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d[%d] = 1): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_cpse(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cpse r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d = R%d): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_eor(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "eor r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d xor R%d", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_x(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, X", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[X]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_xi(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, X+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[X+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_dx(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, -X", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-X]", dest);

    return sstream_alloc(&ss);
}

char* mnem_ld_y(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, Y", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y]", dest);

    return sstream_alloc(&ss);
}

char* mnem_ld_yi(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, Y+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y+]", dest);

    return sstream_alloc(&ss);
}

char* mnem_ld_dy(plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, -Y", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-Y]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ldd_yq(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ldd r%d, Y+%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y+%d]", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_ldd_zq(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ldd r%d, Z+%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z+%d]", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_ld_z(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_zi(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, Z+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_ld_dz(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ld r%d, -Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_st_x(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st X, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[X] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_xi(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st X+, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[X+] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_dx(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st -X, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-X] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_y(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Y, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_yi(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Y+, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y+] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_dy(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st -Y, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-Y] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_std_yq(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "std Y+%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y+%d] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_st_z(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d", src);

    return sstream_alloc(&ss);
}

char* mnem_st_zi(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st Z+, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z+] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_st_dz(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "st -Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-Z] <- R%d", src);
    
    return sstream_alloc(&ss);
}

char* mnem_std_zq(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "std Z+%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z+%d] <- R%d", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_sts(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sts 0x%x, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sts32(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sts 0x%04x, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_lds(plain_t *p) {
    
    const int src = p->src.value;
    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lds r%d, 0x%x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_lds32(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lds r%d, 0x%04x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_xch(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "xch Z, r%d", src);
    sstream_pad(&ss, (TAB - ss.length));
    
    sstream_put(&ss, "; DATA[Z] <- R%d", src);
    sstream_put(&ss, ", R%d <- DATA[Z]", src);

    return sstream_alloc(&ss);
}

char* mnem_brne(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brne %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (Z = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_breq(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "breq %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (Z = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brge(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brge %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N ^ V = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brpl(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brpl %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brlo(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brlo %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (C = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brlt(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brlt %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N ^ V = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brcc(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brcc %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (C = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brcs(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brcs %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (C = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brvs(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brvs %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (V = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brts(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brts %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (T = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brtc(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brtc %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (T = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brmi(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brmi %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (N = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brhc(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brhc %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (H = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brhs(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brhs %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (H = 1): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brid(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brid %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (I = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_brie(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brie %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (I = 1): PC <- PC %s 0x%x + 1", sign, src);
    
    return sstream_alloc(&ss);
}

char* mnem_brvc(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "brvc %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (V = 0): PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_rcall(plain_t *p) {

    int src = p->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "rcall %s%d", sign, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- PC %s 0x%x + 1", sign, src);

    return sstream_alloc(&ss);
}

char* mnem_ret(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ret");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- DATA[SP]"); 

    return sstream_alloc(&ss);
}

char* mnem_reti(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "reti");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- DATA[SP]"); 

    return sstream_alloc(&ss);
}

char* mnem_icall(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "icall");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL"); 

    return sstream_alloc(&ss);
}

char* mnem_call(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "call +%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- 0x%x", src);

    return sstream_alloc(&ss);
}

char* mnem_cp(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cp r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - R%d", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_cpi(plain_t *p) {

    const uint8_t src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cpi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - 0x%02x", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_cpc(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cpc r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - R%d - CF", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_lsr(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lsr r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_asr(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "asr r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_ror(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ror r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_swap(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "swap r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- swap nibbles", src);
    
    return sstream_alloc(&ss);
}

char* mnem_ori(plain_t *p) {

    const uint8_t src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ori r%d, 0x%02x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d | 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_or_asm(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "or r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d | R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_and_asm(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "and r%d, r%d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d & R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_andi(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "andi r%d, 0x%02x", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d & 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* mnem_las(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "las Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);
    
    return sstream_alloc(&ss);
}

char* mnem_lac(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lac Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- (0xff - R%d) * DATA[Z]", src);
    
    return sstream_alloc(&ss);
}

char* mnem_lat(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lat Z, r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);
    
    return sstream_alloc(&ss);
}

char* mnem_com(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "com r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0xff - R%d", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_neg(plain_t *p) {

    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "neg r%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0x00 - R%d", src, src);
    
    return sstream_alloc(&ss);
}

char* mnem_bld(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bld r%d, %d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d[%d] <- TF", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_bst(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bst r%d, %d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- R%d[%d]", dest, src);

    return sstream_alloc(&ss);
}

char* mnem_sbi(plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sbi 0x%02x, %d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[%02x, %d] <- 0x01", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_cbi(plain_t *p) {
    
    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cbi 0x%02x, %d", dest, src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[%02x, %d] <- 0x00", dest, src);
    
    return sstream_alloc(&ss);
}

char* mnem_lpm(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lpm");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R0 <- FLASH[Z]");
    
    return sstream_alloc(&ss);
}

char* mnem_lpm_z(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lpm r%d, Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_lpm_zi(plain_t *p) {

    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "lpm r%d, Z+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[Z+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_eicall(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "eicall");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");
    
    return sstream_alloc(&ss);
}

char* mnem_eijmp(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "eijmp");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");
    
    return sstream_alloc(&ss);
}

char* mnem_elpm(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "elpm");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R0 <- FLASH[RAMPZ:Z]");
    
    return sstream_alloc(&ss);
}

char* mnem_elpm_z(plain_t *p) {
    
    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "elpm r%d, Z", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[RAMPZ:Z]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_elpm_zi(plain_t *p) {
    
    const int dest = p->dest.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "elpm r%d, Z+", dest);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[(RAMPZ:Z)+]", dest);
    
    return sstream_alloc(&ss);
}

char* mnem_des(plain_t *p) {
    
    const int src = p->src.value;
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "des 0x%02x", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; Data Encryption Standard");
    
    return sstream_alloc(&ss);
}

char* mnem_sleep(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sleep");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; circuit sleep");
    
    return sstream_alloc(&ss);
}

char* mnem_wdr(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "wdr");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; watchdog reset");
    
    return sstream_alloc(&ss);
}

char* mnem_break_asm(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "break");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; cpu stop mode");
    
    return sstream_alloc(&ss);
}

char* mnem_spm(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "spm");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; FLASH[Z] <- (R1:R0 v 0xffff)");
    
    return sstream_alloc(&ss);
}

char* mnem_spm_zi(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "spm Z+");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; FLASH[Z+] <- (R1:R0 v 0xffff)");
    
    return sstream_alloc(&ss);
}

char* mnem_ses(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "ses");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; SF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_set(plain_t *p) {
    
    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "set");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sev(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sev");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; VF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sez(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sez");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; ZF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_seh(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "seh");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; HF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sec(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sec");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; CF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sei(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sei");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_sen(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "sen");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; NF <- 0x01");
    
    return sstream_alloc(&ss);
}

char* mnem_cls(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cls");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; SF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clt(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clt");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clv(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clv");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; VF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clz(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clz");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; ZF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clh(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clh");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; HF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_clc(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "clc");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; CF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_cli(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cli");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_cln(plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "cln");
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; NF <- 0x00");
    
    return sstream_alloc(&ss);
}

char* mnem_bclr(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bclr 0x0%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; %s <- 0x00", flags[src]);
    
    return sstream_alloc(&ss);
}

char* mnem_bset(plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);
    
    sstream_put(&ss, "bset 0x0%d", src);
    
    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; %s <- 0x01", flags[src]);
    
    return sstream_alloc(&ss);
}

char* (*mnemonics[INSTR_MAX]) (plain_t *p) = { 

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

