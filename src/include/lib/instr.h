#ifndef VMCU_INSTR_H
#define VMCU_INSTR_H

#include <inttypes.h>

/*
 * AVR instruction ID. A numerical
 * identifier for assembly instructions.
 * */
typedef enum {

    VMCU_IID_ENUM_START = -1, // start of enum
    VMCU_IID_NOP,             // nop
    VMCU_IID_MOVW,            // movw Rd+1:Rd, Rr+1:Rr
    VMCU_IID_MUL,             // mul Rd, Rr
    VMCU_IID_MULS,            // muls Rd, Rr
    VMCU_IID_MULSU,           // mulsu Rd, Rr
    VMCU_IID_FMUL,            // fmul Rd, Rr
    VMCU_IID_FMULS,           // fmuls Rd, Rr
    VMCU_IID_FMULSU,          // fmulsu Rd, Rr
    VMCU_IID_LDI,             // ldi Rd, K
    VMCU_IID_RJMP,            // rjmp (+/-) K
    VMCU_IID_JMP,             // jmp K
    VMCU_IID_IJMP,            // ijmp
    VMCU_IID_MOV,             // mov Rd, Rr
    VMCU_IID_DEC,             // dec Rd
    VMCU_IID_INC,             // inc Rd
    VMCU_IID_ADD,             // add Rd, Rr
    VMCU_IID_ADC,             // adc Rd, Rr
    VMCU_IID_ADIW,            // adiw Rd+1:Rd, K
    VMCU_IID_SUB,             // sub Rd, Rr
    VMCU_IID_SUBI,            // subi Rd, K
    VMCU_IID_SBC,             // sbc Rd, Rr
    VMCU_IID_SBCI,            // sbci Rd, K
    VMCU_IID_SBIW,            // sbiw Rd+1:Rd, K
    VMCU_IID_PUSH,            // push Rd
    VMCU_IID_POP,             // pop Rd
    VMCU_IID_IN,              // in Rd, K
    VMCU_IID_OUT,             // out K, Rd
    VMCU_IID_SBIS,            // sbis K, b
    VMCU_IID_SBIC,            // sbic K, b
    VMCU_IID_SBRC,            // sbrc Rd, b
    VMCU_IID_SBRS,            // sbrs Rd, b
    VMCU_IID_CPSE,            // cpse Rd, Rr
    VMCU_IID_EOR,             // eor Rd, Rr
    VMCU_IID_LDX,             // ld Rd, X
    VMCU_IID_LDXI,            // ld Rd, X+
    VMCU_IID_LDDX,            // ld Rd, -X
    VMCU_IID_LDY,             // ld Rd, Y
    VMCU_IID_LDYI,            // ld Rd, Y+
    VMCU_IID_LDDY,            // ld Rd, -Y
    VMCU_IID_LDDYQ,           // ldd Rd, Y+q
    VMCU_IID_LDDZQ,           // ldd Rd, Z+q
    VMCU_IID_LDZ,             // ld Rd, Z
    VMCU_IID_LDZI,            // ld Rd, Z+
    VMCU_IID_LDDZ,            // ld Rd, -Z
    VMCU_IID_STX,             // st X, Rr
    VMCU_IID_STXI,            // st X+, Rr
    VMCU_IID_STDX,            // st -X, Rr
    VMCU_IID_STY,             // st Y, Rr
    VMCU_IID_STYI,            // st Y+, Rr
    VMCU_IID_STDY,            // st -Y, Rr
    VMCU_IID_STDYQ,           // std Y+q, Rr
    VMCU_IID_STZ,             // st Z, Rr
    VMCU_IID_STZI,            // st Z+, Rr
    VMCU_IID_STDZ,            // st -Z, Rr
    VMCU_IID_STDZQ,           // std Z+q, Rr
    VMCU_IID_STS,             // sts K, Rr
    VMCU_IID_STS32,           // sts K, Rr (32-bit)
    VMCU_IID_LDS,             // lds Rd, K
    VMCU_IID_LDS32,           // lds Rd, K (32-bit)
    VMCU_IID_XCH,             // xch Rd, Rr
    VMCU_IID_BRNE,            // brne (+/-) K
    VMCU_IID_BREQ,            // breq (+/-) K
    VMCU_IID_BRGE,            // brge (+/-) K
    VMCU_IID_BRPL,            // brpl (+/-) K
    VMCU_IID_BRLO,            // brlo (+/-) K
    VMCU_IID_BRLT,            // brlt (+/-) K
    VMCU_IID_BRCC,            // brcc (+/-) K
    VMCU_IID_BRVS,            // brvs (+/-) K
    VMCU_IID_BRTS,            // brts (+/-) K
    VMCU_IID_BRTC,            // brtc (+/-) K
    VMCU_IID_BRMI,            // brmi (+/-) K
    VMCU_IID_BRHC,            // brhc (+/-) K
    VMCU_IID_BRHS,            // brhs (+/-) K
    VMCU_IID_BRID,            // brid (+/-) K
    VMCU_IID_BRIE,            // brie (+/-) K
    VMCU_IID_BRVC,            // brvc (+/-) K
    VMCU_IID_RCALL,           // rcall (+/-) K
    VMCU_IID_RET,             // ret
    VMCU_IID_RETI,            // reti
    VMCU_IID_ICALL,           // icall
    VMCU_IID_CALL,            // call K
    VMCU_IID_CP,              // cp Rd, Rr
    VMCU_IID_CPI,             // cpi Rd, K
    VMCU_IID_CPC,             // cpc Rd, Rr
    VMCU_IID_LSR,             // lsr Rd
    VMCU_IID_ASR,             // asr Rd
    VMCU_IID_ROR,             // ror Rd
    VMCU_IID_SWAP,            // swap Rd
    VMCU_IID_ORI,             // ori Rd, K
    VMCU_IID_OR,              // or Rd, Rr
    VMCU_IID_AND,             // and Rd, Rr
    VMCU_IID_ANDI,            // andi Rd, K
    VMCU_IID_LAS,             // las Z, Rr
    VMCU_IID_LAC,             // lac Z, Rr
    VMCU_IID_LAT,             // lat Z, Rr
    VMCU_IID_COM,             // com Rd
    VMCU_IID_NEG,             // neg Rd
    VMCU_IID_BLD,             // bld Rd, b
    VMCU_IID_BST,             // bst Rd, b
    VMCU_IID_SBI,             // sbi K, b
    VMCU_IID_CBI,             // cbi K, b
    VMCU_IID_LPM,             // lpm
    VMCU_IID_LPMZ,            // lpm Rd, Z
    VMCU_IID_LPMZI,           // lpm Rd, Z+
    VMCU_IID_EICALL,          // eicall
    VMCU_IID_EIJMP,           // eijmp
    VMCU_IID_ELPM,            // elpm
    VMCU_IID_ELPMZ,           // elpm Rd, Z
    VMCU_IID_ELPMZI,          // elpm Rd, Z+
    VMCU_IID_DES,             // des
    VMCU_IID_SLEEP,           // sleep
    VMCU_IID_WDR,             // wdr
    VMCU_IID_BREAK,           // break
    VMCU_IID_SPM,             // spm Z, Rr
    VMCU_IID_SPMZI,           // spm Z+, Rr
    VMCU_IID_SES,             // ses
    VMCU_IID_SET,             // set
    VMCU_IID_SEV,             // sev
    VMCU_IID_SEZ,             // sez
    VMCU_IID_SEH,             // seh
    VMCU_IID_SEC,             // sec
    VMCU_IID_SEI,             // sei
    VMCU_IID_SEN,             // sen
    VMCU_IID_CLS,             // cls
    VMCU_IID_CLT,             // clt
    VMCU_IID_CLV,             // clv
    VMCU_IID_CLZ,             // clz
    VMCU_IID_CLH,             // clh
    VMCU_IID_CLC,             // clc
    VMCU_IID_CLI,             // cli
    VMCU_IID_CLN,             // cln
    VMCU_IID_ENUM_END         // end of enum

} vmcu_iid_t;

/* 
 * Enumeration of AVR General Purpose
 * Registers (GPRs)
 * */
typedef enum {

    VMCU_REGISTER_ENUM_START = -1, // start of enum
    VMCU_REGISTER_R0,              // r0
    VMCU_REGISTER_R1,              // r1
    VMCU_REGISTER_R2,              // r2
    VMCU_REGISTER_R3,              // r3
    VMCU_REGISTER_R4,              // r4
    VMCU_REGISTER_R5,              // r5
    VMCU_REGISTER_R6,              // r6
    VMCU_REGISTER_R7,              // r7
    VMCU_REGISTER_R8,              // r8
    VMCU_REGISTER_R9,              // r9
    VMCU_REGISTER_R10,             // r10
    VMCU_REGISTER_R11,             // r11
    VMCU_REGISTER_R12,             // r12
    VMCU_REGISTER_R13,             // r13
    VMCU_REGISTER_R14,             // r14
    VMCU_REGISTER_R15,             // r15
    VMCU_REGISTER_R16,             // r16
    VMCU_REGISTER_R17,             // r17
    VMCU_REGISTER_R18,             // r18
    VMCU_REGISTER_R19,             // r19
    VMCU_REGISTER_R20,             // r20
    VMCU_REGISTER_R21,             // r21
    VMCU_REGISTER_R22,             // r22
    VMCU_REGISTER_R23,             // r23
    VMCU_REGISTER_R24,             // r24
    VMCU_REGISTER_R25,             // r25
    VMCU_REGISTER_R26,             // r26
    VMCU_REGISTER_R27,             // r27
    VMCU_REGISTER_R28,             // r28
    VMCU_REGISTER_R29,             // r29
    VMCU_REGISTER_R30,             // r30
    VMCU_REGISTER_R31,             // r31
    VMCU_REGISTER_ENUM_END         // end of enum

} vmcu_register_t;

/*
 * A structure representing a
 * registerpair of the form rh:rl
 * */
typedef struct vmcu_registerpair {

    vmcu_register_t low;  // low byte register
    vmcu_register_t high; // high byte register

} vmcu_registerpair_t;

/*
 * Instruction groups.
 * TODO put reference here
 * */
typedef enum {

    VMCU_GROUP_ENUM_START = -1, // start of enum
    VMCU_GROUP_MATH_LOGIC,      // math logic instructions (mul, adiw, ...)
    VMCU_GROUP_SYS_CTRL,        // system control instructions (sleep, wdr, ...)
    VMCU_GROUP_TRANSFER,        // transfer instructions (movw, ldi, ...)
    VMCU_GROUP_FLOW,            // flow control instructions (jmp, icall, ...)
    VMCU_GROUP_BIT,             // bit instructions (sbis, cbi, ...)
    VMCU_GROUP_ENUM_END         // end of enum

} vmcu_group_t;

/*
 * A bitfield holding read write access
 * information. A '1' (bit set) indicates
 * that the instruction has read/write access.
 * A '0' (bit cleared) indicates no access.
 * */
typedef struct vmcu_rw_access {

    unsigned int registers : 1; // accessing registers?
    unsigned int flash     : 1; // accessing flash?
    unsigned int stack     : 1; // accessing stack?
    unsigned int io        : 1; // acccessing io?
    unsigned int ds        : 1; // accessing data segment?
    unsigned int sp        : 1; // accessing stack pointer?
    unsigned int pc        : 1; // accessing program counter?
    unsigned int c_flag    : 1; // accessing carry flag?
    unsigned int z_flag    : 1; // accessing zero flag?
    unsigned int n_flag    : 1; // accessing negative flag?
    unsigned int v_flag    : 1; // accessing overflow flag?
    unsigned int s_flag    : 1; // accessing sign flag?
    unsigned int h_flag    : 1; // accessing half-carry flag?
    unsigned int t_flag    : 1; // accessing t-flag?
    unsigned int i_flag    : 1; // accessing interrupt flag?

} vmcu_rw_access_t;

/* 
 * Type of instruction operand. See
 * wikipedia.org/wiki/Atmel_AVR_instruction_set
 * */
typedef enum {

    VMCU_OPTYPE_ENUM_START = -1, // no operand
    VMCU_OPTYPE_R,               // general purpose register
    VMCU_OPTYPE_RP,              // pair of general purpose registers
    VMCU_OPTYPE_X,               // x pointer register (r27:r26)
    VMCU_OPTYPE_Y,               // y pointer register (r29:r28)
    VMCU_OPTYPE_Z,               // z pointer register (r31:r30)
    VMCU_OPTYPE_B,               // bit number in gpr or io register
    VMCU_OPTYPE_K4,              // 4-bit immediate unsigned constant
    VMCU_OPTYPE_K6,              // 6-bit immediate unsigned constant
    VMCU_OPTYPE_K8,              // 8-bit immediate constant, signedness irrelevant
    VMCU_OPTYPE_IO5,             // 5-bit io address
    VMCU_OPTYPE_IO6,             // 6-bit io address
    VMCU_OPTYPE_D7,              // 7-bit data address
    VMCU_OPTYPE_D16,             // 16-bit data address
    VMCU_OPTYPE_P22,             // 22-bit program address
    VMCU_OPTYPE_S7,              // 7-bit signed displacement in units of words
    VMCU_OPTYPE_S12,             // 12-bit signed displacement in units of words
    VMCU_OPTYPE_ENUM_END         // end of enum

} vmcu_optype_t;

/*
 * Instruction operand structure.
 * vmcu_operand.type indicates which
 * union member should be accessed.
 * */
/*
 todo: decide
 backup:
typedef struct vmcu_operand {

    union {

        uint8_t             k;  // if .type = K4, K6 or K8
        uint8_t             b;  // if .type = B 
        uint8_t             io; // if .type = IO5 or IO6
        uint16_t            d;  // if .type = D7 or D16
        uint32_t            p;  // if .type = P22
        int16_t             s;  // if .type = S7 or S12
        vmcu_register_t     r;  // if .type = R
        vmcu_registerpair_t rp; // if .type = RP, X, Y or Z
    };

    char          str[8];       // operand string
    vmcu_optype_t type;         // type of operand

} vmcu_operand_t;
*/
typedef struct vmcu_operand {

    union {

        uint8_t             uimm8;  // if .type = K4, K6, K8, B, IO5 or IO6
        uint16_t            uimm16; // if .type = D7 or D16
        uint32_t            uimm32; // if .type = P22
        int16_t             imm16;  // if .type = S7 or S12
        vmcu_register_t     r;      // if .type = R
        vmcu_registerpair_t rp;     // if .type = RP, X, Y or Z
    };
    
    char          str[8];       // operand string
    vmcu_optype_t type;         // type of operand

} vmcu_operand_t;

/*
 * A structure representing a single
 * 16/32-bit machine instruction.
 * TODO add cycles
 * */
typedef struct vmcu_instr {

    vmcu_iid_t       id;          // numerical instruction identifier
    vmcu_group_t     group;       // instruction group
    vmcu_rw_access_t reads;       // implicit/explicit reads
    vmcu_rw_access_t writes;      // implicit/explicit writes
    char             str[8];      // instruction string
    uint32_t         addr;        // flash address of instruction
    unsigned int     n_words;     // number of 16-bit words
    uint16_t         words[2];    // raw 16-bit words of instruction
    unsigned int     n_operands;  // number of operands
    vmcu_operand_t   operands[2]; // instruction operands

} vmcu_instr_t;

#endif
