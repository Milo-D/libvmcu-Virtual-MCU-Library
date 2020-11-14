/* MCU specific Defintion Header */

#ifndef MCUDEF_H
#define MCUDEF_H

#ifdef ATMEGA328P

    #define SYSTEM "ATmega328(P)"

    #define CLOCK 1000000U      /* Default CPU Clock [Hz]*/

    #define GPR_SIZE 32
    #define SREG_SIZE 8
    #define SRAM_SIZE 2048
    #define EEPROM_SIZE 1024
    #define FLASH_SIZE 16384    /* 16384 x 16-bit */

    #define SFR_SIZE 167
    #define SFR_START 0x0020
    #define SFR_END 0x00c6
    
    #define SFRL_SIZE 32
    #define SFRL_START SFR_START
    #define SFRL_END 0x003f
    
    #define RAM_END 0x08ff

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

    /* PIN/PORT B */

    #define PINB 0x0003

    #define PINB0 0
    #define PINB1 1
    #define PINB2 2
    #define PINB3 3
    #define PINB4 4
    #define PINB5 5
    #define PINB6 6
    #define PINB7 7

    #define PORTB 0x0005

    #define PORTB0 0
    #define PORTB1 1
    #define PORTB2 2
    #define PORTB3 3
    #define PORTB4 4
    #define PORTB5 5
    #define PORTB6 6
    #define PORTB7 7

    #define DDRB 0x0004

    /* PIN/PORT C */

    #define PINC 0x0006

    #define PINC0 0
    #define PINC1 1
    #define PINC2 2
    #define PINC3 3
    #define PINC4 4
    #define PINC5 5
    #define PINC6 6

    #define PORTC 0x0008

    #define PORTC0 0
    #define PORTC1 1
    #define PORTC2 2
    #define PORTC3 3
    #define PORTC4 4
    #define PORTC5 5
    #define PORTC6 6

    #define DDRC 0x0004

    /* PIN/PORT D */

    #define PIND 0x0009

    #define PIND0 0
    #define PIND1 1
    #define PIND2 2
    #define PIND3 3
    #define PIND4 4
    #define PIND5 5
    #define PIND6 6
    #define PIND7 7

    #define PORTD 0x000b

    #define PORTD0 0
    #define PORTD1 1
    #define PORTD2 2
    #define PORTD3 3
    #define PORTD4 4
    #define PORTD5 5
    #define PORTD6 6
    #define PORTD7 7

    #define DDRD 0x000a
    
    /* SPM Control Register */
    
    #define SPMCSR 0x0037
    
    #define SPMIE  7
    #define RWWSB  6
    #define SIGRD  5
    #define RWWSRE 4
    #define BLBSET 3
    #define PGWRT  2
    #define PGERS  1
    #define SPMEN  0
    
    /* EEPROM (General) */
    
    #define EEP_CLK 8000000         /* Calibrated RC Oscillator [Hz] */
    
    #define EEEW_CYCLES 26368
    #define EEE_CYCLES  13960       /* calculated */
    #define EEW_CYCLES  13960       /* calculated */
    
    /* EEPROM Control Register */
    
    #define EECR 0x1f
    
    #define EEPM1 5
    #define EEPM0 4
    #define EERIE 3
    #define EEMPE 2
    #define EEPE  1
    #define EERE  0
    
    #define EEPM0_MSK (0x01 << EEPM0)
    #define EEPM1_MSK (0x01 << EEPM1)
    
    #define eep_mode(eecr) ((eecr & EEPM1_MSK) >> (EEPM1 - 1)) | \
                           ((eecr & EEPM0_MSK) >> EEPM0)         \
    
    /* EEPROM Data and Address Register */
    
    #define EEDR 0x20
    #define EEARL 0x21
    #define EEARH 0x22

    /* Timer (General) */

    #define CSX_MSK 0x07

    #define NMODT8 8
    #define NMODT16 16

    #define TOV 0
    #define OCFA 1
    #define OCFB 2
    
    #define TOIE 0
    #define OCIEA 1
    #define OCIEB 2

    /* Timer0 (8-bit) */

    #define TIMSK0 0x4e
    #define TIFR0  0x15

    #define TCNT0 0x0026
    
    #define TCCR0A 0x0024
    #define TCCR0B 0x0025
    
    #define OCR0A  0x0027
    #define OCR0B  0x0028

    #define WGM00 0
    #define WGM01 1
    #define WGM02 3
    
    #define COM0A0 6
    #define COM0A1 7
    
    #define COM0B0 4
    #define COM0B1 5
    
    #define FOC0A 7
    #define FOC0B 6

    #define OC0A_DDR DDRD
    #define OC0A_P PORTD
    #define OC0A 6
    
    #define OC0B_DDR DDRD
    #define OC0B_P PORTD
    #define OC0B 5

    #define WGM00_MSK (0x01 << WGM00)
    #define WGM01_MSK (0x01 << WGM01)
    #define WGM02_MSK (0x01 << WGM02)

    #define COM0A0_MSK (0x01 << COM0A0)
    #define COM0A1_MSK (0x01 << COM0A1)
    
    #define COM0B0_MSK (0x01 << COM0B0)
    #define COM0B1_MSK (0x01 << COM0B1)

    /* Timer2 (8-bit) */

    #define TIMSK2 0x0050
    #define TIFR2 0x0017

    #define TCNT2 0x0092
    
    #define TCCR2A 0x0090
    #define TCCR2B 0x0091
    
    #define OCR2A  0x0093
    #define OCR2B  0x0094

    #define WGM20 0
    #define WGM21 1
    #define WGM22 3
    
    #define COM2A0 6
    #define COM2A1 7
    
    #define COM2B0 4
    #define COM2B1 5
    
    #define FOC2A 7
    #define FOC2B 6 
    
    #define OC2A_DDR DDRB
    #define OC2A_P PORTB
    #define OC2A 3
    
    #define OC2B_DDR DDRD
    #define OC2B_P PORTD
    #define OC2B 3

    #define WGM20_MSK (0x01 << WGM20)
    #define WGM21_MSK (0x01 << WGM21)
    #define WGM22_MSK (0x01 << WGM22)

    #define COM2A0_MSK (0x01 << COM2A0)
    #define COM2A1_MSK (0x01 << COM2A1)

    /* Timer0 and Timer2 */

    #define wgmtc8(tccra, tccrb) ((tccra & WGM00_MSK) >> WGM00)                     | \
                                 ((tccra & WGM01_MSK) | ((tccrb & WGM02_MSK) >> 1))   \
                           
                           
    #define comtc8a(tccra) ((tccra & COM0A1_MSK) >> (COM0A1 - 1)) | \
                           ((tccra & COM0A0_MSK) >> COM0A0)         \
    
    #define comtc8b(tccra) ((tccra & COM0B1_MSK) >> (COM0B1 - 1)) | \
                           ((tccra & COM0B0_MSK) >> COM0B0)         \

    /* ISR Vector Table */

    #define NVECT 24

    #define RST_VECT        0x0000
    #define INT0_VECT       0x0002
    #define INT1_VECT       0x0004
    #define PCINT0_VECT     0x0006
    #define PCINT1_VECT     0x0008
    #define PCINT2_VECT     0x000a
    #define WDT_VECT        0x000c
    #define OC2A_VECT       0x000e
    #define OC2B_VECT       0x0010
    #define OVF2_VECT       0x0012
    #define CAPT1_VECT      0x0014
    #define OC1A_VECT       0x0016
    #define OC1B_VECT       0x0018
    #define OVF1_VECT       0x001a
    #define OC0A_VECT       0x001c
    #define OC0B_VECT       0x001e
    #define OVF0_VECT       0x0020
    #define STC_VECT        0x0022
    #define USART_RX_VECT   0x0024
    #define USART_UDRE_VECT 0x0026
    #define USART_TX_VECT   0x0028
    #define ADC_VECT        0x002a
    #define ERDY_VECT       0x002c
    #define ACOMP_VECT      0x002e

#endif

#ifdef ATMEGA16

/* currently not supported */

#endif

#ifdef ATMEGA8

/* currently not supported */

#endif

#endif
