/* SFR Implementation */

// Project Headers (engine)
#include "engine/include/system/core/sfr.h"
#include "engine/include/system/core/io.h"
#include "engine/include/system/core/irq.h"
#include "engine/include/system/peripherals/timer8.h"
#include "engine/include/system/peripherals/eeprom.h"
#include "engine/include/system/mcudef.h"

// Project Headers (shared)
#include "shared/include/misc/bitmanip.h"

/*
 * This file belongs to the IO Module but is
 * quite long so I decided to create a seperate
 * file for these functions.
 * 
 * This file will be automatically included by
 * including 'io.h'.
 * 
 **/

#ifdef ATMEGA328P

static void set_reserved(io_t *io, const int bit) {
    
    /* set access on reserved memory */
    return;
}

static void set_PINB(io_t *io, const int bit) {

    setbit(io->memory[0x03], bit);
}

static void set_DDRB(io_t *io, const int bit) {

    setbit(io->memory[0x04], bit);
}

static void set_PORTB(io_t *io, const int bit) {

    setbit(io->memory[0x05], bit);
}

static void set_PINC(io_t *io, const int bit) {

    setbit(io->memory[0x06], bit);
}

static void set_DDRC(io_t *io, const int bit) {

    setbit(io->memory[0x07], bit);
}

static void set_PORTC(io_t *io, const int bit) {

    setbit(io->memory[0x08], bit);
}

static void set_PIND(io_t *io, const int bit) {

    setbit(io->memory[0x09], bit);
}

static void set_DDRD(io_t *io, const int bit) {

    setbit(io->memory[0x0a], bit);
}

static void set_PORTD(io_t *io, const int bit) {

    setbit(io->memory[0x0b], bit);
}

static void set_TIFR0(io_t *io, const int bit) {

    switch(bit) {

        case 3: case 4: case 5:
        case 6: case 7: return;

        default: break;
    }

    if(bit(io->memory[TIFR0], bit) == 0x00)
        return;

    const uint16_t vector[3] = { 

        OVF0_VECT, 
        OC0A_VECT,
        OC0B_VECT 
    };
    
    clearbit(io->memory[TIFR0], bit);
    irq_disable(io->irq, vector[bit]);
}

static void set_TIFR1(io_t *io, const int bit) {

    setbit(io->memory[0x16], bit);
}

static void set_TIFR2(io_t *io, const int bit) {

    setbit(io->memory[0x17], bit);
}

static void set_PCIFR(io_t *io, const int bit) {

    setbit(io->memory[0x1b], bit);
}

static void set_EIFR(io_t *io, const int bit) {

    setbit(io->memory[0x1c], bit);
}

static void set_EIMSK(io_t *io, const int bit) {

    setbit(io->memory[0x1d], bit);
}

static void set_GPIOR0(io_t *io, const int bit) {

    setbit(io->memory[0x1e], bit);
}

static void set_EECR(io_t *io, const int bit) {
    
    const uint8_t eecr = io->memory[EECR];
    
    switch(bit) {
        
        case EERE:
        
            if(bit(eecr, EEPE) == 0x00)
                eeprom_try_read(io->eeprom);
        
        break;

        case EEPE:

            if(bit(eecr, EEPE) == 0x01)
                return;
                
            setbit(io->memory[EECR], EEPE);

            if(eeprom_try_write(io->eeprom) == 0)
                irq_disable(io->irq, ERDY_VECT);
        
        break;
        
        case EEMPE:
        
            if(bit(eecr, EEMPE) == 0x01)
                return;
            
            setbit(io->memory[EECR], EEMPE);
            eeprom_enable_write(io->eeprom);
        
        break;
        
        case EERIE:
        
            setbit(io->memory[EECR], EERIE);
        
        break;
        
        case EEPM0: case EEPM1:
        
            if(bit(eecr, EEPE) == 0x00)
                setbit(io->memory[EECR], bit);

        break;

        default: break;
    }
}

static void clear_reserved(io_t *io, const int bit) {
    
    /* clear access on reserved memory */
    return;
}

static void clear_PINB(io_t *io, const int bit) {

    clearbit(io->memory[0x03], bit);
}

static void clear_DDRB(io_t *io, const int bit) {

    clearbit(io->memory[0x04], bit);
}

static void clear_PORTB(io_t *io, const int bit) {

    clearbit(io->memory[0x05], bit);
}

static void clear_PINC(io_t *io, const int bit) {

    clearbit(io->memory[0x06], bit);
}

static void clear_DDRC(io_t *io, const int bit) {

    clearbit(io->memory[0x07], bit);
}

static void clear_PORTC(io_t *io, const int bit) {

    clearbit(io->memory[0x08], bit);
}

static void clear_PIND(io_t *io, const int bit) {

    clearbit(io->memory[0x09], bit);
}

static void clear_DDRD(io_t *io, const int bit) {

    clearbit(io->memory[0x0a], bit);
}

static void clear_PORTD(io_t *io, const int bit) {

    clearbit(io->memory[0x0b], bit);
}

static void clear_TIFR0(io_t *io, const int bit) {

    /* TIFR0 can not be cleared this way */
    return;
}

static void clear_TIFR1(io_t *io, const int bit) {

    clearbit(io->memory[0x16], bit);
}

static void clear_TIFR2(io_t *io, const int bit) {

    clearbit(io->memory[0x17], bit);
}

static void clear_PCIFR(io_t *io, const int bit) {

    clearbit(io->memory[0x1b], bit);
}

static void clear_EIFR(io_t *io, const int bit) {

    clearbit(io->memory[0x1c], bit);
}

static void clear_EIMSK(io_t *io, const int bit) {

    clearbit(io->memory[0x1d], bit);
}

static void clear_GPIOR0(io_t *io, const int bit) {

    clearbit(io->memory[0x1e], bit);
}

static void clear_EECR(io_t *io, const int bit) {

    const uint8_t eecr = io->memory[EECR];

    switch(bit) {
        
        case EEMPE: clearbit(io->memory[EECR], EEMPE); break;
        
        case EERIE:
        
            if(bit(eecr, EERIE) == 0x01)
                irq_disable(io->irq, ERDY_VECT);

            clearbit(io->memory[EECR], EERIE);
        
        break;
        
        case EEPM0: case EEPM1:
        
            if(bit(eecr, EEPE) == 0x00)
                clearbit(io->memory[EECR], bit);
        
        break;
    }
}

static void write_reserved(io_t *io, const int8_t value) {

    /* write access on reserved memory */
    return;
}

static void write_PINB(io_t *io, const int8_t value) {

    io->memory[0x03] = value;
}

static void write_DDRB(io_t *io, const int8_t value) {

    io->memory[0x04] = value;
}

static void write_PORTB(io_t *io, const int8_t value) {

    io->memory[0x05] = value;
}

static void write_PINC(io_t *io, const int8_t value) {

    io->memory[0x06] = value;
}

static void write_DDRC(io_t *io, const int8_t value) {

    io->memory[0x07] = value;
}

static void write_PORTC(io_t *io, const int8_t value) {

    io->memory[0x08] = value;
}

static void write_PIND(io_t *io, const int8_t value) {

    io->memory[0x09] = value;
}

static void write_DDRD(io_t *io, const int8_t value) {

    io->memory[0x0a] = value;
}

static void write_PORTD(io_t *io, const int8_t value) {

    io->memory[0x0b] = value;
}

static void write_TIFR0(io_t *io, const int8_t value) {

    for(int i = TOV; i <= OCFB; i++) {

        if(bit(value, i) == 0x01)
            set_TIFR0(io, i);
    }

    io->memory[TIFR0] &= ~value;
}

static void write_TIFR1(io_t *io, const int8_t value) {

    io->memory[0x16] = value;
}

static void write_TIFR2(io_t *io, const int8_t value) {

    io->memory[0x17] = value;
}

static void write_PCIFR(io_t *io, const int8_t value) {

    io->memory[0x1b] = value;
}

static void write_EIFR(io_t *io, const int8_t value) {

    io->memory[0x1c] = value;
}

static void write_EIMSK(io_t *io, const int8_t value) {

    io->memory[0x1d] = value;
}

static void write_GPIOR0(io_t *io, const int8_t value) {

    io->memory[0x1e] = value;
}

static void write_EECR(io_t *io, const int8_t value) {

    void (*bitop[2]) (io_t *io, const int bit) = { clear_EECR, set_EECR };

    for(int i = EERE; i <= EEPM1; i++)
        (*bitop[ bit(value, i) ])(io, i);
}

static void write_EEDR(io_t *io, const int8_t value) {

    io->memory[EEDR] = value;
}

static void write_EEARL(io_t *io, const int8_t value) {

    if(bit(io->memory[EECR], EEPE) == 0x01)
        return;

    io->memory[EEARL] = value;
}

static void write_EEARH(io_t *io, const int8_t value) {

    if(bit(io->memory[EECR], EEPE) == 0x01)
        return;

    io->memory[EEARH] = value;
}

static void write_GTCCR(io_t *io, const int8_t value) {

    io->memory[0x23] = value;
}

static void write_TCCR0A(io_t *io, const int8_t value) {

    io->memory[0x24] = value;
}

static void write_TCCR0B(io_t *io, const int8_t value) {

    io->memory[TCCR0B] = (value & 0x3f);

    if(bit(value, FOC0A) == 0x01)
        timer8_force_ocpa(io->timer0);
        
    if(bit(value, FOC0B) == 0x01)
        timer8_force_ocpb(io->timer0);

    timer8_update_prescaler(io->timer0);
}

static void write_TCNT0(io_t *io, const int8_t value) {

    io->memory[0x26] = value;
}

static void write_OCR0A(io_t *io, const int8_t value) {

    io->memory[0x27] = value;
}

static void write_OCR0B(io_t *io, const int8_t value) {

    io->memory[0x28] = value;
}

static void write_GPIOR1(io_t *io, const int8_t value) {

    io->memory[0x2a] = value;
}

static void write_GPIOR2(io_t *io, const int8_t value) {

    io->memory[0x2b] = value;
}

static void write_SPCR0(io_t *io, const int8_t value) {

    io->memory[0x2c] = value;
}

static void write_SPSR0(io_t *io, const int8_t value) {

    io->memory[0x2d] = value;
}

static void write_SPDR0(io_t *io, const int8_t value) {

    io->memory[0x2e] = value;
}

static void write_ACSR(io_t *io, const int8_t value) {

    io->memory[0x30] = value;
}

static void write_DWDR(io_t *io, const int8_t value) {

    io->memory[0x31] = value;
}

static void write_SMCR(io_t *io, const int8_t value) {

    io->memory[0x33] = value;
}

static void write_MCUSR(io_t *io, const int8_t value) {

    io->memory[0x34] = value;
}

static void write_MCUCR(io_t *io, const int8_t value) {

    io->memory[0x35] = value;
}

static void write_SPMCSR(io_t *io, const int8_t value) {

    io->memory[0x37] = value;
}

static void write_SPL(io_t *io, const int8_t value) {

    io->memory[0x3d] = value;
}

static void write_SPH(io_t *io, const int8_t value) {

    io->memory[0x3e] = value;
}

static void write_SREG(io_t *io, const int8_t value) {

    io->memory[0x3f] = value;
}

static void write_WDTCSR(io_t *io, const int8_t value) {

    io->memory[0x40] = value;
}

static void write_CLKPR(io_t *io, const int8_t value) {

    io->memory[0x41] = value;
}

static void write_PRR(io_t *io, const int8_t value) {

    io->memory[0x44] = value;
}

static void write_OSCCAL(io_t *io, const int8_t value) {

    io->memory[0x46] = value;
}

static void write_PCICR(io_t *io, const int8_t value) {

    io->memory[0x48] = value;
}

static void write_EICRA(io_t *io, const int8_t value) {

    io->memory[0x49] = value;
}

static void write_PCMSK0(io_t *io, const int8_t value) {

    io->memory[0x4b] = value;
}

static void write_PCMSK1(io_t *io, const int8_t value) {

    io->memory[0x4c] = value;
}

static void write_PCMSK2(io_t *io, const int8_t value) {

    io->memory[0x4d] = value;
}

static void write_TIMSK0(io_t *io, const int8_t value) {

    const uint8_t timsk = io->memory[TIMSK0];
    const uint8_t tifr = io->memory[TIFR0];

    const uint16_t vector[3] = { 
        
        OVF0_VECT, 
        OC0A_VECT, 
        OC0B_VECT 
    };

    for(int i = TOIE; i <= OCIEB; i++) {

        const uint8_t bv = bit(value, i);

        if(bv == 0x00 && bit(timsk, i) == 0x01) {

            irq_disable(io->irq, vector[i]);
            continue;
        }
        
        if(bv == 0x01 && bit(tifr, i) == 0x01)
            irq_enable(io->irq, vector[i]);
    }

    io->memory[TIMSK0] = (value & 0x07);
}

static void write_TIMSK1(io_t *io, const int8_t value) {

    io->memory[0x4f] = value;
}

static void write_TIMSK2(io_t *io, const int8_t value) {

    io->memory[0x50] = value;
}

static void write_ADCL(io_t *io, const int8_t value) {

    io->memory[0x58] = value;
}

static void write_ADCH(io_t *io, const int8_t value) {

    io->memory[0x59] = value;
}

static void write_ADCSRA(io_t *io, const int8_t value) {

    io->memory[0x5a] = value;
}

static void write_ADCSRB(io_t *io, const int8_t value) {

    io->memory[0x5b] = value;
}

static void write_ADMUX(io_t *io, const int8_t value) {

    io->memory[0x5c] = value;
}

static void write_DIDR0(io_t *io, const int8_t value) {

    io->memory[0x5e] = value;
}

static void write_DIDR1(io_t *io, const int8_t value) {

    io->memory[0x5f] = value;
}

static void write_TCCR1A(io_t *io, const int8_t value) {

    io->memory[0x60] = value;
}

static void write_TCCR1B(io_t *io, const int8_t value) {

    io->memory[0x61] = value;
}

static void write_TCCR1C(io_t *io, const int8_t value) {

    io->memory[0x62] = value;
}

static void write_TCNT1L(io_t *io, const int8_t value) {

    io->memory[0x64] = value;
}

static void write_TCNT1H(io_t *io, const int8_t value) {

    io->memory[0x65] = value;
}

static void write_ICR1L(io_t *io, const int8_t value) {

    io->memory[0x66] = value;
}

static void write_ICR1H(io_t *io, const int8_t value) {

    io->memory[0x67] = value;
}

static void write_OCR1AL(io_t *io, const int8_t value) {

    io->memory[0x68] = value;
}

static void write_OCR1AH(io_t *io, const int8_t value) {

    io->memory[0x69] = value;
}

static void write_OCR1BL(io_t *io, const int8_t value) {

    io->memory[0x6a] = value;
}

static void write_OCR1BH(io_t *io, const int8_t value) {

    io->memory[0x6b] = value;
}

static void write_TCCR2A(io_t *io, const int8_t value) {

    io->memory[0x90] = value;
}

static void write_TCCR2B(io_t *io, const int8_t value) {

    io->memory[0x91] = value;
}

static void write_TCNT2(io_t *io, const int8_t value) {

    io->memory[0x92] = value;
}

static void write_OCR2A(io_t *io, const int8_t value) {

    io->memory[0x93] = value;
}

static void write_OCR2B(io_t *io, const int8_t value) {

    io->memory[0x94] = value;
}

static void write_ASSR(io_t *io, const int8_t value) {

    io->memory[0x96] = value;
}

static void write_TWBR(io_t *io, const int8_t value) {

    io->memory[0x98] = value;
}

static void write_TWSR(io_t *io, const int8_t value) {

    io->memory[0x99] = value;
}

static void write_TWAR(io_t *io, const int8_t value) {

    io->memory[0x9a] = value;
}

static void write_TWDR(io_t *io, const int8_t value) {

    io->memory[0x9b] = value;
}

static void write_TWCR(io_t *io, const int8_t value) {

    io->memory[0x9c] = value;
}

static void write_TWAMR(io_t *io, const int8_t value) {

    io->memory[0x9d] = value;
}

static void write_UCSR0A(io_t *io, const int8_t value) {

    io->memory[0xa0] = value;
}

static void write_UCSR0B(io_t *io, const int8_t value) {

    io->memory[0xa1] = value;
}

static void write_UCSR0C(io_t *io, const int8_t value) {

    io->memory[0xa2] = value;
}

static void write_UBRR0L(io_t *io, const int8_t value) {

    io->memory[0xa4] = value;
}

static void write_UBRR0H(io_t *io, const int8_t value) {

    io->memory[0xa5] = value;
}

static void write_UDR0(io_t *io, const int8_t value) {

    io->memory[0xa6] = value;
}

static int8_t read_reserved(io_t *io) {
    
    /* read access on reserved memory */
    return 0x00;
}

static int8_t read_PINB(io_t *io) {

    return io->memory[0x03];
}

static int8_t read_DDRB(io_t *io) {

    return io->memory[0x04];
}

static int8_t read_PORTB(io_t *io) {

    return io->memory[0x05];
}

static int8_t read_PINC(io_t *io) {

    return io->memory[0x06];
}

static int8_t read_DDRC(io_t *io) {

    return io->memory[0x07];
}

static int8_t read_PORTC(io_t *io) {

    return io->memory[0x08];
}

static int8_t read_PIND(io_t *io) {

    return io->memory[0x09];
}

static int8_t read_DDRD(io_t *io) {

    return io->memory[0x0a];
}

static int8_t read_PORTD(io_t *io) {

    return io->memory[0x0b];
}

static int8_t read_TIFR0(io_t *io) {

    return io->memory[TIFR0];
}

static int8_t read_TIFR1(io_t *io) {

    return io->memory[0x16];
}

static int8_t read_TIFR2(io_t *io) {

    return io->memory[0x17];
}

static int8_t read_PCIFR(io_t *io) {

    return io->memory[0x1b];
}

static int8_t read_EIFR(io_t *io) {

    return io->memory[0x1c];
}

static int8_t read_EIMSK(io_t *io) {

    return io->memory[0x1d];
}

static int8_t read_GPIOR0(io_t *io) {

    return io->memory[0x1e];
}

static int8_t read_EECR(io_t *io) {

    return io->memory[0x1f];
}

static int8_t read_EEDR(io_t *io) {

    return io->memory[0x20];
}

static int8_t read_EEARL(io_t *io) {

    return io->memory[0x21];
}

static int8_t read_EEARH(io_t *io) {

    return io->memory[0x22];
}

static int8_t read_GTCCR(io_t *io) {

    return io->memory[0x23];
}

static int8_t read_TCCR0A(io_t *io) {

    return io->memory[0x24];
}

static int8_t read_TCCR0B(io_t *io) {

    return io->memory[0x25];
}

static int8_t read_TCNT0(io_t *io) {

    return io->memory[0x26];
}

static int8_t read_OCR0A(io_t *io) {

    return io->memory[0x27];
}

static int8_t read_OCR0B(io_t *io) {

    return io->memory[0x28];
}

static int8_t read_GPIOR1(io_t *io) {

    return io->memory[0x2a];
}

static int8_t read_GPIOR2(io_t *io) {

    return io->memory[0x2b];
}

static int8_t read_SPCR0(io_t *io) {

    return io->memory[0x2c];
}

static int8_t read_SPSR0(io_t *io) {

    return io->memory[0x2d];
}

static int8_t read_SPDR0(io_t *io) {

    return io->memory[0x2e];
}

static int8_t read_ACSR(io_t *io) {

    return io->memory[0x30];
}

static int8_t read_DWDR(io_t *io) {

    return io->memory[0x31];
}

static int8_t read_SMCR(io_t *io) {

    return io->memory[0x33];
}

static int8_t read_MCUSR(io_t *io) {

    return io->memory[0x34];
}

static int8_t read_MCUCR(io_t *io) {

    return io->memory[0x35];
}

static int8_t read_SPMCSR(io_t *io) {

    return io->memory[0x37];
}

static int8_t read_SPL(io_t *io) {

    return io->memory[0x3d];
}

static int8_t read_SPH(io_t *io) {

    return io->memory[0x3e];
}

static int8_t read_SREG(io_t *io) {

    return io->memory[0x3f];
}

static int8_t read_WDTCSR(io_t *io) {

    return io->memory[0x40];
}

static int8_t read_CLKPR(io_t *io) {

    return io->memory[0x41];
}

static int8_t read_PRR(io_t *io) {

    return io->memory[0x44];
}

static int8_t read_OSCCAL(io_t *io) {

    return io->memory[0x46];
}

static int8_t read_PCICR(io_t *io) {

    return io->memory[0x48];
}

static int8_t read_EICRA(io_t *io) {

    return io->memory[0x49];
}

static int8_t read_PCMSK0(io_t *io) {

    return io->memory[0x4b];
}

static int8_t read_PCMSK1(io_t *io) {

    return io->memory[0x4c];
}

static int8_t read_PCMSK2(io_t *io) {

    return io->memory[0x4d];
}

static int8_t read_TIMSK0(io_t *io) {

    return io->memory[TIMSK0];
}

static int8_t read_TIMSK1(io_t *io) {

    return io->memory[0x4f];
}

static int8_t read_TIMSK2(io_t *io) {

    return io->memory[0x50];
}

static int8_t read_ADCL(io_t *io) {

    return io->memory[0x58];
}

static int8_t read_ADCH(io_t *io) {

    return io->memory[0x59];
}

static int8_t read_ADCSRA(io_t *io) {

    return io->memory[0x5a];
}

static int8_t read_ADCSRB(io_t *io) {

    return io->memory[0x5b];
}

static int8_t read_ADMUX(io_t *io) {

    return io->memory[0x5c];
}

static int8_t read_DIDR0(io_t *io) {

    return io->memory[0x5e];
}

static int8_t read_DIDR1(io_t *io) {

    return io->memory[0x5f];
}

static int8_t read_TCCR1A(io_t *io) {

    return io->memory[0x60];
}

static int8_t read_TCCR1B(io_t *io) {

    return io->memory[0x61];
}

static int8_t read_TCCR1C(io_t *io) {

    return io->memory[0x62];
}

static int8_t read_TCNT1L(io_t *io) {

    return io->memory[0x64];
}

static int8_t read_TCNT1H(io_t *io) {

    return io->memory[0x65];
}

static int8_t read_ICR1L(io_t *io) {

    return io->memory[0x66];
}

static int8_t read_ICR1H(io_t *io) {

    return io->memory[0x67];
}

static int8_t read_OCR1AL(io_t *io) {

    return io->memory[0x68];
}

static int8_t read_OCR1AH(io_t *io) {

    return io->memory[0x69];
}

static int8_t read_OCR1BL(io_t *io) {

    return io->memory[0x6a];
}

static int8_t read_OCR1BH(io_t *io) {

    return io->memory[0x6b];
}

static int8_t read_TCCR2A(io_t *io) {

    return io->memory[0x90];
}

static int8_t read_TCCR2B(io_t *io) {

    return io->memory[0x91];
}

static int8_t read_TCNT2(io_t *io) {

    return io->memory[0x92];
}

static int8_t read_OCR2A(io_t *io) {

    return io->memory[0x93];
}

static int8_t read_OCR2B(io_t *io) {

    return io->memory[0x94];
}

static int8_t read_ASSR(io_t *io) {

    return io->memory[0x96];
}

static int8_t read_TWBR(io_t *io) {

    return io->memory[0x98];
}

static int8_t read_TWSR(io_t *io) {

    return io->memory[0x99];
}

static int8_t read_TWAR(io_t *io) {

    return io->memory[0x9a];
}

static int8_t read_TWDR(io_t *io) {

    return io->memory[0x9b];
}

static int8_t read_TWCR(io_t *io) {

    return io->memory[0x9c];
}

static int8_t read_TWAMR(io_t *io) {

    return io->memory[0x9d];
}

static int8_t read_UCSR0A(io_t *io) {

    return io->memory[0xa0];
}

static int8_t read_UCSR0B(io_t *io) {

    return io->memory[0xa1];
}

static int8_t read_UCSR0C(io_t *io) {

    return io->memory[0xa2];
}

static int8_t read_UBRR0L(io_t *io) {

    return io->memory[0xa4];
}

static int8_t read_UBRR0H(io_t *io) {

    return io->memory[0xa5];
}

static int8_t read_UDR0(io_t *io) {

    return io->memory[0xa6];
}

void (*sfr_set[SFRL_SIZE]) (io_t *this, const int bit) = {

    set_reserved,
    set_reserved,
    set_reserved,
    set_PINB,
    set_DDRB,
    set_PORTB,
    set_PINC,
    set_DDRC,
    set_PORTC,
    set_PIND,
    set_DDRD,
    set_PORTD,
    set_reserved,
    set_reserved,
    set_reserved,
    set_reserved,
    set_reserved,
    set_reserved,
    set_reserved,
    set_reserved,
    set_reserved,
    set_TIFR0,
    set_TIFR1,
    set_TIFR2,
    set_reserved,
    set_reserved,
    set_reserved,
    set_PCIFR,
    set_EIFR,
    set_EIMSK,
    set_GPIOR0,
    set_EECR
};

void (*sfr_clear[SFRL_SIZE]) (io_t *this, const int bit) = {
  
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_PINB,
    clear_DDRB,
    clear_PORTB,
    clear_PINC,
    clear_DDRC,
    clear_PORTC,
    clear_PIND,
    clear_DDRD,
    clear_PORTD,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_TIFR0,
    clear_TIFR1,
    clear_TIFR2,
    clear_reserved,
    clear_reserved,
    clear_reserved,
    clear_PCIFR,
    clear_EIFR,
    clear_EIMSK,
    clear_GPIOR0,
    clear_EECR
};

void (*sfr_write[SFR_SIZE]) (io_t *this, const int8_t value) = {

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

int8_t (*sfr_read[SFR_SIZE]) (io_t *this) = {

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







