/* MCU specific Defintion Header */

#ifndef MCUDEF_H
#define MCUDEF_H

#ifdef ATMEGA32

    #define SYSTEM "ATmega32"

    #define CLOCK 1000000U

    #define GPR_SIZE 32
    #define SREG_SIZE 8
    #define SFR_SIZE 64
    #define SRAM_SIZE 2048
    #define EEPROM_SIZE 1024
    #define FLASH_SIZE 16384    /* 16384 x 16-bit */

    #define SFR_START 0x0020
    #define SFR_END 0x005f

    #define SRAM_START 0x0060
    #define RAM_END 0x085f

    #define SPL 0x005d          /* using absolute SFR address */
    #define SPH 0x005e          /* using absolute SFR address */

    #define PC_BIT 16

    #define IF 7
    #define TF 6
    #define HF 5
    #define SF 4
    #define VF 3
    #define NF 2
    #define ZF 1
    #define CF 0

    #define XL 26
    #define XH 27
    #define YL 28
    #define YH 29
    #define ZL 30
    #define ZH 31

    extern const char *flags[SREG_SIZE];
    extern const char *sfreg[SFR_SIZE];

    /* PIN/PORT A */

    #define PINA 0x0019

    #define PINA0 0
    #define PINA1 1
    #define PINA2 2
    #define PINA3 3
    #define PINA4 4
    #define PINA5 5
    #define PINA6 6
    #define PINA7 7

    #define PORTA 0x001b

    #define PORTA0 0
    #define PORTA1 1
    #define PORTA2 2
    #define PORTA3 3
    #define PORTA4 4
    #define PORTA5 5
    #define PORTA6 6
    #define PORTA7 7

    #define DDRA 0x001a                         

    /* PIN/PORT B */

    #define PINB 0x0016

    #define PINB0 0
    #define PINB1 1
    #define PINB2 2
    #define PINB3 3
    #define PINB4 4
    #define PINB5 5
    #define PINB6 6
    #define PINB7 7

    #define PORTB 0x0018

    #define PORTB0 0
    #define PORTB1 1
    #define PORTB2 2
    #define PORTB3 3
    #define PORTB4 4
    #define PORTB5 5
    #define PORTB6 6
    #define PORTB7 7

    #define DDRB 0x0017

    /* PIN/PORT C */

    #define PINC 0x0013

    #define PINC0 0
    #define PINC1 1
    #define PINC2 2
    #define PINC3 3
    #define PINC4 4
    #define PINC5 5
    #define PINC6 6
    #define PINC7 7

    #define PORTC 0x0015

    #define PORTC0 0
    #define PORTC1 1
    #define PORTC2 2
    #define PORTC3 3
    #define PORTC4 4
    #define PORTC5 5
    #define PORTC6 6
    #define PORTC7 7

    #define DDRC 0x0014

    /* PIN/PORT D */

    #define PIND 0x0010

    #define PIND0 0
    #define PIND1 1
    #define PIND2 2
    #define PIND3 3
    #define PIND4 4
    #define PIND5 5
    #define PIND6 6
    #define PIND7 7

    #define PORTD 0x0012

    #define PORTD0 0
    #define PORTD1 1
    #define PORTD2 2
    #define PORTD3 3
    #define PORTD4 4
    #define PORTD5 5
    #define PORTD6 6
    #define PORTD7 7

    #define DDRD 0x0011

    /* Timer (General) */

    #define TIMSK 0x0039
    #define TIFR  0x0038

    #define CSX_MSK 0x07

    #define NMODT8 4
    #define NMODT16 16

    /* Timer0 (8-bit) */

    #define TCNT0 0x0032
    #define TCCR0 0x0033
    #define OCR0  0x003C

    #define TOV0 0
    #define OCF0 1

    #define WGM00 6
    #define WGM01 3
    #define COM00 4
    #define COM01 5
    #define FOC0  7

    #define OC0DDR DDRB
    #define OC0P PORTB
    #define OC0 3

    #define WGM00_MSK (0x01 << WGM00)
    #define WGM01_MSK (0x01 << WGM01)

    #define COM00_MSK (0x01 << COM00)
    #define COM01_MSK (0x01 << COM01)

    /* Timer2 (8-bit) */

    #define TCNT2 0x0024
    #define TCCR2 0x0025
    #define OCR2  0x0023

    #define TOV2 6
    #define OCF2 7

    #define WGM20 6
    #define WGM21 3
    #define COM20 4
    #define COM21 5
    #define FOC2  7

    #define OC2DDR DDRD
    #define OC2P PORTD 
    #define OC2 7

    #define WGM20_MSK (0x01 << WGM20)
    #define WGM21_MSK (0x01 << WGM21)

    #define COM20_MSK (0x01 << COM20)
    #define COM21_MSK (0x01 << COM21)

    /* Timer0 and Timer2 */

    #define wgmtc8(tccr) ((tccr & WGM00_MSK) >> WGM00 - 1) | \
                         ((tccr & WGM01_MSK) >> WGM01)       \

    #define comtc8(tccr) ((tccr & COM01_MSK) >> COM01 - 1) | \
                         ((tccr & COM00_MSK) >> COM00)       \

    /* ISR Vector Table */

    #define NVECT 21

    #define RST_VECT  0x0000
    #define INT0_VECT 0x0002
    #define INT1_VECT 0x0004
    #define INT2_VECT 0x0006
    #define OC2_VECT  0x0008
    #define OVF2_VECT 0x000a
    #define ICP1_VECT 0x000c
    #define OC1A_VECT 0x000e
    #define OC1B_VECT 0x0010
    #define OVF1_VECT 0x0012
    #define OC0_VECT  0x0014
    #define OVF0_VECT 0x0016
    #define SPI_VECT  0x0018
    #define URXC_VECT 0x001a
    #define UDRE_VECT 0x001c
    #define UTXC_VECT 0x001e
    #define ADCC_VECT 0x0020
    #define ERDY_VECT 0x0022
    #define ACI_VECT  0x0024
    #define TWI_VECT  0x0026
    #define SPMR_VECT 0x0028

#endif

#ifdef ATMEGA16

/* currently not supported */

#endif

#ifdef ATMEGA8

/* currently not supported */

#endif

#ifdef ATTINY45

    /* Warning: experimental Architecture */

    #define SYSTEM "ATtiny45"

    #define CLOCK 1000000U

    #define GPR_SIZE 32
    #define SREG_SIZE 8
    #define SFR_SIZE 64
    #define SRAM_SIZE 256
    #define EEPROM_SIZE 256
    #define FLASH_SIZE 2048
    #define SRAM_START 0x0060
    #define RAM_END 0x015f

    #define SPL 0x005d
    #define SPH 0x005e

    #define PC_BIT 16

    #define IF 7
    #define TF 6
    #define HF 5
    #define SF 4
    #define VF 3
    #define NF 2
    #define ZF 1
    #define CF 0

    #define XL 26
    #define XH 27
    #define YL 28
    #define YH 29
    #define ZL 30
    #define ZH 31

    extern const char *flags[SREG_SIZE];

#endif

#endif
