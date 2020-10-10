/* IO Memory Header */

// C Headers
#include <stdlib.h>

// Project Headers
#include "system/core/io.h"
#include "system/core/irq.h"
#include "system/peripherals/timer8.h"
#include "system/mcudef.h"
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

void io_update(struct _io *this, const uint32_t clock, const double dt) {

    if((this->p->memory[TCCR0] & CSX_MSK) != 0x00)
        timer8_tick(this->p->timer0, this->p->irq, clock, dt);
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