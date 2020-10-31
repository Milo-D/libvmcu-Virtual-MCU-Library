/* IO Memory Header */

// C Headers
#include <stdlib.h>

// Project Headers
#include "system/core/io.h"
#include "system/core/irq.h"
#include "system/peripherals/timer8.h"
#include "misc/bitmanip.h"

struct _io* io_ctor(int8_t *io_start) {

    struct _io *io;

    if((io = malloc(sizeof(struct _io))) == NULL)
        return NULL;

    io->memory = io_start;

    io->irq = irq_ctor();
    io->timer0 = timer8_ctor(TC0, io->memory);

    return io;
}

void io_dtor(struct _io *this) {

    timer8_dtor(this->timer0);
    irq_dtor(this->irq);

    free(this);
}

void io_update(struct _io *this, const uint64_t dc) {

    if((this->memory[TCCR0B] & CSX_MSK) != 0x00)
        timer8_tick(this->timer0, this->irq, dc);
}

int io_check_irq(const struct _io *this) {

    if(this->irq->size < 1)
        return -1;

    uint16_t isr = 0x0000;
    irq_pop(this->irq, &isr);

    switch(isr) {

        case RST_VECT:        /* not yet implemented */           break;
        case INT0_VECT:       /* not yet implemented */           break;
        case INT1_VECT:       /* not yet implemented */           break;
        case PCINT0_VECT:     /* not yet implemented */           break;
        case PCINT1_VECT:     /* not yet implemented */           break;
        case PCINT2_VECT:     /* not yet implemented */           break;
        case WDT_VECT:        /* not yet implemented */           break;
        case OC2A_VECT:       /* not yet implemented */           break;
        case OC2B_VECT:       /* not yet implemented */           break;
        case OVF2_VECT:       /* not yet implemented */           break;
        case CAPT1_VECT:      /* not yet implemented */           break;
        case OC1A_VECT:       /* not yet implemented */           break;
        case OC1B_VECT:       /* not yet implemented */           break;
        case OVF1_VECT:       /* not yet implemented */           break;
        case OC0A_VECT:       clear(this->memory[TIFR0], OCFA);   break;
        case OC0B_VECT:       clear(this->memory[TIFR0], OCFB);   break;
        case OVF0_VECT:       clear(this->memory[TIFR0], TOV);    break;
        case STC_VECT:        /* not yet implemented */           break;
        case USART_RX_VECT:   /* not yet implemented */           break;
        case USART_UDRE_VECT: /* not yet implemented */           break;
        case USART_TX_VECT:   /* not yet implemented */           break;
        case ADC_VECT:        /* not yet implemented */           break;
        case ERDY_VECT:       /* not yet implemented */           break;
        case ACOMP_VECT:      /* not yet implemented */           break;

        default:              /* should not happen */             break;
    }

    return (int) isr;
}

void io_reboot(const struct _io *this) {

    irq_reboot(this->irq);
    timer8_reboot(this->timer0);
}

/* --- Static --- */

#ifdef ATMEGA328P

static void write_reserved(struct _io *this, const int8_t value) {
    
    /* write access on reserved memory */
    return;
}

static void write_PINB(struct _io *this, const int8_t value) {

    this->memory[0x03] = value;
}

static void write_DDRB(struct _io *this, const int8_t value) {

    this->memory[0x04] = value;
}

static void write_PORTB(struct _io *this, const int8_t value) {

    this->memory[0x05] = value;
}

static void write_PINC(struct _io *this, const int8_t value) {

    this->memory[0x06] = value;
}

static void write_DDRC(struct _io *this, const int8_t value) {

    this->memory[0x07] = value;
}

static void write_PORTC(struct _io *this, const int8_t value) {

    this->memory[0x08] = value;
}

static void write_PIND(struct _io *this, const int8_t value) {

    this->memory[0x09] = value;
}

static void write_DDRD(struct _io *this, const int8_t value) {

    this->memory[0x0a] = value;
}

static void write_PORTD(struct _io *this, const int8_t value) {

    this->memory[0x0b] = value;
}

static void write_TIFR0(struct _io *this, const int8_t value) {

    this->memory[0x15] &= ~value;
}

static void write_TIFR1(struct _io *this, const int8_t value) {

    this->memory[0x16] = value;
}

static void write_TIFR2(struct _io *this, const int8_t value) {

    this->memory[0x17] = value;
}

static void write_PCIFR(struct _io *this, const int8_t value) {

    this->memory[0x1b] = value;
}

static void write_EIFR(struct _io *this, const int8_t value) {

    this->memory[0x1c] = value;
}

static void write_EIMSK(struct _io *this, const int8_t value) {

    this->memory[0x1d] = value;
}

static void write_GPIOR0(struct _io *this, const int8_t value) {

    this->memory[0x1e] = value;
}

static void write_EECR(struct _io *this, const int8_t value) {

    this->memory[0x1f] = value;
}

static void write_EEDR(struct _io *this, const int8_t value) {

    this->memory[0x20] = value;
}

static void write_EEARL(struct _io *this, const int8_t value) {

    this->memory[0x21] = value;
}

static void write_EEARH(struct _io *this, const int8_t value) {

    this->memory[0x22] = value;
}

static void write_GTCCR(struct _io *this, const int8_t value) {

    this->memory[0x23] = value;
}

static void write_TCCR0A(struct _io *this, const int8_t value) {

    this->memory[0x24] = value;
}

static void write_TCCR0B(struct _io *this, const int8_t value) {

    this->memory[0x25] = (value & 0x3f);
    
    if((value & (0x01 << FOC0A)) != 0x00)
        timer8_force_ocpa(this->timer0);
        
    if((value & (0x01 << FOC0B)) != 0x00)
        timer8_force_ocpb(this->timer0);
}

static void write_TCNT0(struct _io *this, const int8_t value) {

    this->memory[0x26] = value;
}

static void write_OCR0A(struct _io *this, const int8_t value) {

    this->memory[0x27] = value;
}

static void write_OCR0B(struct _io *this, const int8_t value) {

    this->memory[0x28] = value;
}

static void write_GPIOR1(struct _io *this, const int8_t value) {

    this->memory[0x2a] = value;
}

static void write_GPIOR2(struct _io *this, const int8_t value) {

    this->memory[0x2b] = value;
}

static void write_SPCR0(struct _io *this, const int8_t value) {

    this->memory[0x2c] = value;
}

static void write_SPSR0(struct _io *this, const int8_t value) {

    this->memory[0x2d] = value;
}

static void write_SPDR0(struct _io *this, const int8_t value) {

    this->memory[0x2e] = value;
}

static void write_ACSR(struct _io *this, const int8_t value) {

    this->memory[0x30] = value;
}

static void write_DWDR(struct _io *this, const int8_t value) {

    this->memory[0x31] = value;
}

static void write_SMCR(struct _io *this, const int8_t value) {

    this->memory[0x33] = value;
}

static void write_MCUSR(struct _io *this, const int8_t value) {

    this->memory[0x34] = value;
}

static void write_MCUCR(struct _io *this, const int8_t value) {

    this->memory[0x35] = value;
}

static void write_SPMCSR(struct _io *this, const int8_t value) {

    this->memory[0x37] = value;
}

static void write_SPL(struct _io *this, const int8_t value) {

    this->memory[0x3d] = value;
}

static void write_SPH(struct _io *this, const int8_t value) {

    this->memory[0x3e] = value;
}

static void write_SREG(struct _io *this, const int8_t value) {

    this->memory[0x3f] = value;
}

static void write_WDTCSR(struct _io *this, const int8_t value) {

    this->memory[0x40] = value;
}

static void write_CLKPR(struct _io *this, const int8_t value) {

    this->memory[0x41] = value;
}

static void write_PRR(struct _io *this, const int8_t value) {

    this->memory[0x44] = value;
}

static void write_OSCCAL(struct _io *this, const int8_t value) {

    this->memory[0x46] = value;
}

static void write_PCICR(struct _io *this, const int8_t value) {

    this->memory[0x48] = value;
}

static void write_EICRA(struct _io *this, const int8_t value) {

    this->memory[0x49] = value;
}

static void write_PCMSK0(struct _io *this, const int8_t value) {

    this->memory[0x4b] = value;
}

static void write_PCMSK1(struct _io *this, const int8_t value) {

    this->memory[0x4c] = value;
}

static void write_PCMSK2(struct _io *this, const int8_t value) {

    this->memory[0x4d] = value;
}

static void write_TIMSK0(struct _io *this, const int8_t value) {

    this->memory[0x4e] = value;
}

static void write_TIMSK1(struct _io *this, const int8_t value) {

    this->memory[0x4f] = value;
}

static void write_TIMSK2(struct _io *this, const int8_t value) {

    this->memory[0x50] = value;
}

static void write_ADCL(struct _io *this, const int8_t value) {

    this->memory[0x58] = value;
}

static void write_ADCH(struct _io *this, const int8_t value) {

    this->memory[0x59] = value;
}

static void write_ADCSRA(struct _io *this, const int8_t value) {

    this->memory[0x5a] = value;
}

static void write_ADCSRB(struct _io *this, const int8_t value) {

    this->memory[0x5b] = value;
}

static void write_ADMUX(struct _io *this, const int8_t value) {

    this->memory[0x5c] = value;
}

static void write_DIDR0(struct _io *this, const int8_t value) {

    this->memory[0x5e] = value;
}

static void write_DIDR1(struct _io *this, const int8_t value) {

    this->memory[0x5f] = value;
}

static void write_TCCR1A(struct _io *this, const int8_t value) {

    this->memory[0x60] = value;
}

static void write_TCCR1B(struct _io *this, const int8_t value) {

    this->memory[0x61] = value;
}

static void write_TCCR1C(struct _io *this, const int8_t value) {

    this->memory[0x62] = value;
}

static void write_TCNT1L(struct _io *this, const int8_t value) {

    this->memory[0x64] = value;
}

static void write_TCNT1H(struct _io *this, const int8_t value) {

    this->memory[0x65] = value;
}

static void write_ICR1L(struct _io *this, const int8_t value) {

    this->memory[0x66] = value;
}

static void write_ICR1H(struct _io *this, const int8_t value) {

    this->memory[0x67] = value;
}

static void write_OCR1AL(struct _io *this, const int8_t value) {

    this->memory[0x68] = value;
}

static void write_OCR1AH(struct _io *this, const int8_t value) {

    this->memory[0x69] = value;
}

static void write_OCR1BL(struct _io *this, const int8_t value) {

    this->memory[0x6a] = value;
}

static void write_OCR1BH(struct _io *this, const int8_t value) {

    this->memory[0x6b] = value;
}

static void write_TCCR2A(struct _io *this, const int8_t value) {

    this->memory[0x90] = value;
}

static void write_TCCR2B(struct _io *this, const int8_t value) {

    this->memory[0x91] = value;
}

static void write_TCNT2(struct _io *this, const int8_t value) {

    this->memory[0x92] = value;
}

static void write_OCR2A(struct _io *this, const int8_t value) {

    this->memory[0x93] = value;
}

static void write_OCR2B(struct _io *this, const int8_t value) {

    this->memory[0x94] = value;
}

static void write_ASSR(struct _io *this, const int8_t value) {

    this->memory[0x96] = value;
}

static void write_TWBR(struct _io *this, const int8_t value) {

    this->memory[0x98] = value;
}

static void write_TWSR(struct _io *this, const int8_t value) {

    this->memory[0x99] = value;
}

static void write_TWAR(struct _io *this, const int8_t value) {

    this->memory[0x9a] = value;
}

static void write_TWDR(struct _io *this, const int8_t value) {

    this->memory[0x9b] = value;
}

static void write_TWCR(struct _io *this, const int8_t value) {

    this->memory[0x9c] = value;
}

static void write_TWAMR(struct _io *this, const int8_t value) {

    this->memory[0x9d] = value;
}

static void write_UCSR0A(struct _io *this, const int8_t value) {

    this->memory[0xa0] = value;
}

static void write_UCSR0B(struct _io *this, const int8_t value) {

    this->memory[0xa1] = value;
}

static void write_UCSR0C(struct _io *this, const int8_t value) {

    this->memory[0xa2] = value;
}

static void write_UBRR0L(struct _io *this, const int8_t value) {

    this->memory[0xa4] = value;
}

static void write_UBRR0H(struct _io *this, const int8_t value) {

    this->memory[0xa5] = value;
}

static void write_UDR0(struct _io *this, const int8_t value) {

    this->memory[0xa6] = value;
}

static int8_t read_reserved(struct _io *this) {
    
    /* read access on reserved memory */
    return 0x00;
}

static int8_t read_PINB(struct _io *this) {

    return this->memory[0x03];
}

static int8_t read_DDRB(struct _io *this) {

    return this->memory[0x04];
}

static int8_t read_PORTB(struct _io *this) {

    return this->memory[0x05];
}

static int8_t read_PINC(struct _io *this) {

    return this->memory[0x06];
}

static int8_t read_DDRC(struct _io *this) {

    return this->memory[0x07];
}

static int8_t read_PORTC(struct _io *this) {

    return this->memory[0x08];
}

static int8_t read_PIND(struct _io *this) {

    return this->memory[0x09];
}

static int8_t read_DDRD(struct _io *this) {

    return this->memory[0x0a];
}

static int8_t read_PORTD(struct _io *this) {

    return this->memory[0x0b];
}

static int8_t read_TIFR0(struct _io *this) {

    return this->memory[0x15];
}

static int8_t read_TIFR1(struct _io *this) {

    return this->memory[0x16];
}

static int8_t read_TIFR2(struct _io *this) {

    return this->memory[0x17];
}

static int8_t read_PCIFR(struct _io *this) {

    return this->memory[0x1b];
}

static int8_t read_EIFR(struct _io *this) {

    return this->memory[0x1c];
}

static int8_t read_EIMSK(struct _io *this) {

    return this->memory[0x1d];
}

static int8_t read_GPIOR0(struct _io *this) {

    return this->memory[0x1e];
}

static int8_t read_EECR(struct _io *this) {

    return this->memory[0x1f];
}

static int8_t read_EEDR(struct _io *this) {

    return this->memory[0x20];
}

static int8_t read_EEARL(struct _io *this) {

    return this->memory[0x21];
}

static int8_t read_EEARH(struct _io *this) {

    return this->memory[0x22];
}

static int8_t read_GTCCR(struct _io *this) {

    return this->memory[0x23];
}

static int8_t read_TCCR0A(struct _io *this) {

    return this->memory[0x24];
}

static int8_t read_TCCR0B(struct _io *this) {

    return this->memory[0x25];
}

static int8_t read_TCNT0(struct _io *this) {

    return this->memory[0x26];
}

static int8_t read_OCR0A(struct _io *this) {

    return this->memory[0x27];
}

static int8_t read_OCR0B(struct _io *this) {

    return this->memory[0x28];
}

static int8_t read_GPIOR1(struct _io *this) {

    return this->memory[0x2a];
}

static int8_t read_GPIOR2(struct _io *this) {

    return this->memory[0x2b];
}

static int8_t read_SPCR0(struct _io *this) {

    return this->memory[0x2c];
}

static int8_t read_SPSR0(struct _io *this) {

    return this->memory[0x2d];
}

static int8_t read_SPDR0(struct _io *this) {

    return this->memory[0x2e];
}

static int8_t read_ACSR(struct _io *this) {

    return this->memory[0x30];
}

static int8_t read_DWDR(struct _io *this) {

    return this->memory[0x31];
}

static int8_t read_SMCR(struct _io *this) {

    return this->memory[0x33];
}

static int8_t read_MCUSR(struct _io *this) {

    return this->memory[0x34];
}

static int8_t read_MCUCR(struct _io *this) {

    return this->memory[0x35];
}

static int8_t read_SPMCSR(struct _io *this) {

    return this->memory[0x37];
}

static int8_t read_SPL(struct _io *this) {

    return this->memory[0x3d];
}

static int8_t read_SPH(struct _io *this) {

    return this->memory[0x3e];
}

static int8_t read_SREG(struct _io *this) {

    return this->memory[0x3f];
}

static int8_t read_WDTCSR(struct _io *this) {

    return this->memory[0x40];
}

static int8_t read_CLKPR(struct _io *this) {

    return this->memory[0x41];
}

static int8_t read_PRR(struct _io *this) {

    return this->memory[0x44];
}

static int8_t read_OSCCAL(struct _io *this) {

    return this->memory[0x46];
}

static int8_t read_PCICR(struct _io *this) {

    return this->memory[0x48];
}

static int8_t read_EICRA(struct _io *this) {

    return this->memory[0x49];
}

static int8_t read_PCMSK0(struct _io *this) {

    return this->memory[0x4b];
}

static int8_t read_PCMSK1(struct _io *this) {

    return this->memory[0x4c];
}

static int8_t read_PCMSK2(struct _io *this) {

    return this->memory[0x4d];
}

static int8_t read_TIMSK0(struct _io *this) {

    return this->memory[0x4e];
}

static int8_t read_TIMSK1(struct _io *this) {

    return this->memory[0x4f];
}

static int8_t read_TIMSK2(struct _io *this) {

    return this->memory[0x50];
}

static int8_t read_ADCL(struct _io *this) {

    return this->memory[0x58];
}

static int8_t read_ADCH(struct _io *this) {

    return this->memory[0x59];
}

static int8_t read_ADCSRA(struct _io *this) {

    return this->memory[0x5a];
}

static int8_t read_ADCSRB(struct _io *this) {

    return this->memory[0x5b];
}

static int8_t read_ADMUX(struct _io *this) {

    return this->memory[0x5c];
}

static int8_t read_DIDR0(struct _io *this) {

    return this->memory[0x5e];
}

static int8_t read_DIDR1(struct _io *this) {

    return this->memory[0x5f];
}

static int8_t read_TCCR1A(struct _io *this) {

    return this->memory[0x60];
}

static int8_t read_TCCR1B(struct _io *this) {

    return this->memory[0x61];
}

static int8_t read_TCCR1C(struct _io *this) {

    return this->memory[0x62];
}

static int8_t read_TCNT1L(struct _io *this) {

    return this->memory[0x64];
}

static int8_t read_TCNT1H(struct _io *this) {

    return this->memory[0x65];
}

static int8_t read_ICR1L(struct _io *this) {

    return this->memory[0x66];
}

static int8_t read_ICR1H(struct _io *this) {

    return this->memory[0x67];
}

static int8_t read_OCR1AL(struct _io *this) {

    return this->memory[0x68];
}

static int8_t read_OCR1AH(struct _io *this) {

    return this->memory[0x69];
}

static int8_t read_OCR1BL(struct _io *this) {

    return this->memory[0x6a];
}

static int8_t read_OCR1BH(struct _io *this) {

    return this->memory[0x6b];
}

static int8_t read_TCCR2A(struct _io *this) {

    return this->memory[0x90];
}

static int8_t read_TCCR2B(struct _io *this) {

    return this->memory[0x91];
}

static int8_t read_TCNT2(struct _io *this) {

    return this->memory[0x92];
}

static int8_t read_OCR2A(struct _io *this) {

    return this->memory[0x93];
}

static int8_t read_OCR2B(struct _io *this) {

    return this->memory[0x94];
}

static int8_t read_ASSR(struct _io *this) {

    return this->memory[0x96];
}

static int8_t read_TWBR(struct _io *this) {

    return this->memory[0x98];
}

static int8_t read_TWSR(struct _io *this) {

    return this->memory[0x99];
}

static int8_t read_TWAR(struct _io *this) {

    return this->memory[0x9a];
}

static int8_t read_TWDR(struct _io *this) {

    return this->memory[0x9b];
}

static int8_t read_TWCR(struct _io *this) {

    return this->memory[0x9c];
}

static int8_t read_TWAMR(struct _io *this) {

    return this->memory[0x9d];
}

static int8_t read_UCSR0A(struct _io *this) {

    return this->memory[0xa0];
}

static int8_t read_UCSR0B(struct _io *this) {

    return this->memory[0xa1];
}

static int8_t read_UCSR0C(struct _io *this) {

    return this->memory[0xa2];
}

static int8_t read_UBRR0L(struct _io *this) {

    return this->memory[0xa4];
}

static int8_t read_UBRR0H(struct _io *this) {

    return this->memory[0xa5];
}

static int8_t read_UDR0(struct _io *this) {

    return this->memory[0xa6];
}

void (*io_write[SFR_SIZE]) (struct _io *this, const int8_t value) = {

    write_reserved,
    write_reserved,
    write_reserved,
    write_PINB,
    write_DDRB,
    write_PORTB,
    write_PINC,
    write_DDRC,
    write_PORTC,
    write_PIND,
    write_DDRD,
    write_PORTD,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_TIFR0,
    write_TIFR1,
    write_TIFR2,
    write_reserved,
    write_reserved,
    write_reserved,
    write_PCIFR,
    write_EIFR,
    write_EIMSK,
    write_GPIOR0,
    write_EECR,
    write_EEDR,
    write_EEARL,
    write_EEARH,
    write_GTCCR,
    write_TCCR0A,
    write_TCCR0B,
    write_TCNT0,
    write_OCR0A,
    write_OCR0B,
    write_reserved,
    write_GPIOR1,
    write_GPIOR2,
    write_SPCR0,
    write_SPSR0,
    write_SPDR0,
    write_reserved,
    write_ACSR,
    write_DWDR,
    write_reserved,
    write_SMCR,
    write_MCUSR,
    write_MCUCR,
    write_reserved,
    write_SPMCSR,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_SPL,
    write_SPH,
    write_SREG,
    write_WDTCSR,
    write_CLKPR,
    write_reserved,
    write_reserved,
    write_PRR,
    write_reserved,
    write_OSCCAL,
    write_reserved,
    write_PCICR,
    write_EICRA,
    write_reserved,
    write_PCMSK0,
    write_PCMSK1,
    write_PCMSK2,
    write_TIMSK0,
    write_TIMSK1,
    write_TIMSK2,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_ADCL,
    write_ADCH,
    write_ADCSRA,
    write_ADCSRB,
    write_ADMUX,
    write_reserved,
    write_DIDR0,
    write_DIDR1,
    write_TCCR1A,
    write_TCCR1B,
    write_TCCR1C,
    write_reserved,
    write_TCNT1L,
    write_TCNT1H,
    write_ICR1L,
    write_ICR1H,
    write_OCR1AL,
    write_OCR1AH,
    write_OCR1BL,
    write_OCR1BH,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_reserved,
    write_TCCR2A,
    write_TCCR2B,
    write_TCNT2,
    write_OCR2A,
    write_OCR2B,
    write_reserved,
    write_ASSR,
    write_reserved,
    write_TWBR,
    write_TWSR,
    write_TWAR,
    write_TWDR,
    write_TWCR,
    write_TWAMR,
    write_reserved,
    write_reserved,
    write_UCSR0A,
    write_UCSR0B,
    write_UCSR0C,
    write_reserved,
    write_UBRR0L,
    write_UBRR0H,
    write_UDR0
};

int8_t (*io_read[SFR_SIZE]) (struct _io *this) = {

    read_reserved,
    read_reserved,
    read_reserved,
    read_PINB,
    read_DDRB,
    read_PORTB,
    read_PINC,
    read_DDRC,
    read_PORTC,
    read_PIND,
    read_DDRD,
    read_PORTD,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_TIFR0,
    read_TIFR1,
    read_TIFR2,
    read_reserved,
    read_reserved,
    read_reserved,
    read_PCIFR,
    read_EIFR,
    read_EIMSK,
    read_GPIOR0,
    read_EECR,
    read_EEDR,
    read_EEARL,
    read_EEARH,
    read_GTCCR,
    read_TCCR0A,
    read_TCCR0B,
    read_TCNT0,
    read_OCR0A,
    read_OCR0B,
    read_reserved,
    read_GPIOR1,
    read_GPIOR2,
    read_SPCR0,
    read_SPSR0,
    read_SPDR0,
    read_reserved,
    read_ACSR,
    read_DWDR,
    read_reserved,
    read_SMCR,
    read_MCUSR,
    read_MCUCR,
    read_reserved,
    read_SPMCSR,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_SPL,
    read_SPH,
    read_SREG,
    read_WDTCSR,
    read_CLKPR,
    read_reserved,
    read_reserved,
    read_PRR,
    read_reserved,
    read_OSCCAL,
    read_reserved,
    read_PCICR,
    read_EICRA,
    read_reserved,
    read_PCMSK0,
    read_PCMSK1,
    read_PCMSK2,
    read_TIMSK0,
    read_TIMSK1,
    read_TIMSK2,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_ADCL,
    read_ADCH,
    read_ADCSRA,
    read_ADCSRB,
    read_ADMUX,
    read_reserved,
    read_DIDR0,
    read_DIDR1,
    read_TCCR1A,
    read_TCCR1B,
    read_TCCR1C,
    read_reserved,
    read_TCNT1L,
    read_TCNT1H,
    read_ICR1L,
    read_ICR1H,
    read_OCR1AL,
    read_OCR1AH,
    read_OCR1BL,
    read_OCR1BH,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_reserved,
    read_TCCR2A,
    read_TCCR2B,
    read_TCNT2,
    read_OCR2A,
    read_OCR2B,
    read_reserved,
    read_ASSR,
    read_reserved,
    read_TWBR,
    read_TWSR,
    read_TWAR,
    read_TWDR,
    read_TWCR,
    read_TWAMR,
    read_reserved,
    read_reserved,
    read_UCSR0A,
    read_UCSR0B,
    read_UCSR0C,
    read_reserved,
    read_UBRR0L,
    read_UBRR0H,
    read_UDR0
};

#endif

