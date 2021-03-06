/* SFR (Datastructure) Header */

#ifndef VMCU_SFR_H
#define VMCU_SFR_H

// C Headers
#include <inttypes.h>

typedef enum {

    VMCU_SFREGISTER_NONE = -1,

    VMCU_RESERVED,
    VMCU_PINB,
    VMCU_DDRB,
    VMCU_PORTB,
    VMCU_PINC,
    VMCU_DDRC,
    VMCU_PORTC,
    VMCU_PIND,
    VMCU_DDRD,
    VMCU_PORTD,
    VMCU_TIFR0,
    VMCU_TIFR1,
    VMCU_TIFR2,
    VMCU_PCIFR,
    VMCU_EIFR,
    VMCU_EIMSK,
    VMCU_GPIOR0,
    VMCU_EECR,
    VMCU_EEDR,
    VMCU_EEARL,
    VMCU_EEARH,
    VMCU_GTCCR,
    VMCU_TCCR0A,
    VMCU_TCCR0B,
    VMCU_TCNT0,
    VMCU_OCR0A,
    VMCU_OCR0B,
    VMCU_GPIOR1,
    VMCU_GPIOR2,
    VMCU_SPCR0,
    VMCU_SPSR0,
    VMCU_SPDR0,
    VMCU_ACSR,
    VMCU_DWDR,
    VMCU_SMCR,
    VMCU_MCUSR,
    VMCU_MCUCR,
    VMCU_SPMCSR,
    VMCU_SPL,
    VMCU_SPH,
    VMCU_SREG,
    VMCU_WDTCSR,
    VMCU_CLKPR,
    VMCU_PRR,
    VMCU_OSCCAL,
    VMCU_PCICR,
    VMCU_EICRA,
    VMCU_PCMSK0,
    VMCU_PCMSK1,
    VMCU_PCMSK2,
    VMCU_TIMSK0,
    VMCU_TIMSK1,
    VMCU_TIMSK2,
    VMCU_ADCL,
    VMCU_ADCH,
    VMCU_ADCSRA,
    VMCU_ADCSRB,
    VMCU_ADMUX,
    VMCU_DIDR0,
    VMCU_DIDR1,
    VMCU_TCCR1A,
    VMCU_TCCR1B,
    VMCU_TCCR1C,
    VMCU_TCNT1L,
    VMCU_TCNT1H,
    VMCU_ICR1L,
    VMCU_ICR1H,
    VMCU_OCR1AL,
    VMCU_OCR1AH,
    VMCU_OCR1BL,
    VMCU_OCR1BH,
    VMCU_TCCR2A,
    VMCU_TCCR2B,
    VMCU_TCNT2,
    VMCU_OCR2A,
    VMCU_OCR2B,
    VMCU_ASSR,
    VMCU_TWBR,
    VMCU_TWSR,
    VMCU_TWAR,
    VMCU_TWDR,
    VMCU_TWCR,
    VMCU_TWAMR,
    VMCU_UCSR0A,
    VMCU_UCSR0B,
    VMCU_UCSR0C,
    VMCU_UBRR0L,
    VMCU_UBRR0H,
    VMCU_UDR0

} VMCU_SFREGISTER;

typedef struct vmcu_xref vmcu_xref_t;

typedef struct vmcu_sfr {

    VMCU_SFREGISTER id;

    int32_t n_xref;
    vmcu_xref_t *xref;

} vmcu_sfr_t;

#endif
