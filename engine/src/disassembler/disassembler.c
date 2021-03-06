/* Implementation of Disassembler */

// C++ Headers
#include <stdlib.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/disassembler/disassembler.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/analyzer/report/plain.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"
#include "engine/include/collections/sstream.h"

#define TAB 26

/* Forward Declaration of static Functions */

static char* disassemble_dw(vmcu_plain_t *p);

/* Forward Declaration of static Members */

static char* (*disassemble_opcode[VMCU_SET_SIZE]) (vmcu_plain_t *p);

/* --- Extern --- */

int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_plain_t *p) {

    if(vmcu_decompose_bytes(bytes, p) < 0)
        return -1;

    if(p->exec == false) {

        p->mnem = disassemble_dw(p);
        return 0;
    }

    p->mnem = (*disassemble_opcode[ p->key ])(p);
    return 0;
}

vmcu_plain_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size) {

    vmcu_plain_t *p;

    if((p = vmcu_decompose_ihex(hex_file, size)) == NULL)
        return NULL;

    for(int i = 0; i < *size; i++) {

        vmcu_plain_t *pi = &(p[i]);

        if(pi->exec == false) {

            pi->mnem = disassemble_dw(pi);
            continue;
        }

        pi->mnem = (*disassemble_opcode[pi->key])(pi);
    }

    return p;
}

/* --- Static --- */

char* disassemble_dw(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, ".dw 0x%04x", p->src.value);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; 0x%x", p->src.value);

    return sstream_alloc(&ss);
}

char* disassemble_nop(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "nop");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; no operation");

    return sstream_alloc(&ss);
}

char* disassemble_movw(vmcu_plain_t *p) {

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

char* disassemble_mul(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mul r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_muls(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "muls r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_mulsu(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mulsu r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_fmul(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmul r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_fmuls(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmuls r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_fmulsu(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "fmulsu r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_ldi(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ldi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0x%02x", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_rjmp(vmcu_plain_t *p) {

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

char* disassemble_jmp(vmcu_plain_t *p) {

    const int addr = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "jmp +%d", addr);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- 0x%x", addr);

    return sstream_alloc(&ss);
}

char* disassemble_ijmp(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ijmp");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL");

    return sstream_alloc(&ss);
}

char* disassemble_mov(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "mov r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_dec(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "dec r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x01", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_inc(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "inc r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + 0x01", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_add(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "add r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_adc(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "adc r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d + R%d + CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_adiw(vmcu_plain_t *p) {

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

char* disassemble_sub(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sub r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_subi(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "subi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sbc(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbc r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - R%d - CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sbci(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbci r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d - 0x%02x - CF", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sbiw(vmcu_plain_t *p) {

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

char* disassemble_push(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "push r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[SP--] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_pop(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "pop r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[++SP]", src);

    return sstream_alloc(&ss);
}

char* disassemble_in(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "in r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- IO[addr]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_out(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "out 0x%02x, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[addr] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_sbis(vmcu_plain_t *p) {

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

char* disassemble_sbic(vmcu_plain_t *p) {

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

char* disassemble_sbrc(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbrc r%d, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d[%d] = 0): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sbrs(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbrs r%d, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d[%d] = 1): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_cpse(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cpse r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; (R%d = R%d): PC <- skip", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_eor(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "eor r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d xor R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_ld_x(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, X", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[X]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_xi(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, X+", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[X+]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_dx(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, -X", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-X]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_y(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, Y", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_yi(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, Y+", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y+]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_dy(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, -Y", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-Y]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ldd_yq(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ldd r%d, Y+%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Y+%d]", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_ldd_zq(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ldd r%d, Z+%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z+%d]", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_ld_z(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, Z", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_zi(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, Z+", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[Z+]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_ld_dz(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ld r%d, -Z", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[-Z]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_st_x(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st X, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[X] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_xi(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st X+, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[X+] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_dx(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st -X, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-X] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_y(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st Y, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_yi(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st Y+, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y+] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_dy(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st -Y, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-Y] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_std_yq(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "std Y+%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Y+%d] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_st_z(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st Z, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_zi(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st Z+, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z+] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_st_dz(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "st -Z, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[-Z] <- R%d", src);

    return sstream_alloc(&ss);
}

char* disassemble_std_zq(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "std Z+%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z+%d] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sts(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sts 0x%x, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sts32(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sts 0x%04x, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_lds(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lds r%d, 0x%x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_lds32(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lds r%d, 0x%04x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_xch(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "xch Z, r%d", src);
    sstream_pad(&ss, (TAB - ss.length));

    sstream_put(&ss, "; DATA[Z] <- R%d", src);
    sstream_put(&ss, ", R%d <- DATA[Z]", src);

    return sstream_alloc(&ss);
}

char* disassemble_brne(vmcu_plain_t *p) {

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

char* disassemble_breq(vmcu_plain_t *p) {

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

char* disassemble_brge(vmcu_plain_t *p) {

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

char* disassemble_brpl(vmcu_plain_t *p) {

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

char* disassemble_brlo(vmcu_plain_t *p) {

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

char* disassemble_brlt(vmcu_plain_t *p) {

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

char* disassemble_brcc(vmcu_plain_t *p) {

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

char* disassemble_brcs(vmcu_plain_t *p) {

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

char* disassemble_brvs(vmcu_plain_t *p) {

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

char* disassemble_brts(vmcu_plain_t *p) {

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

char* disassemble_brtc(vmcu_plain_t *p) {

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

char* disassemble_brmi(vmcu_plain_t *p) {

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

char* disassemble_brhc(vmcu_plain_t *p) {

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

char* disassemble_brhs(vmcu_plain_t *p) {

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

char* disassemble_brid(vmcu_plain_t *p) {

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

char* disassemble_brie(vmcu_plain_t *p) {

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

char* disassemble_brvc(vmcu_plain_t *p) {

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

char* disassemble_rcall(vmcu_plain_t *p) {

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

char* disassemble_ret(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ret");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- DATA[SP]");

    return sstream_alloc(&ss);
}

char* disassemble_reti(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "reti");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- DATA[SP]");

    return sstream_alloc(&ss);
}

char* disassemble_icall(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "icall");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL");

    return sstream_alloc(&ss);
}

char* disassemble_call(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "call +%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- 0x%x", src);

    return sstream_alloc(&ss);
}

char* disassemble_cp(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cp r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - R%d", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_cpi(vmcu_plain_t *p) {

    const uint8_t src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cpi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - 0x%02x", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_cpc(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cpc r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d - R%d - CF", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_lsr(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lsr r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_asr(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "asr r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_ror(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ror r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d >> 1", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_swap(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "swap r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- swap nibbles", src);

    return sstream_alloc(&ss);
}

char* disassemble_ori(vmcu_plain_t *p) {

    const uint8_t src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ori r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d | 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_or_asm(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "or r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d | R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_and_asm(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "and r%d, r%d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d & R%d", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_andi(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "andi r%d, 0x%02x", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- R%d & 0x%02x", dest, dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_las(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "las Z, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);

    return sstream_alloc(&ss);
}

char* disassemble_lac(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lac Z, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- (0xff - R%d) * DATA[Z]", src);

    return sstream_alloc(&ss);
}

char* disassemble_lat(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lat Z, r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);

    return sstream_alloc(&ss);
}

char* disassemble_com(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "com r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0xff - R%d", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_neg(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "neg r%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- 0x00 - R%d", src, src);

    return sstream_alloc(&ss);
}

char* disassemble_bld(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "bld r%d, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d[%d] <- TF", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_bst(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "bst r%d, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- R%d[%d]", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_sbi(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sbi 0x%02x, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[%02x, %d] <- 0x01", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_cbi(vmcu_plain_t *p) {

    const int src = p->src.value;
    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cbi 0x%02x, %d", dest, src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IO[%02x, %d] <- 0x00", dest, src);

    return sstream_alloc(&ss);
}

char* disassemble_lpm(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lpm");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R0 <- FLASH[Z]");

    return sstream_alloc(&ss);
}

char* disassemble_lpm_z(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lpm r%d, Z", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[Z]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_lpm_zi(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "lpm r%d, Z+", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[Z+]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_eicall(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "eicall");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");

    return sstream_alloc(&ss);
}

char* disassemble_eijmp(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "eijmp");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");

    return sstream_alloc(&ss);
}

char* disassemble_elpm(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "elpm");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R0 <- FLASH[RAMPZ:Z]");

    return sstream_alloc(&ss);
}

char* disassemble_elpm_z(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "elpm r%d, Z", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[RAMPZ:Z]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_elpm_zi(vmcu_plain_t *p) {

    const int dest = p->dest.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "elpm r%d, Z+", dest);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; R%d <- FLASH[(RAMPZ:Z)+]", dest);

    return sstream_alloc(&ss);
}

char* disassemble_des(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "des 0x%02x", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; Data Encryption Standard");

    return sstream_alloc(&ss);
}

char* disassemble_sleep(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sleep");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; circuit sleep");

    return sstream_alloc(&ss);
}

char* disassemble_wdr(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "wdr");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; watchdog reset");

    return sstream_alloc(&ss);
}

char* disassemble_break_asm(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "break");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; cpu stop mode");

    return sstream_alloc(&ss);
}

char* disassemble_spm(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "spm");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; FLASH[Z] <- (R1:R0 v 0xffff)");

    return sstream_alloc(&ss);
}

char* disassemble_spm_zi(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "spm Z+");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; FLASH[Z+] <- (R1:R0 v 0xffff)");

    return sstream_alloc(&ss);
}

char* disassemble_ses(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "ses");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; SF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_set(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "set");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_sev(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sev");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; VF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_sez(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sez");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; ZF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_seh(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "seh");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; HF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_sec(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sec");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; CF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_sei(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sei");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_sen(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "sen");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; NF <- 0x01");

    return sstream_alloc(&ss);
}

char* disassemble_cls(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cls");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; SF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_clt(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "clt");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; TF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_clv(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "clv");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; VF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_clz(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "clz");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; ZF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_clh(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "clh");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; HF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_clc(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "clc");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; CF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_cli(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cli");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; IF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_cln(vmcu_plain_t *p) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "cln");

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; NF <- 0x00");

    return sstream_alloc(&ss);
}

char* disassemble_bclr(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "bclr 0x0%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; %s <- 0x00", flags[src]);

    return sstream_alloc(&ss);
}

char* disassemble_bset(vmcu_plain_t *p) {

    const int src = p->src.value;

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "bset 0x0%d", src);

    sstream_pad(&ss, (TAB - ss.length));
    sstream_put(&ss, "; %s <- 0x01", flags[src]);

    return sstream_alloc(&ss);
}

static char* (*disassemble_opcode[VMCU_SET_SIZE]) (vmcu_plain_t *p) = {

    disassemble_nop,
    disassemble_movw,
    disassemble_mul,
    disassemble_muls,
    disassemble_mulsu,
    disassemble_fmul,
    disassemble_fmuls,
    disassemble_fmulsu,
    disassemble_ldi,
    disassemble_rjmp,
    disassemble_jmp,
    disassemble_ijmp,
    disassemble_mov,
    disassemble_dec,
    disassemble_inc,
    disassemble_add,
    disassemble_adc,
    disassemble_adiw,
    disassemble_sub,
    disassemble_subi,
    disassemble_sbc,
    disassemble_sbci,
    disassemble_sbiw,
    disassemble_push,
    disassemble_pop,
    disassemble_in,
    disassemble_out,
    disassemble_sbis,
    disassemble_sbic,
    disassemble_sbrc,
    disassemble_sbrs,
    disassemble_cpse,
    disassemble_eor,
    disassemble_ld_x,
    disassemble_ld_xi,
    disassemble_ld_dx,
    disassemble_ld_y,
    disassemble_ld_yi,
    disassemble_ld_dy,
    disassemble_ldd_yq,
    disassemble_ldd_zq,
    disassemble_ld_z,
    disassemble_ld_zi,
    disassemble_ld_dz,
    disassemble_st_x,
    disassemble_st_xi,
    disassemble_st_dx,
    disassemble_st_y,
    disassemble_st_yi,
    disassemble_st_dy,
    disassemble_std_yq,
    disassemble_st_z,
    disassemble_st_zi,
    disassemble_st_dz,
    disassemble_std_zq,
    disassemble_sts,
    disassemble_sts32,
    disassemble_lds,
    disassemble_lds32,
    disassemble_xch,
    disassemble_brne,
    disassemble_breq,
    disassemble_brge,
    disassemble_brpl,
    disassemble_brlo,
    disassemble_brlt,
    disassemble_brcc,
    disassemble_brcs,
    disassemble_brvs,
    disassemble_brts,
    disassemble_brtc,
    disassemble_brmi,
    disassemble_brhc,
    disassemble_brhs,
    disassemble_brid,
    disassemble_brie,
    disassemble_brvc,
    disassemble_rcall,
    disassemble_ret,
    disassemble_reti,
    disassemble_icall,
    disassemble_call,
    disassemble_cp,
    disassemble_cpi,
    disassemble_cpc,
    disassemble_lsr,
    disassemble_asr,
    disassemble_ror,
    disassemble_swap,
    disassemble_ori,
    disassemble_or_asm,
    disassemble_and_asm,
    disassemble_andi,
    disassemble_las,
    disassemble_lac,
    disassemble_lat,
    disassemble_com,
    disassemble_neg,
    disassemble_bld,
    disassemble_bst,
    disassemble_sbi,
    disassemble_cbi,
    disassemble_lpm,
    disassemble_lpm_z,
    disassemble_lpm_zi,
    disassemble_eicall,
    disassemble_eijmp,
    disassemble_elpm,
    disassemble_elpm_z,
    disassemble_elpm_zi,
    disassemble_des,
    disassemble_sleep,
    disassemble_wdr,
    disassemble_break_asm,
    disassemble_spm,
    disassemble_spm_zi,
    disassemble_ses,
    disassemble_set,
    disassemble_sev,
    disassemble_sez,
    disassemble_seh,
    disassemble_sec,
    disassemble_sei,
    disassemble_sen,
    disassemble_cls,
    disassemble_clt,
    disassemble_clv,
    disassemble_clz,
    disassemble_clh,
    disassemble_clc,
    disassemble_cli,
    disassemble_cln,
    disassemble_bclr,
    disassemble_bset
};