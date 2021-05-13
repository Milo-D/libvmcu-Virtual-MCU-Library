/* Implementation of Disassembler */

// C++ Headers
#include <stdlib.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/disassembler/disassembler.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/analyzer/report/instr.h"

// Project Headers (engine utilities)
#include "engine/include/collections/sstream.h"

#define TAB 26

/* Forward Declaration of static Functions */

static char* disassemble_dw(vmcu_instr_t *instr);

/* Forward Declaration of static Members */

static char* (*disassemble_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr);

/* --- Extern --- */

int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu) {

    if(vmcu_decompose_bytes(bytes, instr, mcu) < 0)
        return -1;

    if(instr->exec == false) {

        instr->mnem = disassemble_dw(instr);
        return 0;
    }

    instr->mnem = (*disassemble_opcode[ instr->key ])(instr);
    return 0;
}

vmcu_instr_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *instr_list;

    if((instr_list = vmcu_decompose_ihex(hex_file, size, mcu)) == NULL)
        return NULL;

    for(int32_t i = 0; i < *size; i++) {

        vmcu_instr_t *instr = &(instr_list[i]);

        if(instr->exec == false) {

            instr->mnem = disassemble_dw(instr);
            continue;
        }

        instr->mnem = (*disassemble_opcode[ instr->key ])(instr);
    }

    return instr_list;
}

/* --- Static --- */

static char* disassemble_dw(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, ".dw 0x%04x", instr->src.value);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; 0x%x", instr->src.value);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_nop(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "nop");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; no operation");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_movw(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "movw r%d:r%d", dest + 1, dest);
    vmcu_sstream_put(&ss, ", r%d:r%d", src + 1, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d:R%d", dest + 1, dest);
    vmcu_sstream_put(&ss, " <- R%d:R%d", src + 1, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_mul(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "mul r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_muls(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "muls r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_mulsu(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "mulsu r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_fmul(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "fmul r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_fmuls(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "fmuls r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_fmulsu(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "fmulsu r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R1:R0 <- R%d * R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ldi(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ldi r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- 0x%02x", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_rjmp(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "rjmp %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_jmp(vmcu_instr_t *instr) {

    const int addr = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "jmp +%d", addr);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- 0x%x", addr);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ijmp(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ijmp");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- ZH:ZL");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_mov(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "mov r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_dec(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "dec r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d - 0x01", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_inc(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "inc r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d + 0x01", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_add(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "add r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d + R%d", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_adc(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "adc r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d + R%d + CF", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_adiw(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "adiw r%d:r%d, ", dest + 1, dest);
    vmcu_sstream_put(&ss, "0x%02x", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));

    vmcu_sstream_put(&ss, "; R%d:R%d <- R%d:R%d", dest + 1, dest, dest + 1, dest);
    vmcu_sstream_put(&ss, " + 0x%02x", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sub(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sub r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d - R%d", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_subi(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "subi r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d - 0x%02x", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbc(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbc r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d - R%d - CF", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbci(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbci r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d - 0x%02x - CF", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbiw(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbiw r%d:r%d, ", dest + 1, dest);
    vmcu_sstream_put(&ss, "0x%02x", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));

    vmcu_sstream_put(&ss, "; R%d:R%d <- R%d:R%d", dest + 1, dest, dest + 1, dest);
    vmcu_sstream_put(&ss, " - 0x%02x", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_push(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "push r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[SP--] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_pop(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "pop r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[++SP]", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_in(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "in r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- IO[addr]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_out(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "out 0x%02x, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; IO[addr] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbis(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbis 0x%02x, %d", dest, src);
    vmcu_sstream_pad(&ss, (TAB - ss.length));

    vmcu_sstream_put(&ss, "; (IO[%02x, %d]", dest, src);
    vmcu_sstream_put(&ss, " = 1): PC <- skip");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbic(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbic 0x%02x, %d", dest, src);
    vmcu_sstream_pad(&ss, (TAB - ss.length));

    vmcu_sstream_put(&ss, "; (IO[%02x, %d]", dest, src);
    vmcu_sstream_put(&ss, " = 0): PC <- skip");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbrc(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbrc r%d, %d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (R%d[%d] = 0): PC <- skip", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbrs(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbrs r%d, %d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (R%d[%d] = 1): PC <- skip", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cpse(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cpse r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (R%d = R%d): PC <- skip", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_eor(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "eor r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d xor R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_x(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, X", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[X]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_xi(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, X+", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[X+]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_dx(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, -X", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[-X]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_y(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, Y", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[Y]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_yi(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, Y+", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[Y+]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_dy(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, -Y", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[-Y]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ldd_yq(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ldd r%d, Y+%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[Y+%d]", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ldd_zq(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ldd r%d, Z+%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[Z+%d]", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_z(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, Z", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[Z]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_zi(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, Z+", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[Z+]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ld_dz(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ld r%d, -Z", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[-Z]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_x(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st X, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[X] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_xi(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st X+, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[X+] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_dx(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st -X, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[-X] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_y(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st Y, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Y] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_yi(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st Y+, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Y+] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_dy(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st -Y, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[-Y] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_std_yq(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "std Y+%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Y+%d] <- R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_z(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st Z, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Z] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_zi(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st Z+, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Z+] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_st_dz(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "st -Z, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[-Z] <- R%d", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_std_zq(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "std Z+%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Z+%d] <- R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sts(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sts 0x%x, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sts32(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sts 0x%04x, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[0x%x] <- R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lds(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lds r%d, 0x%x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lds32(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lds r%d, 0x%04x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- DATA[0x%x]", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_xch(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "xch Z, r%d", src);
    vmcu_sstream_pad(&ss, (TAB - ss.length));

    vmcu_sstream_put(&ss, "; DATA[Z] <- R%d", src);
    vmcu_sstream_put(&ss, ", R%d <- DATA[Z]", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brne(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brne %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (Z = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_breq(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "breq %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (Z = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brge(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brge %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (N ^ V = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brpl(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brpl %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (N = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brlo(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brlo %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (C = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brlt(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brlt %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (N ^ V = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brcc(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brcc %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (C = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brvs(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brvs %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (V = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brts(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brts %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (T = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brtc(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brtc %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (T = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brmi(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brmi %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (N = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brhc(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brhc %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (H = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brhs(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brhs %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (H = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brid(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brid %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (I = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brie(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brie %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (I = 1): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_brvc(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "brvc %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; (V = 0): PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_rcall(vmcu_instr_t *instr) {

    int src = instr->src.value;
    char sign[2] = "+";

    if(src < 0x00) {

        src *= -1;
        sign[0] = '-';
    }

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "rcall %s%d", sign, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- PC %s 0x%x + 1", sign, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ret(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ret");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- DATA[SP]");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_reti(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "reti");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- DATA[SP]");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_icall(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "icall");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- ZH:ZL");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_call(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "call +%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- 0x%x", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cp(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cp r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d - R%d", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cpi(vmcu_instr_t *instr) {

    const uint8_t src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cpi r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d - 0x%02x", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cpc(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cpc r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d - R%d - CF", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lsr(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lsr r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d >> 1", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_asr(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "asr r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d >> 1", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ror(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ror r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d >> 1", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_swap(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "swap r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- swap nibbles", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ori(vmcu_instr_t *instr) {

    const uint8_t src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ori r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d | 0x%02x", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_or_asm(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "or r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d | R%d", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_and_asm(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "and r%d, r%d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d & R%d", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_andi(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "andi r%d, 0x%02x", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- R%d & 0x%02x", dest, dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_las(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "las Z, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lac(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lac Z, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Z] <- (0xff - R%d) * DATA[Z]", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lat(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lat Z, r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; DATA[Z] <- R%d | DATA[Z]", src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_com(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "com r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- 0xff - R%d", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_neg(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "neg r%d", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- 0x00 - R%d", src, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_bld(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "bld r%d, %d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d[%d] <- TF", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_bst(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "bst r%d, %d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; TF <- R%d[%d]", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sbi(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sbi 0x%02x, %d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; IO[%02x, %d] <- 0x01", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cbi(vmcu_instr_t *instr) {

    const int src = instr->src.value;
    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cbi 0x%02x, %d", dest, src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; IO[%02x, %d] <- 0x00", dest, src);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lpm(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lpm");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R0 <- FLASH[Z]");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lpm_z(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lpm r%d, Z", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- FLASH[Z]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_lpm_zi(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "lpm r%d, Z+", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- FLASH[Z+]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_eicall(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "eicall");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_eijmp(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "eijmp");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; PC <- ZH:ZL + (EIND << 16)");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_elpm(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "elpm");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R0 <- FLASH[RAMPZ:Z]");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_elpm_z(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "elpm r%d, Z", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- FLASH[RAMPZ:Z]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_elpm_zi(vmcu_instr_t *instr) {

    const int dest = instr->dest.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "elpm r%d, Z+", dest);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; R%d <- FLASH[(RAMPZ:Z)+]", dest);

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_des(vmcu_instr_t *instr) {

    const int src = instr->src.value;

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "des 0x%02x", src);

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; Data Encryption Standard");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sleep(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sleep");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; circuit sleep");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_wdr(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "wdr");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; watchdog reset");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_break_asm(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "break");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; cpu stop mode");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_spm(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "spm");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; FLASH[Z] <- (R1:R0 v 0xffff)");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_spm_zi(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "spm Z+");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; FLASH[Z+] <- (R1:R0 v 0xffff)");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_ses(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "ses");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; SF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_set(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "set");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; TF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sev(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sev");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; VF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sez(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sez");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; ZF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_seh(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "seh");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; HF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sec(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sec");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; CF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sei(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sei");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; IF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_sen(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "sen");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; NF <- 0x01");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cls(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cls");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; SF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_clt(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "clt");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; TF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_clv(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "clv");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; VF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_clz(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "clz");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; ZF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_clh(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "clh");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; HF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_clc(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "clc");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; CF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cli(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cli");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; IF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* disassemble_cln(vmcu_instr_t *instr) {

    vmcu_sstream_t ss;
    vmcu_sstream_ctor(&ss);

    vmcu_sstream_put(&ss, "cln");

    vmcu_sstream_pad(&ss, (TAB - ss.length));
    vmcu_sstream_put(&ss, "; NF <- 0x00");

    return vmcu_sstream_alloc(&ss);
}

static char* (*disassemble_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr) = {

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
    disassemble_cln
};