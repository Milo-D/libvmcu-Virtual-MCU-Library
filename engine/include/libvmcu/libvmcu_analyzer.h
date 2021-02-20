/* libvmcu - Analyzer Library Interface */

#ifndef LIBVMCU_ANALYZER_INTERFACE_H
#define LIBVMCU_ANALYZER_INTERFACE_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

/*
 * libvmcu - Virtual Microcontroller Library
 *
 * Copyright (C) 2021 David Milosevic - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BDS 3-Clause license.
 *
 * You should have received a copy of the BSD-3 license with
 * this file. If not, please contact me.
 *
 * Email: David.Milosevic@web.de
 *
 * */

/* <------------------------------------------- Enumeration -------------------------------------------> */

typedef enum {                                /// instruction keys

    VMCU_DATA = -1,                           ///< data (.dw)

    VMCU_NOP,                                 ///< nop
    VMCU_MOVW,                                ///< movw Rd+1:Rd, Rr+1:Rr
    VMCU_MUL,                                 ///< mul Rd, Rr
    VMCU_MULS,                                ///< muls Rd, Rr
    VMCU_MULSU,                               ///< mulsu Rd, Rr
    VMCU_FMUL,                                ///< fmul Rd, Rr
    VMCU_FMULS,                               ///< fmuls Rd, Rr
    VMCU_FMULSU,                              ///< fmulsu Rd, Rr
    VMCU_LDI,                                 ///< ldi Rd, K
    VMCU_RJMP,                                ///< rjmp (+/-) K
    VMCU_JMP,                                 ///< jmp K
    VMCU_IJMP,                                ///< ijmp
    VMCU_MOV,                                 ///< mov Rd, Rr
    VMCU_DEC,                                 ///< dec Rd
    VMCU_INC,                                 ///< inc Rd
    VMCU_ADD,                                 ///< add Rd, Rr
    VMCU_ADC,                                 ///< adc Rd, Rr
    VMCU_ADIW,                                ///< adiw Rd+1:Rd, K
    VMCU_SUB,                                 ///< sub Rd, Rr
    VMCU_SUBI,                                ///< subi Rd, K
    VMCU_SBC,                                 ///< sbc Rd, Rr
    VMCU_SBCI,                                ///< sbci Rd, K
    VMCU_SBIW,                                ///< sbiw Rd+1:Rd, K
    VMCU_PUSH,                                ///< push Rd
    VMCU_POP,                                 ///< pop Rd
    VMCU_IN,                                  ///< in Rd, K
    VMCU_OUT,                                 ///< out K, Rd
    VMCU_SBIS,                                ///< sbis K, b
    VMCU_SBIC,                                ///< sbic K, b
    VMCU_SBRC,                                ///< sbrc Rd, b
    VMCU_SBRS,                                ///< sbrs Rd, b
    VMCU_CPSE,                                ///< cpse Rd, Rr
    VMCU_EOR,                                 ///< eor Rd, Rr
    VMCU_LDX,                                 ///< ld Rd, X
    VMCU_LDXI,                                ///< ld Rd, X+
    VMCU_LDDX,                                ///< ld Rd, -X
    VMCU_LDY,                                 ///< ld Rd, Y
    VMCU_LDYI,                                ///< ld Rd, Y+
    VMCU_LDDY,                                ///< ld Rd, -Y
    VMCU_LDDYQ,                               ///< ldd Rd, Y+q
    VMCU_LDDZQ,                               ///< ldd Rd, Z+q
    VMCU_LDZ,                                 ///< ld Rd, Z
    VMCU_LDZI,                                ///< ld Rd, Z+
    VMCU_LDDZ,                                ///< ld Rd, -Z
    VMCU_STX,                                 ///< st X, Rr
    VMCU_STXI,                                ///< st X+, Rr
    VMCU_STDX,                                ///< st -X, Rr
    VMCU_STY,                                 ///< st Y, Rr
    VMCU_STYI,                                ///< st Y+, Rr
    VMCU_STDY,                                ///< st -Y, Rr
    VMCU_STDYQ,                               ///< std Y+q, Rr
    VMCU_STZ,                                 ///< st Z, Rr
    VMCU_STZI,                                ///< st Z+, Rr
    VMCU_STDZ,                                ///< st -Z, Rr
    VMCU_STDZQ,                               ///< std Z+q, Rr
    VMCU_STS,                                 ///< sts K, Rr
    VMCU_STS32,                               ///< sts K, Rr (32-bit)
    VMCU_LDS,                                 ///< lds Rd, K
    VMCU_LDS32,                               ///< lds Rd, K (32-bit)
    VMCU_XCH,                                 ///< xch Rd, Rr
    VMCU_BRNE,                                ///< brne (+/-) K
    VMCU_BREQ,                                ///< breq (+/-) K
    VMCU_BRGE,                                ///< brge (+/-) K
    VMCU_BRPL,                                ///< brpl (+/-) K
    VMCU_BRLO,                                ///< brlo (+/-) K
    VMCU_BRLT,                                ///< brlt (+/-) K
    VMCU_BRCC,                                ///< brcc (+/-) K
    VMCU_BRCS,                                ///< brcs (+/-) K
    VMCU_BRVS,                                ///< brvs (+/-) K
    VMCU_BRTS,                                ///< brts (+/-) K
    VMCU_BRTC,                                ///< brtc (+/-) K
    VMCU_BRMI,                                ///< brmi (+/-) K
    VMCU_BRHC,                                ///< brhc (+/-) K
    VMCU_BRHS,                                ///< brhs (+/-) K
    VMCU_BRID,                                ///< brid (+/-) K
    VMCU_BRIE,                                ///< brie (+/-) K
    VMCU_BRVC,                                ///< brvc (+/-) K
    VMCU_RCALL,                               ///< rcall (+/-) K
    VMCU_RET,                                 ///< ret
    VMCU_RETI,                                ///< reti
    VMCU_ICALL,                               ///< icall
    VMCU_CALL,                                ///< call K
    VMCU_CP,                                  ///< cp Rd, Rr
    VMCU_CPI,                                 ///< cpi Rd, K
    VMCU_CPC,                                 ///< cpc Rd, Rr
    VMCU_LSR,                                 ///< lsr Rd
    VMCU_ASR,                                 ///< asr Rd
    VMCU_ROR,                                 ///< ror Rd
    VMCU_SWAP,                                ///< swap Rd
    VMCU_ORI,                                 ///< ori Rd, K
    VMCU_OR,                                  ///< or Rd, Rr
    VMCU_AND,                                 ///< and Rd, Rr
    VMCU_ANDI,                                ///< andi Rd, K
    VMCU_LAS,                                 ///< las Z, Rr
    VMCU_LAC,                                 ///< lac Z, Rr
    VMCU_LAT,                                 ///< lat Z, Rr
    VMCU_COM,                                 ///< com Rd
    VMCU_NEG,                                 ///< neg Rd
    VMCU_BLD,                                 ///< bld Rd, b
    VMCU_BST,                                 ///< bst Rd, b
    VMCU_SBI,                                 ///< sbi K, b
    VMCU_CBI,                                 ///< cbi K, b
    VMCU_LPM,                                 ///< lpm
    VMCU_LPMZ,                                ///< lpm Rd, Z
    VMCU_LPMZI,                               ///< lpm Rd, Z+
    VMCU_EICALL,                              ///< eicall
    VMCU_EIJMP,                               ///< eijmp
    VMCU_ELPM,                                ///< elpm
    VMCU_ELPMZ,                               ///< elpm Rd, Z
    VMCU_ELPMZI,                              ///< elpm Rd, Z+
    VMCU_DES,                                 ///< des
    VMCU_SLEEP,                               ///< sleep
    VMCU_WDR,                                 ///< wdr
    VMCU_BREAK,                               ///< break
    VMCU_SPM,                                 ///< spm Z, Rr
    VMCU_SPMZI,                               ///< spm Z+, Rr
    VMCU_SES,                                 ///< ses
    VMCU_SET,                                 ///< set
    VMCU_SEV,                                 ///< sev
    VMCU_SEZ,                                 ///< sez
    VMCU_SEH,                                 ///< seh
    VMCU_SEC,                                 ///< sec
    VMCU_SEI,                                 ///< sei
    VMCU_SEN,                                 ///< sen
    VMCU_CLS,                                 ///< cls
    VMCU_CLT,                                 ///< clt
    VMCU_CLV,                                 ///< clv
    VMCU_CLZ,                                 ///< clz
    VMCU_CLH,                                 ///< clh
    VMCU_CLC,                                 ///< clc
    VMCU_CLI,                                 ///< cli
    VMCU_CLN,                                 ///< cln
    VMCU_BCLR,                                ///< blcr K
    VMCU_BSET,                                ///< bset K

} VMCU_IKEY;

typedef enum {                                /// operand types

    VMCU_IMM,                                 ///< immediate type (> 8-bit, < 16-bit)
    VMCU_IMM8,                                ///< 8-bit immediate type (signed)
    VMCU_UIMM8,                               ///< 8-bit immediate type (unsigned)
    VMCU_UIMM16,                              ///< 16-bit immediate type (unsigned)
    VMCU_REGISTER,                            ///< register type
    VMCU_REGISTERPAIR,                        ///< register-pair type (XL, XH, ...)
    VMCU_IODIRECT,                            ///< io-direct type
    VMCU_XPTR,                                ///< X pointer type
    VMCU_YPTR,                                ///< Y pointer type
    VMCU_ZPTR,                                ///< Z pointer type
    VMCU_EMPTY                                ///< no operand

} VMCU_OPTYPE;

/* <------------------------------------------- Structures --------------------------------------------> */

typedef struct vmcu_operand {                 ///< operand structure

    int32_t value;                            ///< operand value
    VMCU_OPTYPE type;                         ///< operand type

} vmcu_operand_t;

typedef struct vmcu_caller {                  ///< caller/jumper structure

    uint16_t addr;                            ///< address of caller/jumper

} vmcu_caller_t;

typedef struct vmcu_label {                   ///< label structure

    uint16_t id;                              ///< unique label identifier
    uint16_t addr;                            ///< label address

    int32_t ncallers;                         ///< caller/jumper count
    vmcu_caller_t *caller;                    ///< caller/jumper list

} vmcu_label_t;

typedef struct vmcu_plain {                   ///< plain structure (represents an instruction)

    int opcode;                               ///< 16-bit or 32-bit opcode (todo: change type to uint32)
    int addr;

    bool exec;                                ///< instruction executable ?
    bool dword;                               ///< 32-bit instruction ?

    char *mnem;                               ///< disassembled mnemonic of instruction

    VMCU_IKEY key;                            ///< instruction key (numerical instruction identifier)

    vmcu_operand_t src;                       ///< source operand (right operand)
    vmcu_operand_t dest;                      ///< destination operand (left operand)

} vmcu_plain_t;

typedef struct vmcu_report {                  ///< report summary of the analyzer pipeline

    int32_t progsize;                         ///< instruction count
    vmcu_plain_t *disassembly;                ///< instruction list

    int32_t nlabels;                          ///< label count
    vmcu_label_t *labels;                     ///< label list

} vmcu_report_t;

/* <---------------------------------- Functions - Analyzer Stage -------------------------------------> */

/*
 * vmcu_analyze_ihex - analyze an intel hex file
 * @hex_file:   intel hex file to analyze
 * */
extern vmcu_report_t* vmcu_analyze_ihex(const char *hex_file);

/*
 * vmcu_report_dtor - destructor of vmcu_report_t
 * @this:   pointer to an instance of vmcu_report_t
 * */
extern void vmcu_report_dtor(vmcu_report_t *this);

/* <-------------------------------- Functions - Disassembler Stage -----------------------------------> */

/*
 * vmcu_disassemble_bytes - disassemble 16/32-bit opcode
 * @bytes:  opcode to disassemble (little endian)
 * @p:      pointer to a single instance of vmcu_plain_t
 * */
extern int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_plain_t *p);

/*
 * vmcu_disassemble_ihex - disassemble an intel hex file
 * @hex_file:   intel hex file to disassemble
 * @size:       size of vmcu_plain_t* after disassembling
 * */
extern vmcu_plain_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size);

/* <--------------------------------- Functions - Decomposer Stage ------------------------------------> */

/*
 * vmcu_decompose_bytes - decompose 16/32-bit opcode
 * @bytes:  opcode to decompose (little endian)
 * @p:      pointer to a single instance of vmcu_plain_t
 * */
extern int vmcu_decompose_bytes(const uint32_t bytes, vmcu_plain_t *p);

/*
 * vmcu_decompose_ihex - decompose an intel hex file
 * @hex_file:   intel hex file to decompose
 * @size:       size of vmcu_plain_t* after decomposing
 * */
extern vmcu_plain_t* vmcu_decompose_ihex(const char *hex_file, int32_t *size);

/* <---------------------------------- Functions - Decoder Stage --------------------------------------> */

/*
 * vmcu_decode_bytes - decode 16/32-bit opcode
 * @bytes:  opcode to decode (little endian)
 * @p:      pointer to a single instance of vmcu_plain_t
 * */
extern int vmcu_decode_bytes(const uint32_t bytes, vmcu_plain_t *p);

/*
 * vmcu_decode_ihex - decode an intel hex file
 * @hex_file:   intel hex file to decode
 * @size:       size of vmcu_plain_t* after decoding
 * */
extern vmcu_plain_t* vmcu_decode_ihex(const char *hex_file, int32_t *size);

#endif
