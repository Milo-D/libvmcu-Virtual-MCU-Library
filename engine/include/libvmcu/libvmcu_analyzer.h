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

typedef enum {                                ///< general purpose registers

    VMCU_REGISTER_NONE = -1,                  ///< no register

    VMCU_REGISTER_R0,                         ///< r0
    VMCU_REGISTER_R1,                         ///< r1
    VMCU_REGISTER_R2,                         ///< r2
    VMCU_REGISTER_R3,                         ///< r3
    VMCU_REGISTER_R4,                         ///< r4
    VMCU_REGISTER_R5,                         ///< r5
    VMCU_REGISTER_R6,                         ///< r6
    VMCU_REGISTER_R7,                         ///< r7
    VMCU_REGISTER_R8,                         ///< r8
    VMCU_REGISTER_R9,                         ///< r9
    VMCU_REGISTER_R10,                        ///< r10
    VMCU_REGISTER_R11,                        ///< r11
    VMCU_REGISTER_R12,                        ///< r12
    VMCU_REGISTER_R13,                        ///< r13
    VMCU_REGISTER_R14,                        ///< r14
    VMCU_REGISTER_R15,                        ///< r15
    VMCU_REGISTER_R16,                        ///< r16
    VMCU_REGISTER_R17,                        ///< r17
    VMCU_REGISTER_R18,                        ///< r18
    VMCU_REGISTER_R19,                        ///< r19
    VMCU_REGISTER_R20,                        ///< r20
    VMCU_REGISTER_R21,                        ///< r21
    VMCU_REGISTER_R22,                        ///< r22
    VMCU_REGISTER_R23,                        ///< r23
    VMCU_REGISTER_R24,                        ///< r24
    VMCU_REGISTER_R25,                        ///< r25
    VMCU_REGISTER_R26,                        ///< r26
    VMCU_REGISTER_R27,                        ///< r27
    VMCU_REGISTER_R28,                        ///< r28
    VMCU_REGISTER_R29,                        ///< r29
    VMCU_REGISTER_R30,                        ///< r30
    VMCU_REGISTER_R31                         ///< r31

} VMCU_REGISTER;

typedef enum {                                ///< instruction keys

    VMCU_IKEY_DATA = -1,                      ///< data (.dw)

    VMCU_IKEY_NOP,                            ///< nop
    VMCU_IKEY_MOVW,                           ///< movw Rd+1:Rd, Rr+1:Rr
    VMCU_IKEY_MUL,                            ///< mul Rd, Rr
    VMCU_IKEY_MULS,                           ///< muls Rd, Rr
    VMCU_IKEY_MULSU,                          ///< mulsu Rd, Rr
    VMCU_IKEY_FMUL,                           ///< fmul Rd, Rr
    VMCU_IKEY_FMULS,                          ///< fmuls Rd, Rr
    VMCU_IKEY_FMULSU,                         ///< fmulsu Rd, Rr
    VMCU_IKEY_LDI,                            ///< ldi Rd, K
    VMCU_IKEY_RJMP,                           ///< rjmp (+/-) K
    VMCU_IKEY_JMP,                            ///< jmp K
    VMCU_IKEY_IJMP,                           ///< ijmp
    VMCU_IKEY_MOV,                            ///< mov Rd, Rr
    VMCU_IKEY_DEC,                            ///< dec Rd
    VMCU_IKEY_INC,                            ///< inc Rd
    VMCU_IKEY_ADD,                            ///< add Rd, Rr
    VMCU_IKEY_ADC,                            ///< adc Rd, Rr
    VMCU_IKEY_ADIW,                           ///< adiw Rd+1:Rd, K
    VMCU_IKEY_SUB,                            ///< sub Rd, Rr
    VMCU_IKEY_SUBI,                           ///< subi Rd, K
    VMCU_IKEY_SBC,                            ///< sbc Rd, Rr
    VMCU_IKEY_SBCI,                           ///< sbci Rd, K
    VMCU_IKEY_SBIW,                           ///< sbiw Rd+1:Rd, K
    VMCU_IKEY_PUSH,                           ///< push Rd
    VMCU_IKEY_POP,                            ///< pop Rd
    VMCU_IKEY_IN,                             ///< in Rd, K
    VMCU_IKEY_OUT,                            ///< out K, Rd
    VMCU_IKEY_SBIS,                           ///< sbis K, b
    VMCU_IKEY_SBIC,                           ///< sbic K, b
    VMCU_IKEY_SBRC,                           ///< sbrc Rd, b
    VMCU_IKEY_SBRS,                           ///< sbrs Rd, b
    VMCU_IKEY_CPSE,                           ///< cpse Rd, Rr
    VMCU_IKEY_EOR,                            ///< eor Rd, Rr
    VMCU_IKEY_LDX,                            ///< ld Rd, X
    VMCU_IKEY_LDXI,                           ///< ld Rd, X+
    VMCU_IKEY_LDDX,                           ///< ld Rd, -X
    VMCU_IKEY_LDY,                            ///< ld Rd, Y
    VMCU_IKEY_LDYI,                           ///< ld Rd, Y+
    VMCU_IKEY_LDDY,                           ///< ld Rd, -Y
    VMCU_IKEY_LDDYQ,                          ///< ldd Rd, Y+q
    VMCU_IKEY_LDDZQ,                          ///< ldd Rd, Z+q
    VMCU_IKEY_LDZ,                            ///< ld Rd, Z
    VMCU_IKEY_LDZI,                           ///< ld Rd, Z+
    VMCU_IKEY_LDDZ,                           ///< ld Rd, -Z
    VMCU_IKEY_STX,                            ///< st X, Rr
    VMCU_IKEY_STXI,                           ///< st X+, Rr
    VMCU_IKEY_STDX,                           ///< st -X, Rr
    VMCU_IKEY_STY,                            ///< st Y, Rr
    VMCU_IKEY_STYI,                           ///< st Y+, Rr
    VMCU_IKEY_STDY,                           ///< st -Y, Rr
    VMCU_IKEY_STDYQ,                          ///< std Y+q, Rr
    VMCU_IKEY_STZ,                            ///< st Z, Rr
    VMCU_IKEY_STZI,                           ///< st Z+, Rr
    VMCU_IKEY_STDZ,                           ///< st -Z, Rr
    VMCU_IKEY_STDZQ,                          ///< std Z+q, Rr
    VMCU_IKEY_STS,                            ///< sts K, Rr
    VMCU_IKEY_STS32,                          ///< sts K, Rr (32-bit)
    VMCU_IKEY_LDS,                            ///< lds Rd, K
    VMCU_IKEY_LDS32,                          ///< lds Rd, K (32-bit)
    VMCU_IKEY_XCH,                            ///< xch Rd, Rr
    VMCU_IKEY_BRNE,                           ///< brne (+/-) K
    VMCU_IKEY_BREQ,                           ///< breq (+/-) K
    VMCU_IKEY_BRGE,                           ///< brge (+/-) K
    VMCU_IKEY_BRPL,                           ///< brpl (+/-) K
    VMCU_IKEY_BRLO,                           ///< brlo (+/-) K
    VMCU_IKEY_BRLT,                           ///< brlt (+/-) K
    VMCU_IKEY_BRCC,                           ///< brcc (+/-) K
    VMCU_IKEY_BRVS,                           ///< brvs (+/-) K
    VMCU_IKEY_BRTS,                           ///< brts (+/-) K
    VMCU_IKEY_BRTC,                           ///< brtc (+/-) K
    VMCU_IKEY_BRMI,                           ///< brmi (+/-) K
    VMCU_IKEY_BRHC,                           ///< brhc (+/-) K
    VMCU_IKEY_BRHS,                           ///< brhs (+/-) K
    VMCU_IKEY_BRID,                           ///< brid (+/-) K
    VMCU_IKEY_BRIE,                           ///< brie (+/-) K
    VMCU_IKEY_BRVC,                           ///< brvc (+/-) K
    VMCU_IKEY_RCALL,                          ///< rcall (+/-) K
    VMCU_IKEY_RET,                            ///< ret
    VMCU_IKEY_RETI,                           ///< reti
    VMCU_IKEY_ICALL,                          ///< icall
    VMCU_IKEY_CALL,                           ///< call K
    VMCU_IKEY_CP,                             ///< cp Rd, Rr
    VMCU_IKEY_CPI,                            ///< cpi Rd, K
    VMCU_IKEY_CPC,                            ///< cpc Rd, Rr
    VMCU_IKEY_LSR,                            ///< lsr Rd
    VMCU_IKEY_ASR,                            ///< asr Rd
    VMCU_IKEY_ROR,                            ///< ror Rd
    VMCU_IKEY_SWAP,                           ///< swap Rd
    VMCU_IKEY_ORI,                            ///< ori Rd, K
    VMCU_IKEY_OR,                             ///< or Rd, Rr
    VMCU_IKEY_AND,                            ///< and Rd, Rr
    VMCU_IKEY_ANDI,                           ///< andi Rd, K
    VMCU_IKEY_LAS,                            ///< las Z, Rr
    VMCU_IKEY_LAC,                            ///< lac Z, Rr
    VMCU_IKEY_LAT,                            ///< lat Z, Rr
    VMCU_IKEY_COM,                            ///< com Rd
    VMCU_IKEY_NEG,                            ///< neg Rd
    VMCU_IKEY_BLD,                            ///< bld Rd, b
    VMCU_IKEY_BST,                            ///< bst Rd, b
    VMCU_IKEY_SBI,                            ///< sbi K, b
    VMCU_IKEY_CBI,                            ///< cbi K, b
    VMCU_IKEY_LPM,                            ///< lpm
    VMCU_IKEY_LPMZ,                           ///< lpm Rd, Z
    VMCU_IKEY_LPMZI,                          ///< lpm Rd, Z+
    VMCU_IKEY_EICALL,                         ///< eicall
    VMCU_IKEY_EIJMP,                          ///< eijmp
    VMCU_IKEY_ELPM,                           ///< elpm
    VMCU_IKEY_ELPMZ,                          ///< elpm Rd, Z
    VMCU_IKEY_ELPMZI,                         ///< elpm Rd, Z+
    VMCU_IKEY_DES,                            ///< des
    VMCU_IKEY_SLEEP,                          ///< sleep
    VMCU_IKEY_WDR,                            ///< wdr
    VMCU_IKEY_BREAK,                          ///< break
    VMCU_IKEY_SPM,                            ///< spm Z, Rr
    VMCU_IKEY_SPMZI,                          ///< spm Z+, Rr
    VMCU_IKEY_SES,                            ///< ses
    VMCU_IKEY_SET,                            ///< set
    VMCU_IKEY_SEV,                            ///< sev
    VMCU_IKEY_SEZ,                            ///< sez
    VMCU_IKEY_SEH,                            ///< seh
    VMCU_IKEY_SEC,                            ///< sec
    VMCU_IKEY_SEI,                            ///< sei
    VMCU_IKEY_SEN,                            ///< sen
    VMCU_IKEY_CLS,                            ///< cls
    VMCU_IKEY_CLT,                            ///< clt
    VMCU_IKEY_CLV,                            ///< clv
    VMCU_IKEY_CLZ,                            ///< clz
    VMCU_IKEY_CLH,                            ///< clh
    VMCU_IKEY_CLC,                            ///< clc
    VMCU_IKEY_CLI,                            ///< cli
    VMCU_IKEY_CLN                             ///< cln

} VMCU_IKEY;

typedef enum {                                ///< special function register id's

    VMCU_SFR_NONE = -1,                       ///< no sfr (used as error code)

    VMCU_SFR_RESERVED,                        ///< reserved i/o memory
    VMCU_SFR_PINB,                            ///< pinb
    VMCU_SFR_DDRB,                            ///< ddrb
    VMCU_SFR_PORTB,                           ///< portb
    VMCU_SFR_PINC,                            ///< pinc
    VMCU_SFR_DDRC,                            ///< ddrc
    VMCU_SFR_PORTC,                           ///< portc
    VMCU_SFR_PIND,                            ///< pind
    VMCU_SFR_DDRD,                            ///< ddrd
    VMCU_SFR_PORTD,                           ///< portd
    VMCU_SFR_TIFR0,                           ///< tifr0
    VMCU_SFR_TIFR1,                           ///< tifr1
    VMCU_SFR_TIFR2,                           ///< tifr2
    VMCU_SFR_PCIFR,                           ///< pcifr
    VMCU_SFR_EIFR,                            ///< eifr
    VMCU_SFR_EIMSK,                           ///< eimsk
    VMCU_SFR_GPIOR0,                          ///< gpior0
    VMCU_SFR_EECR,                            ///< eecr
    VMCU_SFR_EEDR,                            ///< eedr
    VMCU_SFR_EEARL,                           ///< eearl
    VMCU_SFR_EEARH,                           ///< eearh
    VMCU_SFR_GTCCR,                           ///< gtccr
    VMCU_SFR_TCCR0A,                          ///< tccr0a
    VMCU_SFR_TCCR0B,                          ///< tccr0b
    VMCU_SFR_TCNT0,                           ///< tcnt0
    VMCU_SFR_OCR0A,                           ///< ocr0a
    VMCU_SFR_OCR0B,                           ///< ocr0b
    VMCU_SFR_GPIOR1,                          ///< gpior1
    VMCU_SFR_GPIOR2,                          ///< gpior2
    VMCU_SFR_SPCR0,                           ///< spcr0
    VMCU_SFR_SPSR0,                           ///< spsr0
    VMCU_SFR_SPDR0,                           ///< spdr0
    VMCU_SFR_ACSR,                            ///< acsr
    VMCU_SFR_DWDR,                            ///< dwdr
    VMCU_SFR_SMCR,                            ///< smcr
    VMCU_SFR_MCUSR,                           ///< mcusr
    VMCU_SFR_MCUCR,                           ///< mcucr
    VMCU_SFR_SPMCSR,                          ///< spmcsr
    VMCU_SFR_SPL,                             ///< spl
    VMCU_SFR_SPH,                             ///< sph
    VMCU_SFR_SREG,                            ///< sreg
    VMCU_SFR_WDTCSR,                          ///< wdtcsr
    VMCU_SFR_CLKPR,                           ///< clkpr
    VMCU_SFR_PRR,                             ///< prr
    VMCU_SFR_OSCCAL,                          ///< osccal
    VMCU_SFR_PCICR,                           ///< pcicr
    VMCU_SFR_EICRA,                           ///< eicra
    VMCU_SFR_PCMSK0,                          ///< pcmsk0
    VMCU_SFR_PCMSK1,                          ///< pcmsk1
    VMCU_SFR_PCMSK2,                          ///< pcmsk2
    VMCU_SFR_TIMSK0,                          ///< timsk0
    VMCU_SFR_TIMSK1,                          ///< timsk1
    VMCU_SFR_TIMSK2,                          ///< timsk2
    VMCU_SFR_ADCL,                            ///< adcl
    VMCU_SFR_ADCH,                            ///< adch
    VMCU_SFR_ADCSRA,                          ///< adcsra
    VMCU_SFR_ADCSRB,                          ///< adcsrb
    VMCU_SFR_ADMUX,                           ///< admux
    VMCU_SFR_DIDR0,                           ///< didr0
    VMCU_SFR_DIDR1,                           ///< didr1
    VMCU_SFR_TCCR1A,                          ///< tccr1a
    VMCU_SFR_TCCR1B,                          ///< tccr1b
    VMCU_SFR_TCCR1C,                          ///< tccr1c
    VMCU_SFR_TCNT1L,                          ///< tcnt1l
    VMCU_SFR_TCNT1H,                          ///< tcnt1h
    VMCU_SFR_ICR1L,                           ///< icr1l
    VMCU_SFR_ICR1H,                           ///< icr1h
    VMCU_SFR_OCR1AL,                          ///< ocr1al
    VMCU_SFR_OCR1AH,                          ///< ocr1ah
    VMCU_SFR_OCR1BL,                          ///< ocr1bl
    VMCU_SFR_OCR1BH,                          ///< ocr1bh
    VMCU_SFR_TCCR2A,                          ///< tccr2a
    VMCU_SFR_TCCR2B,                          ///< tccr2b
    VMCU_SFR_TCNT2,                           ///< tcnt2
    VMCU_SFR_OCR2A,                           ///< ocr2a
    VMCU_SFR_OCR2B,                           ///< ocr2b
    VMCU_SFR_ASSR,                            ///< assr
    VMCU_SFR_TWBR,                            ///< twbr
    VMCU_SFR_TWSR,                            ///< twsr
    VMCU_SFR_TWAR,                            ///< twar
    VMCU_SFR_TWDR,                            ///< twdr
    VMCU_SFR_TWCR,                            ///< twcr
    VMCU_SFR_TWAMR,                           ///< twamr
    VMCU_SFR_UCSR0A,                          ///< ucsr0a
    VMCU_SFR_UCSR0B,                          ///< ucsr0b
    VMCU_SFR_UCSR0C,                          ///< ucsr0c
    VMCU_SFR_UBRR0L,                          ///< ubbr0l
    VMCU_SFR_UBRR0H,                          ///< ubbr0h
    VMCU_SFR_UDR0                             ///< udr0

} VMCU_SFR;

typedef enum {                                ///< operand types (wikipedia.org/wiki/Atmel_AVR_instruction_set)

    VMCU_OPTYPE_NONE = -1,                    ///< no operand

    VMCU_OPTYPE_R,                            ///< general purpose register
    VMCU_OPTYPE_RP,                           ///< pair of general purpose registers
    VMCU_OPTYPE_X,                            ///< x pointer register (r27:r26)
    VMCU_OPTYPE_Y,                            ///< y pointer register (r29:r28)
    VMCU_OPTYPE_Z,                            ///< z pointer register (r31:r30)
    VMCU_OPTYPE_B,                            ///< bit number in gpr or io register
    VMCU_OPTYPE_K4,                           ///< 4-bit immediate unsigned constant
    VMCU_OPTYPE_K6,                           ///< 6-bit immediate unsigned constant
    VMCU_OPTYPE_K8,                           ///< 8-bit immediate constant, signedness irrelevant
    VMCU_OPTYPE_IO5,                          ///< 5-bit io address
    VMCU_OPTYPE_IO6,                          ///< 6-bit io address
    VMCU_OPTYPE_D7,                           ///< 7-bit data address
    VMCU_OPTYPE_D16,                          ///< 16-bit data address
    VMCU_OPTYPE_P22,                          ///< 22-bit program address
    VMCU_OPTYPE_S7,                           ///< 7-bit signed displacement in units of words
    VMCU_OPTYPE_S12                           ///< 12-bit signed displacement in units of words

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

    VMCU_DEVICE_M328P                         ///< ATmega328(P) with AVRe+ device core

} VMCU_DEVICE;

/* <--------------------------------------- Structures (fwd) ------------------------------------------> */

typedef struct vmcu_model vmcu_model_t;       ///< device model (opaque, details not relevant for user)
typedef struct vmcu_cfg_node vmcu_cfg_node_t; ///< single cfg node (opaque, see below for declaration)

/* <------------------------------------------- Structures --------------------------------------------> */

typedef struct vmcu_registerpair {            ///< registerpair structure (rh:rl)

    VMCU_REGISTER low;                        ///< low byte register
    VMCU_REGISTER high;                       ///< high byte register

} vmcu_registerpair_t;

typedef struct vmcu_access {                  ///< abstract access structure

    /* larger segments */
    unsigned int registers : 1;               ///< accessing registers ?
    unsigned int flash     : 1;               ///< accessing flash ?
    unsigned int stack     : 1;               ///< accessing stack ?
    unsigned int io        : 1;               ///< acccessing io ?
    unsigned int ds        : 1;               ///< accessing data segment ?

    /* special pointers */
    unsigned int sp        : 1;               ///< accessing stack pointer ?
    unsigned int pc        : 1;               ///< accessing program counter ?

    /* status flags */
    unsigned int c_flag    : 1;               ///< accessing carry flag ?
    unsigned int z_flag    : 1;               ///< accessing zero flag ?
    unsigned int n_flag    : 1;               ///< accessing negative flag ?
    unsigned int v_flag    : 1;               ///< accessing overflow flag ?
    unsigned int s_flag    : 1;               ///< accessing sign flag ?
    unsigned int h_flag    : 1;               ///< accessing half-carry flag ?
    unsigned int t_flag    : 1;               ///< accessing t-flag ?
    unsigned int i_flag    : 1;               ///< accessing interrupt flag ?

} vmcu_access_t;

typedef struct vmcu_operand {                 ///< operand structure

    union {                                   ///< operand value union

        uint8_t k;                            ///< set if type = K4, K6 or K8
        uint8_t b;                            ///< set if type = B
        uint8_t io;                           ///< set if type = IO5 or IO6

        uint16_t d;                           ///< set if type = D7 or D16
        uint32_t p;                           ///< set if type = P22
        int16_t  s;                           ///< set if type = S7 or S12

        VMCU_REGISTER r;                      ///< set if type = R
        vmcu_registerpair_t rp;               ///< set if type = RP
    };

    VMCU_OPTYPE type;                         ///< operand type

} vmcu_operand_t;

typedef struct vmcu_mnemonic {                ///< disassembled mnemonic structure

    char base[11];                            ///< mnemonic base string (ldi, sts, etc.)

    char src[9];                              ///< source operand string (r29, 0xff9a, etc.)
    char dest[9];                             ///< destination operand string (r29, 0xff9a, etc.)

    char comment[40];                         ///< comment string (todo: make comments optional)

} vmcu_mnemonic_t;

typedef struct vmcu_instr {                   ///< instruction structure

    struct {                                  ///< members determined by decoder stage

        VMCU_IKEY key;                        ///< instruction key (for example key = VMCU_IKEY_LDI)

        uint32_t opcode;                      ///< instruction opcode
        uint32_t addr;                        ///< instruction address

        bool exec;                            ///< instruction executable/legal ?
        bool dword;                           ///< instruction 32-bit ?
    };

    struct {                                  ///< members determined by annotator stage

        VMCU_GROUP group;                     ///< instruction group (for example group = VMCU_GROUP_FLOW)

        vmcu_access_t writes;                 ///< abstract write access (implicit + explicit)
        vmcu_access_t reads;                  ///< abstract read access (implicit + explicit)
    };

    struct {                                  ///< members determined by decomposer stage

        vmcu_operand_t src;                   ///< instruction's source operand
        vmcu_operand_t dest;                  ///< instructions's destination operand
    };

    struct {                                  ///< members determined by disassembler stage

        vmcu_mnemonic_t mnem;                 ///< disassembled mnemonic
    };

} vmcu_instr_t;

typedef struct vmcu_xref {                    ///< xref (from, to) structure

    vmcu_instr_t *i;                          ///< cross reference to a single instruction

} vmcu_xref_t;

typedef struct vmcu_sfr {                     ///< sfr (special function register) structure

    VMCU_SFR id;                              ///< sfr id (for example id = VMCU_SFR_UCSR0A)

    uint32_t n_xfrom;                         ///< xref (from) count
    vmcu_xref_t *xfrom;                       ///< xref (from) list

} vmcu_sfr_t;

typedef struct vmcu_label {                   ///< label structure

    uint32_t id;                              ///< unique label identifier
    uint32_t addr;                            ///< label address

    uint32_t n_xfrom;                         ///< xref (from) count
    vmcu_xref_t *xfrom;                       ///< xref (from) list

} vmcu_label_t;

typedef struct vmcu_vector {                  ///< interrupt vector structure

    VMCU_VECT id;                             ///< interrupt vector id
    uint32_t addr;                            ///< interrupt vector address

    uint32_t n_xto;                           ///< xref (to) count
    vmcu_xref_t *xto;                         ///< xref (to) list

} vmcu_vector_t;

typedef struct vmcu_string {                  ///< string structure

    uint32_t addr;                            ///< base address of string
    uint64_t length;                          ///< length of string (in bytes)

    char *bytes;                              ///< actual char buffer

} vmcu_string_t;

typedef struct vmcu_cfg_node {                ///< control flow graph node structure

    vmcu_xref_t xto;                          ///< cross-reference to corresponding instruction

    vmcu_cfg_node_t *t;                       ///< 'true' branch of cfg node
    vmcu_cfg_node_t *f;                       ///< 'false' branch of cfg node

} vmcu_cfg_node_t;

typedef struct vmcu_cfg {                     ///< control flow graph

    uint32_t used;                            ///< the number of nodes in use
    vmcu_cfg_node_t *node;                    ///< entry node (node[0] is defined as entry)

} vmcu_cfg_t;

typedef struct vmcu_report {                  ///< report (summary) of the binary

    vmcu_cfg_t *cfg;                          ///< control flow graph

    uint32_t progsize;                        ///< instruction count
    vmcu_instr_t *disassembly;                ///< instruction list

    uint32_t n_sfr;                           ///< sfr count
    vmcu_sfr_t *sfr;                          ///< sfr list (sorted, ascending)

    uint32_t n_label;                         ///< label count
    vmcu_label_t *label;                      ///< label list (sorted, ascending)

    uint32_t n_vector;                        ///< vector count
    vmcu_vector_t *vector;                    ///< vector list (sorted, ascending)

    uint32_t n_string;                        ///< string count
    vmcu_string_t *string;                    ///< string list (sorted, ascending)

} vmcu_report_t;

/* <------------------------------- Functions - Model Loader (arch/) ----------------------------------> */

/*
 * vmcu_model_ctor - constructor of vmcu_model_t
 * @device: device to be loaded (example: VMCU_DEVICE_M328P)
 * */
extern vmcu_model_t* vmcu_model_ctor(const VMCU_DEVICE device);

/*
 * vmcu_model_dtor - destructor of vmcu_model_t
 * @this:   pointer to vmcu_model_t
 * */
extern void vmcu_model_dtor(vmcu_model_t *this);

/* <---------------------------------- Functions - Analyzer Stage -------------------------------------> */

/*
 * vmcu_analyze_file - analyze a binary file
 * @file:   file to analyze (currently srec or ihex)
 * @mcu:    analyze for this device model
 * */
extern vmcu_report_t* vmcu_analyze_file(const char *file, vmcu_model_t *mcu);

/*
 * vmcu_report_dtor - destructor of vmcu_report_t
 * @this:   pointer to an instance of vmcu_report_t
 * */
extern void vmcu_report_dtor(vmcu_report_t *this);

/* <-------------------------------- Functions - Disassembler Stage -----------------------------------> */

/*
 * vmcu_disassemble_bytes - disassemble 16/32-bit opcode
 * @bytes:  opcode to disassemble (little endian)
 * @instr:  pointer to a single instance of vmcu_instr_t
 * @mcu:    disassemble for this device model
 * */
extern int vmcu_disassemble_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_disassemble_file - disassemble a binary file
 * @file:   file to disassemble (currently srec or ihex)
 * @size:   size of vmcu_instr_t* after disassembling
 * @mcu:    disassemble for this device model
 * */
extern vmcu_instr_t* vmcu_disassemble_file(const char *file, uint32_t *size, vmcu_model_t *mcu);

/* <--------------------------------- Functions - Decomposer Stage ------------------------------------> */

/*
 * vmcu_decompose_bytes - decompose 16/32-bit opcode
 * @bytes:  opcode to decompose (little endian)
 * @instr:  pointer to a single instance of vmcu_instr_t
 * @mcu:    decompose for this device model
 * */
extern int vmcu_decompose_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_decompose_file - decompose a binary file
 * @file:   file to decompose (currently srec or ihex)
 * @size:   size of vmcu_instr_t* after decomposing
 * @mcu:    decompose for this device model
 * */
extern vmcu_instr_t* vmcu_decompose_file(const char *file, uint32_t *size, vmcu_model_t *mcu);

/* <--------------------------------- Functions - Annotator Stage -------------------------------------> */

/*
 * vmcu_annotate_bytes - annotate 16/32-bit opcode
 * @bytes:  opcode to annotate (little endian)
 * @instr:  pointer to a single instance of vmcu_instr_t
 * @mcu:    annotate for this device model
 * */
extern int vmcu_annotate_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_annotate_file - annotate a binary file
 * @file:   file to annotate (currently srec or ihex)
 * @size:   size of vmcu_instr_t* after annotating
 * @mcu:    annotate for this device model
 * */
extern vmcu_instr_t* vmcu_annotate_file(const char *file, uint32_t *size, vmcu_model_t *mcu);

/* <---------------------------------- Functions - Decoder Stage --------------------------------------> */

/*
 * vmcu_decode_bytes - decode 16/32-bit opcode
 * @bytes:  opcode to decode (little endian)
 * @instr:  pointer to a single instance of vmcu_instr_t
 * @mcu:    decode for this device model
 * */
extern int vmcu_decode_bytes(const uint32_t bytes, vmcu_instr_t *instr, vmcu_model_t *mcu);

/*
 * vmcu_decode_file - decode a binary file
 * @file:   file to decode (currently srec or ihex)
 * @size:   size of vmcu_instr_t* after decoding
 * @mcu:    decode for this device model
 * */
extern vmcu_instr_t* vmcu_decode_file(const char *file, uint32_t *size, vmcu_model_t *mcu);

#endif
