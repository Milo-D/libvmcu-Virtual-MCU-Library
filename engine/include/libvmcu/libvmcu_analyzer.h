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
 * terms of the GPLv2 license.
 *
 * You should have received a copy of the GPLv2 license with
 * this file. If not, please contact me.
 *
 * Email: David.Milosevic@web.de
 *
 * */

/* <--------------------------------------- Enumeration (arch/) ---------------------------------------> */

typedef enum {                                ///< instruction keys

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

typedef enum {                                ///< special function register id's

    VMCU_SFREGISTER_NONE = -1,                ///< no sfr (used as error code)

    VMCU_RESERVED,                            ///< reserved i/o memory
    VMCU_PINB,                                ///< pinb
    VMCU_DDRB,                                ///< ddrb
    VMCU_PORTB,                               ///< portb
    VMCU_PINC,                                ///< pinc
    VMCU_DDRC,                                ///< ddrc
    VMCU_PORTC,                               ///< portc
    VMCU_PIND,                                ///< pind
    VMCU_DDRD,                                ///< ddrd
    VMCU_PORTD,                               ///< portd
    VMCU_TIFR0,                               ///< tifr0
    VMCU_TIFR1,                               ///< tifr1
    VMCU_TIFR2,                               ///< tifr2
    VMCU_PCIFR,                               ///< pcifr
    VMCU_EIFR,                                ///< eifr
    VMCU_EIMSK,                               ///< eimsk
    VMCU_GPIOR0,                              ///< gpior0
    VMCU_EECR,                                ///< eecr
    VMCU_EEDR,                                ///< eedr
    VMCU_EEARL,                               ///< eearl
    VMCU_EEARH,                               ///< eearh
    VMCU_GTCCR,                               ///< gtccr
    VMCU_TCCR0A,                              ///< tccr0a
    VMCU_TCCR0B,                              ///< tccr0b
    VMCU_TCNT0,                               ///< tcnt0
    VMCU_OCR0A,                               ///< ocr0a
    VMCU_OCR0B,                               ///< ocr0b
    VMCU_GPIOR1,                              ///< gpior1
    VMCU_GPIOR2,                              ///< gpior2
    VMCU_SPCR0,                               ///< spcr0
    VMCU_SPSR0,                               ///< spsr0
    VMCU_SPDR0,                               ///< spdr0
    VMCU_ACSR,                                ///< acsr
    VMCU_DWDR,                                ///< dwdr
    VMCU_SMCR,                                ///< smcr
    VMCU_MCUSR,                               ///< mcusr
    VMCU_MCUCR,                               ///< mcucr
    VMCU_SPMCSR,                              ///< spmcsr
    VMCU_SPL,                                 ///< spl
    VMCU_SPH,                                 ///< sph
    VMCU_SREG,                                ///< sreg
    VMCU_WDTCSR,                              ///< wdtcsr
    VMCU_CLKPR,                               ///< clkpr
    VMCU_PRR,                                 ///< prr
    VMCU_OSCCAL,                              ///< osccal
    VMCU_PCICR,                               ///< pcicr
    VMCU_EICRA,                               ///< eicra
    VMCU_PCMSK0,                              ///< pcmsk0
    VMCU_PCMSK1,                              ///< pcmsk1
    VMCU_PCMSK2,                              ///< pcmsk2
    VMCU_TIMSK0,                              ///< timsk0
    VMCU_TIMSK1,                              ///< timsk1
    VMCU_TIMSK2,                              ///< timsk2
    VMCU_ADCL,                                ///< adcl
    VMCU_ADCH,                                ///< adch
    VMCU_ADCSRA,                              ///< adcsra
    VMCU_ADCSRB,                              ///< adcsrb
    VMCU_ADMUX,                               ///< admux
    VMCU_DIDR0,                               ///< didr0
    VMCU_DIDR1,                               ///< didr1
    VMCU_TCCR1A,                              ///< tccr1a
    VMCU_TCCR1B,                              ///< tccr1b
    VMCU_TCCR1C,                              ///< tccr1c
    VMCU_TCNT1L,                              ///< tcnt1l
    VMCU_TCNT1H,                              ///< tcnt1h
    VMCU_ICR1L,                               ///< icr1l
    VMCU_ICR1H,                               ///< icr1h
    VMCU_OCR1AL,                              ///< ocr1al
    VMCU_OCR1AH,                              ///< ocr1ah
    VMCU_OCR1BL,                              ///< ocr1bl
    VMCU_OCR1BH,                              ///< ocr1bh
    VMCU_TCCR2A,                              ///< tccr2a
    VMCU_TCCR2B,                              ///< tccr2b
    VMCU_TCNT2,                               ///< tcnt2
    VMCU_OCR2A,                               ///< ocr2a
    VMCU_OCR2B,                               ///< ocr2b
    VMCU_ASSR,                                ///< assr
    VMCU_TWBR,                                ///< twbr
    VMCU_TWSR,                                ///< twsr
    VMCU_TWAR,                                ///< twar
    VMCU_TWDR,                                ///< twdr
    VMCU_TWCR,                                ///< twcr
    VMCU_TWAMR,                               ///< twamr
    VMCU_UCSR0A,                              ///< ucsr0a
    VMCU_UCSR0B,                              ///< ucsr0b
    VMCU_UCSR0C,                              ///< ucsr0c
    VMCU_UBRR0L,                              ///< ubbr0l
    VMCU_UBRR0H,                              ///< ubbr0h
    VMCU_UDR0                                 ///< udr0

} VMCU_SFREGISTER;

typedef enum {                                ///< operand types

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

typedef enum {                                ///< instruction groups (see official I.S.M.)

    VMCU_GROUP_NONE = -1,                     ///< no instruction group

    VMCU_GROUP_MATH_LOGIC,                    ///< arithmetic and logic instructions (add, cp, ...)
    VMCU_GROUP_SYS_CTRL,                      ///< mcu control instructions (sleep, wdr, ...)
    VMCU_GROUP_TRANSFER,                      ///< data transfer instructions (lds, st, ...)
    VMCU_GROUP_FLOW,                          ///< controlflow instructions (rjmp, call, ...)
    VMCU_GROUP_BIT,                           ///< bit and bit-test instructions (sbi, sbic, ...)

} VMCU_GROUP;

typedef enum {                                ///< interrupt vectors

    VMCU_VECT_NONE = -1,                      ///< no interrupt vector

    VMCU_VECT_RESET,                          ///< reset vector
    VMCU_VECT_INT0,                           ///< int0 vector
    VMCU_VECT_INT1,                           ///< int1 vector
    VMCU_VECT_PCINT0,                         ///< pcint0 vector
    VMCU_VECT_PCINT1,                         ///< pcint1 vector
    VMCU_VECT_PCINT2,                         ///< pcint2 vector
    VMCU_VECT_WDT,                            ///< wdt vector
    VMCU_VECT_TIMER2_COMPA,                   ///< timer2 compa vector
    VMCU_VECT_TIMER2_COMPB,                   ///< timer2 compb vector
    VMCU_VECT_TIMER2_OVF,                     ///< timer2 ovf vector
    VMCU_VECT_TIMER1_CAPT,                    ///< timer1 capt vector
    VMCU_VECT_TIMER1_COMPA,                   ///< timer1 compa vector
    VMCU_VECT_TIMER1_COMPB,                   ///< timer1 compb vector
    VMCU_VECT_TIMER1_OVF,                     ///< timer1 ovf vector
    VMCU_VECT_TIMER0_COMPA,                   ///< timer0 compa vector
    VMCU_VECT_TIMER0_COMPB,                   ///< timer0 compb vector
    VMCU_VECT_TIMER0_OVF,                     ///< timer0 ovf vector
    VMCU_VECT_SPI_STC,                        ///< spi stc vector
    VMCU_VECT_USART_RX,                       ///< usart rx vector
    VMCU_VECT_USART_UDRE,                     ///< usart udre vector
    VMCU_VECT_USART_TX,                       ///< usart tx vector
    VMCU_VECT_ADC,                            ///< adc vector
    VMCU_VECT_EE_READY,                       ///< ee ready vector
    VMCU_VECT_ANALOG_COMP,                    ///< analog comp vector
    VMCU_VECT_TWI,                            ///< twi vector
    VMCU_VECT_SPM_READY                       ///< spm ready vector

} VMCU_VECT;

typedef enum {                                ///< supported devices for the analyzer

    VMCU_M328P                                ///< ATmega328(P) with AVRe+ device core

} VMCU_DEVICE;

/* <--------------------------------------- Structures (arch/) ----------------------------------------> */

typedef struct vmcu_model vmcu_model_t;       ///< device model (opaque, details not relevant for user)

/* <------------------------------------------- Structures --------------------------------------------> */

typedef struct vmcu_operand {                 ///< operand structure

    int32_t value;                            ///< operand value
    VMCU_OPTYPE type;                         ///< operand type

} vmcu_operand_t;

typedef struct vmcu_instr {                   ///< instruction structure

    struct {                                  ///< instruction core

        VMCU_IKEY key;                        ///< instruction key (instruction identifier)
        VMCU_GROUP group;                     ///< instruction group
    };

    int opcode;                               ///< 16-bit or 32-bit opcode (todo: change type to uint32)
    int addr;

    bool exec;                                ///< instruction executable ?
    bool dword;                               ///< 32-bit instruction ?

    char *mnem;                               ///< disassembled mnemonic of instruction

    vmcu_operand_t src;                       ///< source operand (right operand)
    vmcu_operand_t dest;                      ///< destination operand (left operand)

} vmcu_instr_t;

typedef struct vmcu_xref {                    ///< xref (from, to) structure

    vmcu_instr_t *i;                          ///< cross reference to a single instruction

} vmcu_xref_t;

typedef struct vmcu_sfr {                     ///< sfr (special function register) structure

    VMCU_SFREGISTER id;                       ///< sfr id (for example id = VMCU_UCSR0A)

    int32_t n_xref;                           ///< xref (from) count
    vmcu_xref_t *xref;                        ///< xref (from) list

} vmcu_sfr_t;

typedef struct vmcu_label {                   ///< label structure

    uint16_t id;                              ///< unique label identifier
    uint16_t addr;                            ///< label address

    int32_t n_xref;                           ///< xref (from) count
    vmcu_xref_t *xref;                        ///< xref (from) list

} vmcu_label_t;

typedef struct vmcu_vector {                  ///< interrupt vector structure

    VMCU_VECT id;                             ///< interrupt vector id
    uint16_t addr;                            ///< interrupt vector address

    int32_t n_xto;                            ///< xref (to) count
    vmcu_xref_t *xto;                         ///< xref (to) list

} vmcu_vector_t;

typedef struct vmcu_report {                  ///< report (summary) of the binary

    int32_t progsize;                         ///< instruction count
    vmcu_instr_t *disassembly;                ///< instruction list

    int32_t n_sfr;                            ///< sfr count
    vmcu_sfr_t *sfr;                          ///< sfr list (sorted, ascending)

    int32_t n_label;                          ///< label count
    vmcu_label_t *label;                      ///< label list (sorted, ascending)

    int32_t n_vector;                         ///< vector count
    vmcu_vector_t *vector;                    ///< vector list (sorted, ascending)

} vmcu_report_t;

/* <------------------------------- Functions - Model Loader (arch/) ----------------------------------> */

/*
 * vmcu_model_ctor - constructor of vmcu_model_t
 * @device: device to be loaded (for example VMCU_M328P)
 * */
extern vmcu_model_t* vmcu_model_ctor(const VMCU_DEVICE device);

/*
 * vmcu_model_dtor - destructor of vmcu_model_t
 * @this:   pointer to vmcu_model_t
 * */
extern void vmcu_model_dtor(vmcu_model_t *this);

/* <---------------------------------- Functions - Analyzer Stage -------------------------------------> */

/*
 * vmcu_analyze_ihex - analyze an intel hex file
 * @hex_file:   intel hex file to analyze
 * @mcu:        analyze for this device model
 * */
extern vmcu_report_t* vmcu_analyze_ihex(const char *hex_file, vmcu_model_t *mcu);

/*
 * vmcu_report_dtor - destructor of vmcu_report_t
 * @this:   pointer to an instance of vmcu_report_t
 * */
extern void vmcu_report_dtor(vmcu_report_t *this);

/* <-------------------------------- Functions - Disassembler Stage -----------------------------------> */

/*
 * vmcu_disassemble_bytes - disassemble 16/32-bit opcode
 * @bytes:  opcode to disassemble (little endian)
 * @p:      pointer to a single instance of vmcu_instr_t
 * @mcu:    disassemble for this device model
 * */
extern int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_disassemble_ihex - disassemble an intel hex file
 * @hex_file:   intel hex file to disassemble
 * @size:       size of vmcu_instr_t* after disassembling
 * @mcu:        disassemble for this device model
 * */
extern vmcu_instr_t* vmcu_disassemble_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu);

/* <--------------------------------- Functions - Decomposer Stage ------------------------------------> */

/*
 * vmcu_decompose_bytes - decompose 16/32-bit opcode
 * @bytes:  opcode to decompose (little endian)
 * @p:      pointer to a single instance of vmcu_instr_t
 * @mcu:    decompose for this device model
 * */
extern int vmcu_decompose_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_decompose_ihex - decompose an intel hex file
 * @hex_file:   intel hex file to decompose
 * @size:       size of vmcu_instr_t* after decomposing
 * @mcu:        decompose for this device model
 * */
extern vmcu_instr_t* vmcu_decompose_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu);

/* <---------------------------------- Functions - Decoder Stage --------------------------------------> */

/*
 * vmcu_decode_bytes - decode 16/32-bit opcode
 * @bytes:  opcode to decode (little endian)
 * @p:      pointer to a single instance of vmcu_instr_t
 * @mcu:    decode for this device model
 * */
extern int vmcu_decode_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_decode_ihex - decode an intel hex file
 * @hex_file:   intel hex file to decode
 * @size:       size of vmcu_instr_t* after decoding
 * @mcu:        decode for this device model
 * */
extern vmcu_instr_t* vmcu_decode_ihex(const char *hex_file, int32_t *size, vmcu_model_t *mcu);

#endif
