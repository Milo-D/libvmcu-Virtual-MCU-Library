/* IO Memory Header */

// C Headers
#include <stdlib.h>

// Project Headers
#include "system/core/io.h"
#include "system/core/irq.h"
#include "system/peripherals/timer8.h"
#include "misc/bitmanip.h"

struct _private {

    int8_t *memory;

    /* IRQ-Handler */
    irq_t *irq;

    /* Peripherals */
    timer8_t *timer0;
};

struct _io* io_ctor(int8_t *io_start) {

    struct _io *io;

    if((io = malloc(sizeof(struct _io))) == NULL)
        return NULL;

    if((io->p = malloc(sizeof(struct _private))) == NULL) {

        free(io);
        return NULL;
    }

    io->p->memory = io_start;

    io->p->irq = irq_ctor();
    io->p->timer0 = timer8_ctor(TC0, io->p->memory);

    return io;
}

void io_dtor(struct _io *this) {

    timer8_dtor(this->p->timer0);
    irq_dtor(this->p->irq);

    free(this->p);
    free(this);
}

void io_update(struct _io *this, const uint64_t dc) {

    if((this->p->memory[TCCR0] & CSX_MSK) != 0x00)
        timer8_tick(this->p->timer0, this->p->irq, dc);
}

int io_check_irq(const struct _io *this) {

    if(this->p->irq->size < 1)
        return -1;

    uint16_t isr = 0x0000;
    irq_pop(this->p->irq, &isr);

    switch(isr) {

        case RST_VECT:  /* not yet implemented */           break;
        case INT0_VECT: /* not yet implemented */           break;
        case INT1_VECT: /* not yet implemented */           break;
        case INT2_VECT: /* not yet implemented */           break;
        case OC2_VECT:  /* not yet implemented */           break;
        case OVF2_VECT: /* not yet implemented */           break;
        case ICP1_VECT: /* not yet implemented */           break;
        case OC1A_VECT: /* not yet implemented */           break;
        case OC1B_VECT: /* not yet implemented */           break;
        case OVF1_VECT: /* not yet implemented */           break;
        case OC0_VECT:  clear(this->p->memory[TIFR], OCF0); break;
        case OVF0_VECT: clear(this->p->memory[TIFR], TOV0); break;
        case SPI_VECT:  /* not yet implemented */           break;
        case URXC_VECT: /* not yet implemented */           break;
        case UDRE_VECT: /* not yet implemented */           break;
        case UTXC_VECT: /* not yet implemented */           break;
        case ADCC_VECT: /* not yet implemented */           break;
        case ERDY_VECT: /* not yet implemented */           break;
        case ACI_VECT:  /* not yet implemented */           break;
        case TWI_VECT:  /* not yet implemented */           break;
        case SPMR_VECT: /* not yet implemented */           break;

        default: /* should not happen */ break;
    }

    return (int) isr;
}

void io_reboot(const struct _io *this) {

    irq_reboot(this->p->irq);
    timer8_reboot(this->p->timer0);
}

/* --- Static --- */

#ifdef ATMEGA32

static void write_twbr(struct _io *this, const int8_t value) {

    this->p->memory[0x00] = value;
}

static void write_twsr(struct _io *this, const int8_t value) {

    this->p->memory[0x01] = value;
}

static void write_twar(struct _io *this, const int8_t value) {

    this->p->memory[0x02] = value;
}

static void write_tdwr(struct _io *this, const int8_t value) {

    this->p->memory[0x03] = value;
}

static void write_adcl(struct _io *this, const int8_t value) {

    this->p->memory[0x04] = value;
}

static void write_adch(struct _io *this, const int8_t value) {

    this->p->memory[0x05] = value;
}

static void write_adcsra(struct _io *this, const int8_t value) {

    this->p->memory[0x06] = value;
}

static void write_admux(struct _io *this, const int8_t value) {

    this->p->memory[0x07] = value;
}

static void write_acsr(struct _io *this, const int8_t value) {

    this->p->memory[0x08] = value;
}

static void write_ubrrl(struct _io *this, const int8_t value) {

    this->p->memory[0x09] = value;
}

static void write_ucsrb(struct _io *this, const int8_t value) {

    this->p->memory[0x0a] = value;
}

static void write_ucsra(struct _io *this, const int8_t value) {

    this->p->memory[0x0b] = value;
}

static void write_udr(struct _io *this, const int8_t value) {

    this->p->memory[0x0c] = value;
}

static void write_spcr(struct _io *this, const int8_t value) {

    this->p->memory[0x0d] = value;
}

static void write_spsr(struct _io *this, const int8_t value) {

    this->p->memory[0x0e] = value;
}

static void write_spdr(struct _io *this, const int8_t value) {

    this->p->memory[0x0f] = value;
}

static void write_pind(struct _io *this, const int8_t value) {

    this->p->memory[0x10] = value;
}

static void write_ddrd(struct _io *this, const int8_t value) {

    this->p->memory[0x11] = value;
}

static void write_portd(struct _io *this, const int8_t value) {

    this->p->memory[0x12] = value;
}

static void write_pinc(struct _io *this, const int8_t value) {

    this->p->memory[0x13] = value;
}

static void write_ddrc(struct _io *this, const int8_t value) {

    this->p->memory[0x14] = value;
}

static void write_portc(struct _io *this, const int8_t value) {

    this->p->memory[0x15] = value;
}

static void write_pinb(struct _io *this, const int8_t value) {

    this->p->memory[0x16] = value;
}

static void write_ddrb(struct _io *this, const int8_t value) {

    this->p->memory[0x17] = value;
}

static void write_portb(struct _io *this, const int8_t value) {

    this->p->memory[0x18] = value;
}

static void write_pina(struct _io *this, const int8_t value) {

    this->p->memory[0x19] = value;
}

static void write_ddra(struct _io *this, const int8_t value) {

    this->p->memory[0x1a] = value;
}

static void write_porta(struct _io *this, const int8_t value) {

    this->p->memory[0x1b] = value;
}

static void write_eecr(struct _io *this, const int8_t value) {

    this->p->memory[0x1c] = value;
}

static void write_eedr(struct _io *this, const int8_t value) {

    this->p->memory[0x1d] = value;
}

static void write_eearl(struct _io *this, const int8_t value) {

    this->p->memory[0x1e] = value;
}

static void write_eearh(struct _io *this, const int8_t value) {

    this->p->memory[0x1f] = value;
}

static void write_ucsrc(struct _io *this, const int8_t value) {

    this->p->memory[0x20] = value;
}

static void write_wdtcr(struct _io *this, const int8_t value) {

    this->p->memory[0x21] = value;
}

static void write_assr(struct _io *this, const int8_t value) {

    this->p->memory[0x22] = value;
}

static void write_ocr2(struct _io *this, const int8_t value) {

    this->p->memory[0x23] = value;
}

static void write_tcnt2(struct _io *this, const int8_t value) {

    this->p->memory[0x24] = value;
}

static void write_tccr2(struct _io *this, const int8_t value) {

    this->p->memory[0x25] = value;
}

static void write_icr1l(struct _io *this, const int8_t value) {

    this->p->memory[0x26] = value;
}

static void write_icr1h(struct _io *this, const int8_t value) {

    this->p->memory[0x27] = value;
}

static void write_ocr1bl(struct _io *this, const int8_t value) {

    this->p->memory[0x28] = value;
}

static void write_ocr1bh(struct _io *this, const int8_t value) {

    this->p->memory[0x29] = value;
}

static void write_ocr1al(struct _io *this, const int8_t value) {

    this->p->memory[0x2a] = value;
}

static void write_ocr1ah(struct _io *this, const int8_t value) {

    this->p->memory[0x2b] = value;
}

static void write_tcnt1l(struct _io *this, const int8_t value) {

    this->p->memory[0x2c] = value;
}

static void write_tcnt1h(struct _io *this, const int8_t value) {

    this->p->memory[0x2d] = value;
}

static void write_tccr1b(struct _io *this, const int8_t value) {

    this->p->memory[0x2e] = value;
}

static void write_tccr1a(struct _io *this, const int8_t value) {

    this->p->memory[0x2f] = value;
}

static void write_sfior(struct _io *this, const int8_t value) {

    this->p->memory[0x30] = value;
}

static void write_osccal(struct _io *this, const int8_t value) {

    this->p->memory[0x31] = value;
}

static void write_tcnt0(struct _io *this, const int8_t value) {

    this->p->memory[0x32] = value;
}

static void write_tccr0(struct _io *this, const int8_t value) {

    this->p->memory[0x33] = value;
}

static void write_mcucsr(struct _io *this, const int8_t value) {

    this->p->memory[0x34] = value;
}

static void write_mcucr(struct _io *this, const int8_t value) {

    this->p->memory[0x35] = value;
}

static void write_twcr(struct _io *this, const int8_t value) {

    this->p->memory[0x36] = value;
}

static void write_spmcr(struct _io *this, const int8_t value) {

    this->p->memory[0x37] = value;
}

static void write_tifr(struct _io *this, const int8_t value) {

    this->p->memory[0x38] = value;
}

static void write_timsk(struct _io *this, const int8_t value) {

    this->p->memory[0x39] = value;
}

static void write_gifr(struct _io *this, const int8_t value) {

    this->p->memory[0x3a] = value;
}

static void write_gicr(struct _io *this, const int8_t value) {

    this->p->memory[0x3b] = value;
}

static void write_ocr0(struct _io *this, const int8_t value) {

    this->p->memory[0x3c] = value;
}

static void write_spl(struct _io *this, const int8_t value) {

    this->p->memory[0x3d] = value;
}

static void write_sph(struct _io *this, const int8_t value) {

    this->p->memory[0x3e] = value;
}

static void write_sreg(struct _io *this, const int8_t value) {

    this->p->memory[0x3f] = value;
}

static int8_t read_twbr(struct _io *this) {

    return this->p->memory[0x00];
}

static int8_t read_twsr(struct _io *this) {

    return this->p->memory[0x01];
}

static int8_t read_twar(struct _io *this) {

    return this->p->memory[0x02];
}

static int8_t read_tdwr(struct _io *this) {

    return this->p->memory[0x03];
}

static int8_t read_adcl(struct _io *this) {

    return this->p->memory[0x04];
}

static int8_t read_adch(struct _io *this) {

    return this->p->memory[0x05];
}

static int8_t read_adcsra(struct _io *this) {

    return this->p->memory[0x06];
}

static int8_t read_admux(struct _io *this) {

    return this->p->memory[0x07];
}

static int8_t read_acsr(struct _io *this) {

    return this->p->memory[0x08];
}

static int8_t read_ubrrl(struct _io *this) {

    return this->p->memory[0x09];
}

static int8_t read_ucsrb(struct _io *this) {

    return this->p->memory[0x0a];
}

static int8_t read_ucsra(struct _io *this) {

    return this->p->memory[0x0b];
}

static int8_t read_udr(struct _io *this) {

    return this->p->memory[0x0c];
}

static int8_t read_spcr(struct _io *this) {

    return this->p->memory[0x0d];
}

static int8_t read_spsr(struct _io *this) {

    return this->p->memory[0x0e];
}

static int8_t read_spdr(struct _io *this) {

    return this->p->memory[0x0f];
}

static int8_t read_pind(struct _io *this) {

    return this->p->memory[0x10];
}

static int8_t read_ddrd(struct _io *this) {

    return this->p->memory[0x11];
}

static int8_t read_portd(struct _io *this) {

    return this->p->memory[0x12];
}

static int8_t read_pinc(struct _io *this) {

    return this->p->memory[0x13];
}

static int8_t read_ddrc(struct _io *this) {

    return this->p->memory[0x14];
}

static int8_t read_portc(struct _io *this) {

    return this->p->memory[0x15];
}

static int8_t read_pinb(struct _io *this) {

    return this->p->memory[0x16];
}

static int8_t read_ddrb(struct _io *this) {

    return this->p->memory[0x17];
}

static int8_t read_portb(struct _io *this) {

    return this->p->memory[0x18];
}

static int8_t read_pina(struct _io *this) {

    return this->p->memory[0x19];
}

static int8_t read_ddra(struct _io *this) {

    return this->p->memory[0x1a];
}

static int8_t read_porta(struct _io *this) {

    return this->p->memory[0x1b];
}

static int8_t read_eecr(struct _io *this) {

    return this->p->memory[0x1c];
}

static int8_t read_eedr(struct _io *this) {

    return this->p->memory[0x1d];
}

static int8_t read_eearl(struct _io *this) {

    return this->p->memory[0x1e];
}

static int8_t read_eearh(struct _io *this) {

    return this->p->memory[0x1f];
}

static int8_t read_ucsrc(struct _io *this) {

    return this->p->memory[0x20];
}

static int8_t read_wdtcr(struct _io *this) {

    return this->p->memory[0x21];
}

static int8_t read_assr(struct _io *this) {

    return this->p->memory[0x22];
}

static int8_t read_ocr2(struct _io *this) {

    return this->p->memory[0x23];
}

static int8_t read_tcnt2(struct _io *this) {

    return this->p->memory[0x24];
}

static int8_t read_tccr2(struct _io *this) {

    return this->p->memory[0x25];
}

static int8_t read_icr1l(struct _io *this) {

    return this->p->memory[0x26];
}

static int8_t read_icr1h(struct _io *this) {

    return this->p->memory[0x27];
}

static int8_t read_ocr1bl(struct _io *this) {

    return this->p->memory[0x28];
}

static int8_t read_ocr1bh(struct _io *this) {

    return this->p->memory[0x29];
}

static int8_t read_ocr1al(struct _io *this) {

    return this->p->memory[0x2a];
}

static int8_t read_ocr1ah(struct _io *this) {

    return this->p->memory[0x2b];
}

static int8_t read_tcnt1l(struct _io *this) {

    return this->p->memory[0x2c];
}

static int8_t read_tcnt1h(struct _io *this) {

    return this->p->memory[0x2d];
}

static int8_t read_tccr1b(struct _io *this) {

    return this->p->memory[0x2e];
}

static int8_t read_tccr1a(struct _io *this) {

    return this->p->memory[0x2f];
}

static int8_t read_sfior(struct _io *this) {

    return this->p->memory[0x30];
}

static int8_t read_osccal(struct _io *this) {

    return this->p->memory[0x31];
}

static int8_t read_tcnt0(struct _io *this) {

    return this->p->memory[0x32];
}

static int8_t read_tccr0(struct _io *this) {

    return this->p->memory[0x33];
}

static int8_t read_mcucsr(struct _io *this) {

    return this->p->memory[0x34];
}

static int8_t read_mcucr(struct _io *this) {

    return this->p->memory[0x35];
}

static int8_t read_twcr(struct _io *this) {

    return this->p->memory[0x36];
}

static int8_t read_spmcr(struct _io *this) {

    return this->p->memory[0x37];
}

static int8_t read_tifr(struct _io *this) {

    return this->p->memory[0x38];
}

static int8_t read_timsk(struct _io *this) {

    return this->p->memory[0x39];
}

static int8_t read_gifr(struct _io *this) {

    return this->p->memory[0x3a];
}

static int8_t read_gicr(struct _io *this) {

    return this->p->memory[0x3b];
}

static int8_t read_ocr0(struct _io *this) {

    return this->p->memory[0x3c];
}

static int8_t read_spl(struct _io *this) {

    return this->p->memory[0x3d];
}

static int8_t read_sph(struct _io *this) {

    return this->p->memory[0x3e];
}

static int8_t read_sreg(struct _io *this) {

    return this->p->memory[0x3f];
}

void (*io_write[SFR_SIZE]) (struct _io *this, const int8_t value) = {

    write_twbr,
    write_twsr,
    write_twar,
    write_tdwr,
    write_adcl,
    write_adch,
    write_adcsra,
    write_admux,
    write_acsr,
    write_ubrrl,
    write_ucsrb,
    write_ucsra,
    write_udr,
    write_spcr,
    write_spsr,
    write_spdr,
    write_pind,
    write_ddrd,
    write_portd,
    write_pinc,
    write_ddrc,
    write_portc,
    write_pinb,
    write_ddrb,
    write_portb,
    write_pina,
    write_ddra,
    write_porta,
    write_eecr,
    write_eedr,
    write_eearl,
    write_eearh,
    write_ucsrc,
    write_wdtcr,
    write_assr,
    write_ocr2,
    write_tcnt2,
    write_tccr2,
    write_icr1l,
    write_icr1h,
    write_ocr1bl,
    write_ocr1bh,
    write_ocr1al,
    write_ocr1ah,
    write_tcnt1l,
    write_tcnt1h,
    write_tccr1b,
    write_tccr1a,
    write_sfior,
    write_osccal,
    write_tcnt0,
    write_tccr0,
    write_mcucsr,
    write_mcucr,
    write_twcr,
    write_spmcr,
    write_tifr,
    write_timsk,
    write_gifr,
    write_gicr,
    write_ocr0,
    write_spl,
    write_sph,
    write_sreg
};

int8_t (*io_read[SFR_SIZE]) (struct _io *this) = {

    read_twbr,
    read_twsr,
    read_twar,
    read_tdwr,
    read_adcl,
    read_adch,
    read_adcsra,
    read_admux,
    read_acsr,
    read_ubrrl,
    read_ucsrb,
    read_ucsra,
    read_udr,
    read_spcr,
    read_spsr,
    read_spdr,
    read_pind,
    read_ddrd,
    read_portd,
    read_pinc,
    read_ddrc,
    read_portc,
    read_pinb,
    read_ddrb,
    read_portb,
    read_pina,
    read_ddra,
    read_porta,
    read_eecr,
    read_eedr,
    read_eearl,
    read_eearh,
    read_ucsrc,
    read_wdtcr,
    read_assr,
    read_ocr2,
    read_tcnt2,
    read_tccr2,
    read_icr1l,
    read_icr1h,
    read_ocr1bl,
    read_ocr1bh,
    read_ocr1al,
    read_ocr1ah,
    read_tcnt1l,
    read_tcnt1h,
    read_tccr1b,
    read_tccr1a,
    read_sfior,
    read_osccal,
    read_tcnt0,
    read_tccr0,
    read_mcucsr,
    read_mcucr,
    read_twcr,
    read_spmcr,
    read_tifr,
    read_timsk,
    read_gifr,
    read_gicr,
    read_ocr0,
    read_spl,
    read_sph,
    read_sreg
};

#endif

