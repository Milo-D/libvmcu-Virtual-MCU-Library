/* MCU specific Definition Implementation */

// Project Headers
#include "system/mcudef.h"

#ifdef ATMEGA32

    const char *flags[SREG_SIZE] = { "CF", "ZF", "NF", "VF", "SF", "HF", "TF", "IF" };

    const char *sfreg[SFR_SIZE]  = {  

        "TWBR",
        "TWSR",
        "TWAR",
        "TWDR",
        "ADCL",
        "ADCH",
        "ADCSRA",
        "ADMUX",
        "ACSR",
        "UBRRL",
        "UCSRB",
        "UCSRA",
        "UDR",
        "SPCR",
        "SPSR",
        "SPDR",
        "PIND",
        "DDRD",
        "PORTD",
        "PINC",
        "DDRC",
        "PORTC",
        "PINB",
        "DDRB",
        "PORTB",
        "PINA",
        "DDRA",
        "PORTA",
        "EECR",
        "EEDR",
        "EEARL",
        "EEARH",
        "UCSRC",
        "WDTCR",
        "ASSR",
        "OCR2",
        "TCNT2",
        "TCCR2",
        "ICR1L",
        "ICR1H",
        "OCR1BL",
        "OCR1BH",
        "OCR1AL",
        "OCR1AH",
        "TCNT1L",
        "TCNT1H",
        "TCCR1B",
        "TCCR1A",
        "SFIOR",
        "OSCCAL",
        "TCNT0",
        "TCCR0",
        "MCUCSR",
        "MCUCR",
        "TWCR",
        "SPMCR",
        "TIFR",
        "TIMSK",
        "GIFR",
        "GICR",
        "OCR0",
        "SPL",
        "SPH",
        "SREG"
    };

#endif

#ifdef ATMEGA16

/* currently not supported */

#endif

#ifdef ATMEGA8

/* currenlty not supported */

#endif

#ifdef ATTINY45

    /* Warning: experimental Architecture */

    const char *flags[SREG_SIZE] = { "CF", "ZF", "NF", "VF", "SF", "HF", "TF", "IF" };

#endif

