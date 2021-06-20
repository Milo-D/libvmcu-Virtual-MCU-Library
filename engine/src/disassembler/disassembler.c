/* Implementation of Disassembler */

// C++ Headers
#include <stdio.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/disassembler/disassembler.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/analyzer/report/instr.h"

/* Forward Declaration of static Functions */

static void disassemble_dw(vmcu_instr_t *instr);

/* Forward Declaration of static Members */

static void (*disassemble_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr);

/* --- Extern --- */

int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu) {

    if(vmcu_decompose_bytes(bytes, instr, mcu) < 0)
        return -1;

    if(instr->exec == false) {

        disassemble_dw(instr);
        return 0;
    }

    (*disassemble_opcode[ instr->key ])(instr);
    return 0;
}

vmcu_instr_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu) {

    vmcu_instr_t *instr_list;

    if((instr_list = vmcu_decompose_ihex(hex_file, size, mcu)) == NULL)
        return NULL;

    for(int32_t i = 0; i < *size; i++) {

        vmcu_instr_t *instr = &(instr_list[i]);

        if(instr->exec == false) {

            disassemble_dw(instr);
            continue;
        }

        (*disassemble_opcode[ instr->key ])(instr);
    }

    return instr_list;
}

/* --- Static --- */

static void disassemble_dw(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    ".dw");

    sprintf(instr->mnem.src,     "0x%04x", instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s",   instr->mnem.src);
}

static void disassemble_nop(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "nop");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; no operation");
}

static void disassemble_movw(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "movw");

    sprintf(instr->mnem.src,     "r%d:r%d",    instr->src.value + 1,
                                               instr->src.value + 0);
    sprintf(instr->mnem.dest,    "r%d:r%d",    instr->dest.value + 1,
                                               instr->dest.value + 0);

    sprintf(instr->mnem.comment, "; %s <- %s", instr->mnem.dest,
                                               instr->mnem.src);
}

static void disassemble_mul(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "mul");

    sprintf(instr->mnem.src,     "r%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                instr->dest.value);

    sprintf(instr->mnem.comment, "; r1:r0 <- %s * %s", instr->mnem.dest,
                                                       instr->mnem.src);
}

static void disassemble_muls(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "muls");

    sprintf(instr->mnem.src,     "r%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                instr->dest.value);

    sprintf(instr->mnem.comment, "; r1:r0 <- %s * %s", instr->mnem.dest,
                                                       instr->mnem.src);
}

static void disassemble_mulsu(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "mulsu");

    sprintf(instr->mnem.src,     "r%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                instr->dest.value);

    sprintf(instr->mnem.comment, "; r1:r0 <- %s * %s", instr->mnem.dest,
                                                       instr->mnem.src);
}

static void disassemble_fmul(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "fmul");

    sprintf(instr->mnem.src,     "r%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                instr->dest.value);

    sprintf(instr->mnem.comment, "; r1:r0 <- %s * %s", instr->mnem.dest,
                                                       instr->mnem.src);
}

static void disassemble_fmuls(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "fmuls");

    sprintf(instr->mnem.src,     "r%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                instr->dest.value);

    sprintf(instr->mnem.comment, "; r1:r0 <- %s * %s", instr->mnem.dest,
                                                       instr->mnem.src);
}

static void disassemble_fmulsu(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "fmulsu");

    sprintf(instr->mnem.src,     "r%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                instr->dest.value);

    sprintf(instr->mnem.comment, "; r1:r0 <- %s * %s", instr->mnem.dest,
                                                       instr->mnem.src);
}

static void disassemble_ldi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ldi");

    sprintf(instr->mnem.src,     "0x%02x",     instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",        instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s", instr->mnem.dest,
                                               instr->mnem.src);
}

static void disassemble_rjmp(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "rjmp");

    sprintf(instr->mnem.src,     "%d",                  instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_jmp(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "jmp");

    sprintf(instr->mnem.src,     "0x%x",       instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- %s", instr->mnem.src);
}

static void disassemble_ijmp(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ijmp");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- ZH:ZL");
}

static void disassemble_mov(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "mov");

    sprintf(instr->mnem.src,     "r%d",        instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",        instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s", instr->mnem.dest,
                                               instr->mnem.src);
}

static void disassemble_dec(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "dec");

    sprintf(instr->mnem.src,     "r%d",               instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- %s - 0x01", instr->mnem.src,
                                                      instr->mnem.src);
}

static void disassemble_inc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "inc");

    sprintf(instr->mnem.src,     "r%d",               instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- %s + 0x01", instr->mnem.src,
                                                      instr->mnem.src);
}

static void disassemble_add(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "add");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s + %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_adc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "adc");

    sprintf(instr->mnem.src,     "r%d",                  instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                  instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s + %s + CF", instr->mnem.dest,
                                                         instr->mnem.dest,
                                                         instr->mnem.src);
}

static void disassemble_adiw(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "adiw");

    sprintf(instr->mnem.src,     "0x%02x",          instr->src.value);
    sprintf(instr->mnem.dest,    "r%d:r%d",         instr->dest.value + 1,
                                                    instr->dest.value + 0);

    sprintf(instr->mnem.comment, "; %s <- %s + %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_sub(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sub");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s - %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_subi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "subi");

    sprintf(instr->mnem.src,     "0x%02x",          instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s - %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_sbc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbc");

    sprintf(instr->mnem.src,     "r%d",                  instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                  instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s - %s - CF", instr->mnem.dest,
                                                         instr->mnem.dest,
                                                         instr->mnem.src);
}

static void disassemble_sbci(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbci");

    sprintf(instr->mnem.src,     "0x%02x",               instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                  instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s - %s - CF", instr->mnem.dest,
                                                         instr->mnem.dest,
                                                         instr->mnem.src);
}

static void disassemble_sbiw(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbiw");

    sprintf(instr->mnem.src,     "0x%02x",          instr->src.value);
    sprintf(instr->mnem.dest,    "r%d:r%d",         instr->dest.value + 1,
                                                    instr->dest.value + 0);

    sprintf(instr->mnem.comment, "; %s <- %s - %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_push(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "push");

    sprintf(instr->mnem.src,     "r%d",              instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; DS[SP--] <- %s", instr->mnem.src);
}

static void disassemble_pop(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "pop");

    sprintf(instr->mnem.src,     "r%d",              instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- DS[++SP]", instr->mnem.src);
}

static void disassemble_in(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "in");

    sprintf(instr->mnem.src,     "0x%02x",         instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- IO[%s]", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_out(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "out");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "0x%02x",         instr->dest.value);

    sprintf(instr->mnem.comment, "; IO[%s] <- %s", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_sbis(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbis");

    sprintf(instr->mnem.src,     "%d",                           instr->src.value);
    sprintf(instr->mnem.dest,    "0x%02x",                       instr->dest.value);

    sprintf(instr->mnem.comment, "; (IO[%s, %s] == 1): PC skip", instr->mnem.dest,
                                                                 instr->mnem.src);
}

static void disassemble_sbic(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbic");

    sprintf(instr->mnem.src,     "%d",                           instr->src.value);
    sprintf(instr->mnem.dest,    "0x%02x",                       instr->dest.value);

    sprintf(instr->mnem.comment, "; (IO[%s, %s] == 0): PC skip", instr->mnem.dest,
                                                                 instr->mnem.src);
}

static void disassemble_sbrc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbrc");

    sprintf(instr->mnem.src,     "%d",                       instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                      instr->dest.value);

    sprintf(instr->mnem.comment, "; (%s[%s] == 0): PC skip", instr->mnem.dest,
                                                             instr->mnem.src);
}

static void disassemble_sbrs(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbrs");

    sprintf(instr->mnem.src,     "%d",                       instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                      instr->dest.value);

    sprintf(instr->mnem.comment, "; (%s[%s] == 1): PC skip", instr->mnem.dest,
                                                             instr->mnem.src);
}

static void disassemble_cpse(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cpse");

    sprintf(instr->mnem.src,     "r%d",                   instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",                   instr->dest.value);

    sprintf(instr->mnem.comment, "; (%s == %s): PC skip", instr->mnem.dest,
                                                          instr->mnem.src);
}

static void disassemble_eor(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "eor");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s ^ %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_ld_x(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "X");
    sprintf(instr->mnem.dest,    "r%d",           instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[X]", instr->mnem.dest);
}

static void disassemble_ld_xi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "X+");
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[X+]", instr->mnem.dest);
}

static void disassemble_ld_dx(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "-X");
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[-X]", instr->mnem.dest);
}

static void disassemble_ld_y(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "Y");
    sprintf(instr->mnem.dest,    "r%d",           instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[Y]", instr->mnem.dest);
}

static void disassemble_ld_yi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "Y+");
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[Y+]", instr->mnem.dest);
}

static void disassemble_ld_dy(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "-Y");
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[-Y]", instr->mnem.dest);
}

static void disassemble_ldd_yq(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ldd");

    sprintf(instr->mnem.src,     "Y+%d",           instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[%s]", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_ldd_zq(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ldd");

    sprintf(instr->mnem.src,     "Z+%d",           instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[%s]", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_ld_z(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "Z");
    sprintf(instr->mnem.dest,    "r%d",           instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[Z]", instr->mnem.dest);
}

static void disassemble_ld_zi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "Z+");
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[Z+]", instr->mnem.dest);
}

static void disassemble_ld_dz(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ld");

    sprintf(instr->mnem.src,     "-Z");
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[-Z]", instr->mnem.dest);
}

static void disassemble_st_x(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",           instr->src.value);
    sprintf(instr->mnem.dest,    "X");

    sprintf(instr->mnem.comment, "; DS[X] <- %s", instr->mnem.src);
}

static void disassemble_st_xi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "X+");

    sprintf(instr->mnem.comment, "; DS[X+] <- %s", instr->mnem.src);
}

static void disassemble_st_dx(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "-X");

    sprintf(instr->mnem.comment, "; DS[-X] <- %s", instr->mnem.src);
}

static void disassemble_st_y(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",           instr->src.value);
    sprintf(instr->mnem.dest,    "Y");

    sprintf(instr->mnem.comment, "; DS[Y] <- %s", instr->mnem.src);
}

static void disassemble_st_yi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "Y+");

    sprintf(instr->mnem.comment, "; DS[Y+] <- %s", instr->mnem.src);
}

static void disassemble_st_dy(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "-Y");

    sprintf(instr->mnem.comment, "; DS[-Y] <- %s", instr->mnem.src);
}

static void disassemble_std_yq(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "std");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "Y+%d",           instr->dest.value);

    sprintf(instr->mnem.comment, "; DS[%s] <- %s", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_st_z(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",           instr->src.value);
    sprintf(instr->mnem.dest,    "Z");

    sprintf(instr->mnem.comment, "; DS[Z] <- %s", instr->mnem.src);
}

static void disassemble_st_zi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "Z+");

    sprintf(instr->mnem.comment, "; DS[Z+] <- %s", instr->mnem.src);
}

static void disassemble_st_dz(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "st");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "-Z");

    sprintf(instr->mnem.comment, "; DS[-Z] <- %s", instr->mnem.src);
}

static void disassemble_std_zq(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "std");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "Z+%d",           instr->dest.value);

    sprintf(instr->mnem.comment, "; DS[%s] <- %s", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_sts(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sts");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "0x%x",           instr->dest.value);

    sprintf(instr->mnem.comment, "; DS[%s] <- %s", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_sts32(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sts");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "0x%04x",         instr->dest.value);

    sprintf(instr->mnem.comment, "; DS[%s] <- %s", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_lds(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lds");

    sprintf(instr->mnem.src,     "0x%x",           instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[%s]", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_lds32(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lds");

    sprintf(instr->mnem.src,     "0x%04x",         instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- DS[%s]", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_xch(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "xch");

    sprintf(instr->mnem.src,     "r%d",                        instr->src.value);
    sprintf(instr->mnem.dest,    "Z");

    sprintf(instr->mnem.comment, "; DS[Z] <- %s, %s <- DS[Z]", instr->mnem.src,
                                                               instr->mnem.src);
}

static void disassemble_brne(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brne");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (ZF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_breq(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "breq");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (ZF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brge(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brge");

    sprintf(instr->mnem.src,     "%d",                                  instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (NF ^ VF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brpl(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brpl");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (NF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brlo(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brlo");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (CF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brlt(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brlt");

    sprintf(instr->mnem.src,     "%d",                                  instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (NF ^ VF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brcc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brcc");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (CF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brvs(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brvs");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (VF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brts(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brts");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (TF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brtc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brtc");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (TF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brmi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brmi");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (NF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brhc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brhc");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (HF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brhs(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brhs");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (HF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brid(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brid");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (IF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brie(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brie");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (IF == 1): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_brvc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "brvc");

    sprintf(instr->mnem.src,     "%d",                             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; (VF == 0): PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_rcall(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "rcall");

    sprintf(instr->mnem.src,     "%d",                  instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- PC + %s + 1", instr->mnem.src);
}

static void disassemble_ret(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ret");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- DS[2|3+SP]");
}

static void disassemble_reti(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "reti");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- DS[2|3+SP]");
}

static void disassemble_icall(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "icall");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- ZH:ZL");
}

static void disassemble_call(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "call");

    sprintf(instr->mnem.src,     "0x%x",       instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- %s", instr->mnem.src);
}

static void disassemble_cp(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cp");

    sprintf(instr->mnem.src,     "r%d",       instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",       instr->dest.value);

    sprintf(instr->mnem.comment, "; %s - %s", instr->mnem.dest,
                                              instr->mnem.src);
}

static void disassemble_cpi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cpi");

    sprintf(instr->mnem.src,     "0x%02x",    (uint8_t) instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",       instr->dest.value);

    sprintf(instr->mnem.comment, "; %s - %s", instr->mnem.dest,
                                              instr->mnem.src);
}

static void disassemble_cpc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cpc");

    sprintf(instr->mnem.src,     "r%d",            instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s - %s - CF", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_lsr(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lsr");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- %s >> 1", instr->mnem.src,
                                                    instr->mnem.src);
}

static void disassemble_asr(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "asr");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- %s >> 1", instr->mnem.src,
                                                    instr->mnem.src);
}

static void disassemble_ror(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ror");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- %s >> 1", instr->mnem.src,
                                                    instr->mnem.src);
}

static void disassemble_swap(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "swap");

    sprintf(instr->mnem.src,     "r%d",                  instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- swap nibbles", instr->mnem.src);
}

static void disassemble_ori(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ori");

    sprintf(instr->mnem.src,     "0x%02x",          (uint8_t) instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s | %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_or_asm(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "or");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s | %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_and_asm(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "and");

    sprintf(instr->mnem.src,     "r%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s & %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_andi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "andi");

    sprintf(instr->mnem.src,     "0x%02x",          instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",             instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- %s & %s", instr->mnem.dest,
                                                    instr->mnem.dest,
                                                    instr->mnem.src);
}

static void disassemble_las(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "las");

    sprintf(instr->mnem.src,     "r%d",                   instr->src.value);
    sprintf(instr->mnem.dest,    "Z");

    sprintf(instr->mnem.comment, "; DS[Z] <- DS[Z] | %s", instr->mnem.src);
}

static void disassemble_lac(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lac");

    sprintf(instr->mnem.src,     "r%d",                            instr->src.value);
    sprintf(instr->mnem.dest,    "Z");

    sprintf(instr->mnem.comment, "; DS[Z] <- DS[Z] * (0xff - %s)", instr->mnem.src);
}

static void disassemble_lat(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lat");

    sprintf(instr->mnem.src,     "r%d",                   instr->src.value);
    sprintf(instr->mnem.dest,    "Z");

    sprintf(instr->mnem.comment, "; DS[Z] <- DS[Z] ^ %s", instr->mnem.src);
}

static void disassemble_com(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "com");

    sprintf(instr->mnem.src,     "r%d",               instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- 0xff - %s", instr->mnem.src,
                                                      instr->mnem.src);
}

static void disassemble_neg(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "neg");

    sprintf(instr->mnem.src,     "r%d",               instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; %s <- 0x00 - %s", instr->mnem.src,
                                                      instr->mnem.src);
}

static void disassemble_bld(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "bld");

    sprintf(instr->mnem.src,     "%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; %s[%s] <- TF", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_bst(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "bst");

    sprintf(instr->mnem.src,     "%d",             instr->src.value);
    sprintf(instr->mnem.dest,    "r%d",            instr->dest.value);

    sprintf(instr->mnem.comment, "; TF <- %s[%s]", instr->mnem.dest,
                                                   instr->mnem.src);
}

static void disassemble_sbi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sbi");

    sprintf(instr->mnem.src,     "%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "0x%02x",            instr->dest.value);

    sprintf(instr->mnem.comment, "; IO[%s, %s] <- 1", instr->mnem.dest,
                                                      instr->mnem.src);
}

static void disassemble_cbi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cbi");

    sprintf(instr->mnem.src,     "%d",                instr->src.value);
    sprintf(instr->mnem.dest,    "0x%02x",            instr->dest.value);

    sprintf(instr->mnem.comment, "; IO[%s, %s] <- 0", instr->mnem.dest,
                                                      instr->mnem.src);
}

static void disassemble_lpm(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lpm");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; r0 <- FLASH[Z]");
}

static void disassemble_lpm_z(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lpm");

    sprintf(instr->mnem.src,     "Z");
    sprintf(instr->mnem.dest,    "r%d",              instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- FLASH[Z]", instr->mnem.dest);
}

static void disassemble_lpm_zi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "lpm");

    sprintf(instr->mnem.src,     "Z+");
    sprintf(instr->mnem.dest,    "r%d",               instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- FLASH[Z+]", instr->mnem.dest);
}

static void disassemble_eicall(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "eicall");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- Z + (EIND << 16)");
}

static void disassemble_eijmp(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "eijmp");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; PC <- Z + (EIND << 16)");
}

static void disassemble_elpm(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "elpm");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; r0 <- FLASH[RAMPZ:Z]");
}

static void disassemble_elpm_z(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "elpm");

    sprintf(instr->mnem.src,     "Z");
    sprintf(instr->mnem.dest,    "r%d",                    instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- FLASH[RAMPZ:Z]", instr->mnem.dest);
}

static void disassemble_elpm_zi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "elpm");

    sprintf(instr->mnem.src,     "Z+");
    sprintf(instr->mnem.dest,    "r%d",                       instr->dest.value);

    sprintf(instr->mnem.comment, "; %s <- FLASH[(RAMPZ:Z)+]", instr->mnem.dest);
}

static void disassemble_des(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "des");

    sprintf(instr->mnem.src,     "0x%02x", instr->src.value);
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; Data Encryption Standard");
}

static void disassemble_sleep(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sleep");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; circuit sleep");
}

static void disassemble_wdr(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "wdr");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; watchdog reset");
}

static void disassemble_break_asm(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "break");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; cpu stop mode");
}

static void disassemble_spm(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "spm");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; FLASH[Z] <- (r1:r0 v 0xffff)");
}

static void disassemble_spm_zi(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "spm");

    sprintf(instr->mnem.src,     "Z+");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; FLASH[Z+] <- (r1:r0 v 0xffff)");
}

static void disassemble_ses(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "ses");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; SF <- 1");
}

static void disassemble_set(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "set");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; TF <- 1");
}

static void disassemble_sev(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sev");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; VF <- 1");
}

static void disassemble_sez(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sez");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; ZF <- 1");
}

static void disassemble_seh(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "seh");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; HF <- 1");
}

static void disassemble_sec(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sec");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; CF <- 1");
}

static void disassemble_sei(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sei");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; IF <- 1");
}

static void disassemble_sen(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "sen");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; NF <- 1");
}

static void disassemble_cls(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cls");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; SF <- 0");
}

static void disassemble_clt(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "clt");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; TF <- 0");
}

static void disassemble_clv(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "clv");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; VF <- 0");
}

static void disassemble_clz(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "clz");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; ZF <- 0");
}

static void disassemble_clh(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "clh");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; HF <- 0");
}

static void disassemble_clc(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "clc");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; CF <- 0");
}

static void disassemble_cli(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cli");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; IF <- 0");
}

static void disassemble_cln(vmcu_instr_t *instr) {

    sprintf(instr->mnem.base,    "cln");

    sprintf(instr->mnem.src,     "");
    sprintf(instr->mnem.dest,    "");

    sprintf(instr->mnem.comment, "; NF <- 0");
}

static void (*disassemble_opcode[VMCU_SET_SIZE]) (vmcu_instr_t *instr) = {

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