/* Instruction Decomposer Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/decomposer/decomposer.h"
#include "engine/include/decoder/decoder.h"
#include "engine/include/decoder/opcode.h"
#include "engine/include/analyzer/report/plain.h"

// Project Headers (shared)
#include "shared/include/misc/bitmanip.h"
#include "shared/include/collections/array.h"

/* Forward Declaration of static Members */

static void decompose_dw(plain_t *p);
static void (*decompose_opcode[SET_SIZE]) (plain_t *p);

/* --- Extern --- */

int decompose(const char *hex_file, array_t *buffer) {
    
    if(decode_hex(hex_file, buffer) < 0)
        return -1;
    
    for(int i = 0; i < buffer->size; i++) {
        
        plain_t *p = (plain_t*) array_at(buffer, i);
        
        if(p->exec == false) {
            
            decompose_dw(p);
            continue;
        }
        
        (*decompose_opcode[ p->key ])(p);
    }
        
    return 0;
}

/* --- Static --- */

void decompose_dw(plain_t *p) {
    
    p->src.type   = UIMM16;
    p->dest.type  = EMPTY;
    
    p->src.value  = p->opcode;
}

void decompose_nop(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_movw(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;

    p->src.value  = extract(p->opcode, 0, 4, 0) * 2;
    p->dest.value = extract(p->opcode, 4, 8, 0) * 2;
}

void decompose_mul(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0); 
}

void decompose_muls(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 4, 0) + 16;
    p->dest.value = extract(p->opcode, 4, 8, 0) + 16;
}

void decompose_mulsu(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0) + 16;
    p->dest.value = extract(p->opcode, 4, 7, 0) + 16;
}

void decompose_fmul(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0) + 16;
    p->dest.value = extract(p->opcode, 4, 7, 0) + 16;
}

void decompose_fmuls(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0) + 16;
    p->dest.value = extract(p->opcode, 4, 7, 0) + 16;
}

void decompose_fmulsu(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0) + 16;
    p->dest.value = extract(p->opcode, 4, 7, 0) + 16;
}

void decompose_ldi(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = IMM8;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 8, 12, 4);
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}

void decompose_rjmp(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 0, 12, 0);
    
    if(((0x01 << 11) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 12) * -1;
}

void decompose_jmp(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = (opc & 0x1ffff) + (opc & 0x1f00000);
}

void decompose_ijmp(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_mov(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_dec(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
} 

void decompose_inc(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}

void decompose_add(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_adc(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_adiw(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = UIMM8;
    p->dest.type  = REGISTERPAIR;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 6, 8, 4);
    p->dest.value = (2 * extract(opc, 4, 6, 0)) + 24;
}

void decompose_sub(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_subi(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = IMM8;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 8, 12, 4);
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}

void decompose_sbc(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_sbci(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = IMM8;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 8, 12, 4);
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}

void decompose_sbiw(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = UIMM8;
    p->dest.type  = REGISTERPAIR;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 6, 8, 4);
    p->dest.value = (2 * extract(opc, 4, 6, 0)) + 24; 
}

void decompose_push(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(opc, 4, 9, 0);
}

void decompose_pop(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(opc, 4, 9, 0);
}

void decompose_in(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = IODIRECT;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 11, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_out(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = IODIRECT;
    
    p->src.value  = extract(opc, 4, 9, 0);
    p->dest.value = extract(opc, 0, 4, 0) + extract(opc, 9, 11, 4);
}

void decompose_sbis(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = IODIRECT;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 3, 8, 0);
}

void decompose_sbic(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = IODIRECT;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 3, 8, 0);
}

void decompose_sbrc(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_sbrs(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 4, 9, 0); 
}

void decompose_cpse(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_eor(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_ld_x(plain_t *p) {
    
    p->src.type   = XPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 26;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ld_xi(plain_t *p) {

    p->src.type   = XPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 26;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ld_dx(plain_t *p) {
    
    p->src.type   = XPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 26;
    p->dest.value = extract(p->opcode, 4, 9, 0);
} 

void decompose_ld_y(plain_t *p) {
    
    p->src.type   = YPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 28;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ld_yi(plain_t *p) {
    
    p->src.type   = YPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 28;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ld_dy(plain_t *p) {
    
    p->src.type   = YPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 28;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ldd_yq(plain_t *p) {

    const int opc = p->opcode;
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 3, 0) 
                  + extract(opc, 10, 12, 3)
                  + extract(opc, 13, 14, 5);
                  
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_ldd_zq(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 3, 0) 
                  + extract(opc, 10, 12, 3)
                  + extract(opc, 13, 14, 5);
                  
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_ld_z(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ld_zi(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_ld_dz(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_st_x(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = XPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 26;
}

void decompose_st_xi(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = XPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 26;
}

void decompose_st_dx(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = XPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 26;
}

void decompose_st_y(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = YPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 28;
}

void decompose_st_yi(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = YPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 28;
}

void decompose_st_dy(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = YPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 28;
}

void decompose_std_yq(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = IMM;
    
    p->src.value  = extract(opc, 4, 9, 0);
    
    p->dest.value = extract(opc, 0, 3, 0) 
                  + extract(opc, 10, 12, 3)
                  + extract(opc, 13, 14, 5);
}

void decompose_st_z(plain_t *p) {
 
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

void decompose_st_zi(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

void decompose_st_dz(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

void decompose_std_zq(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = IMM;
    
    p->src.value  = extract(opc, 4, 9, 0);
    
    p->dest.value = extract(opc, 0, 3, 0) 
                  + extract(opc, 10, 12, 3)
                  + extract(opc, 13, 14, 5);
}

void decompose_sts(plain_t *p) {
    
    const int opc = p->opcode;
    const int tmp = extract(opc, 0, 4, 0) + extract(opc, 8, 11, 4);
    
    p->src.type   = REGISTER;
    p->dest.type  = IMM;
    
    p->src.value  = extract(opc, 4, 8, 0) + 16;
    
    p->dest.value = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) | 
                     ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                     ((0x0f & opc));
}

void decompose_sts32(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = UIMM16;
    
    p->src.value  = extract(p->opcode, 20, 25, 0);
    p->dest.value = extract(p->opcode, 0, 16, 0);
}

void decompose_lds(plain_t *p) {
    
    const int opc = p->opcode;
    const int tmp = extract(opc, 0, 4, 0) + extract(opc, 8, 11, 4);
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = (~(0x10 & tmp) << 3) | ((0x10 & tmp) << 2) | 
                     ((0x40 & tmp) >> 1) | ((0x20 & tmp) >> 1) |
                     ((0x0f & opc));
                     
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}

void decompose_lds32(plain_t *p) {
    
    p->src.type   = UIMM16;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 16, 0);
    p->dest.value = extract(p->opcode, 20, 25, 0);
}

void decompose_xch(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

void decompose_brne(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

void decompose_breq(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

void decompose_brge(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

void decompose_brpl(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}
 
void decompose_brlo(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brlt(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;        
}
 
void decompose_brcc(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brcs(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}
 
void decompose_brvs(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brts(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brtc(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}
 
void decompose_brmi(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brhc(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brhs(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brid(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brie(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;    
}

void decompose_brvc(plain_t *p) {
 
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 3, 10, 0);
    
    if(((0x01 << 6) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 7) * -1;
}

void decompose_rcall(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 0, 12, 0);
    
    if(((0x01 << 11) & p->src.value) != 0x00)
        p->src.value = comp(p->src.value, 12) * -1;
}

void decompose_ret(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_reti(plain_t *p) {

    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_icall(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}
 
void decompose_call(plain_t *p) {

    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = (p->opcode & 0x1ffff) 
                  + (p->opcode & 0x1f00000);
} 
 
void decompose_cp(plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}
 
void decompose_cpi(plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = UIMM8;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 8, 12, 4);
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}
 
void decompose_cpc(plain_t *p) {

    const int opc = p->opcode;

    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_lsr(plain_t *p) {

    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}

void decompose_asr(plain_t *p) {

    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}

void decompose_ror(plain_t *p) {

    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}
 
void decompose_swap(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}

void decompose_ori(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = UIMM8;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 8, 12, 4);
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}
 
void decompose_or_asm(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
} 

void decompose_and_asm(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = REGISTER;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 9, 10, 4);
    p->dest.value = extract(opc, 4, 9, 0);
}

void decompose_andi(plain_t *p) {
    
    const int opc = p->opcode;
    
    p->src.type   = UIMM8;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(opc, 0, 4, 0) + extract(opc, 8, 12, 4);
    p->dest.value = extract(opc, 4, 8, 0) + 16;
}
 
void decompose_las(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}
 
void decompose_lac(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}
 
void decompose_lat(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = ZPTR;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
    p->dest.value = 30;
}

void decompose_com(plain_t *p) {
 
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}
 
void decompose_neg(plain_t *p) {
    
    p->src.type   = REGISTER;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 9, 0);
}
 
void decompose_bld(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 4, 9, 0);
}
 
void decompose_bst(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = REGISTER;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 4, 9, 0);
}
 
void decompose_sbi(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = IODIRECT;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 3, 8, 0);
}

void decompose_cbi(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = IODIRECT;
    
    p->src.value  = extract(p->opcode, 0, 3, 0);
    p->dest.value = extract(p->opcode, 3, 8, 0);
}

void decompose_lpm(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_lpm_z(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}
 
void decompose_lpm_zi(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_eicall(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_eijmp(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_elpm(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_elpm_z(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_elpm_zi(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = REGISTER;
    
    p->src.value  = 30;
    p->dest.value = extract(p->opcode, 4, 9, 0);
}

void decompose_des(plain_t *p) {
 
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 8, 0);
}

void decompose_sleep(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_wdr(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_break_asm(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_spm(plain_t *p) {

    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_spm_zi(plain_t *p) {
    
    p->src.type   = ZPTR;
    p->dest.type  = EMPTY;
    
    p->src.value  = 30;
}

void decompose_ses(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_set(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_sev(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_sez(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}
 
void decompose_seh(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_sec(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_sei(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_sen(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_cls(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}
 
void decompose_clt(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}
 
void decompose_clv(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_clz(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_clh(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_clc(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_cli(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}
 
void decompose_cln(plain_t *p) {
    
    p->src.type   = EMPTY;
    p->dest.type  = EMPTY;
}

void decompose_bclr(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;
    
    p->src.value  = extract(p->opcode, 4, 7, 0);
} 
    
void decompose_bset(plain_t *p) {
    
    p->src.type   = IMM;
    p->dest.type  = EMPTY;

    p->src.value  = extract(p->opcode, 4, 7, 0);
}

static void (*decompose_opcode[SET_SIZE]) (plain_t *p) = {

    decompose_nop, 
    decompose_movw, 
    decompose_mul,
    decompose_muls, 
    decompose_mulsu, 
    decompose_fmul, 
    decompose_fmuls,
    decompose_fmulsu,
    decompose_ldi, 
    decompose_rjmp, 
    decompose_jmp, 
    decompose_ijmp, 
    decompose_mov, 
    decompose_dec, 
    decompose_inc, 
    decompose_add, 
    decompose_adc, 
    decompose_adiw, 
    decompose_sub, 
    decompose_subi, 
    decompose_sbc, 
    decompose_sbci, 
    decompose_sbiw, 
    decompose_push, 
    decompose_pop, 
    decompose_in, 
    decompose_out, 
    decompose_sbis, 
    decompose_sbic,
    decompose_sbrc, 
    decompose_sbrs, 
    decompose_cpse, 
    decompose_eor, 
    decompose_ld_x, 
    decompose_ld_xi, 
    decompose_ld_dx, 
    decompose_ld_y, 
    decompose_ld_yi, 
    decompose_ld_dy, 
    decompose_ldd_yq, 
    decompose_ldd_zq, 
    decompose_ld_z, 
    decompose_ld_zi, 
    decompose_ld_dz,
    decompose_st_x, 
    decompose_st_xi, 
    decompose_st_dx, 
    decompose_st_y,
    decompose_st_yi, 
    decompose_st_dy,
    decompose_std_yq, 
    decompose_st_z,
    decompose_st_zi,
    decompose_st_dz,
    decompose_std_zq, 
    decompose_sts, 
    decompose_sts32, 
    decompose_lds,
    decompose_lds32, 
    decompose_xch, 
    decompose_brne, 
    decompose_breq, 
    decompose_brge, 
    decompose_brpl, 
    decompose_brlo, 
    decompose_brlt, 
    decompose_brcc, 
    decompose_brcs, 
    decompose_brvs, 
    decompose_brts, 
    decompose_brtc, 
    decompose_brmi, 
    decompose_brhc,
    decompose_brhs,
    decompose_brid,
    decompose_brie,
    decompose_brvc,
    decompose_rcall, 
    decompose_ret, 
    decompose_reti, 
    decompose_icall, 
    decompose_call, 
    decompose_cp, 
    decompose_cpi, 
    decompose_cpc, 
    decompose_lsr, 
    decompose_asr, 
    decompose_ror, 
    decompose_swap, 
    decompose_ori, 
    decompose_or_asm, 
    decompose_and_asm, 
    decompose_andi, 
    decompose_las, 
    decompose_lac, 
    decompose_lat,
    decompose_com, 
    decompose_neg, 
    decompose_bld, 
    decompose_bst, 
    decompose_sbi,
    decompose_cbi,
    decompose_lpm, 
    decompose_lpm_z, 
    decompose_lpm_zi, 
    decompose_eicall,
    decompose_eijmp,
    decompose_elpm,
    decompose_elpm_z,
    decompose_elpm_zi,
    decompose_des,
    decompose_sleep,
    decompose_wdr,
    decompose_break_asm,
    decompose_spm,
    decompose_spm_zi,
    decompose_ses, 
    decompose_set, 
    decompose_sev, 
    decompose_sez, 
    decompose_seh, 
    decompose_sec, 
    decompose_sei, 
    decompose_sen, 
    decompose_cls, 
    decompose_clt, 
    decompose_clv, 
    decompose_clz, 
    decompose_clh, 
    decompose_clc, 
    decompose_cli, 
    decompose_cln, 
    decompose_bclr, 
    decompose_bset
};


