/* IO Memory Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/system/core/io.h"
#include "engine/include/system/core/irq.h"
#include "engine/include/system/peripherals/timer8.h"
#include "engine/include/system/peripherals/eeprom.h"

// Project Headers (engine utilities)
#include "engine/include/misc/bitmanip.h"

vmcu_io_t* vmcu_io_ctor(int8_t *io_start) {

    vmcu_io_t *io;

    if((io = malloc(sizeof(vmcu_io_t))) == NULL)
        return NULL;

    io->memory = io_start;
    io->irq = vmcu_irq_ctor();
    
    /* Peripherals */
    
    io->timer0 = vmcu_timer8_ctor(VMCU_TC0, io->memory);
    io->eeprom = vmcu_eeprom_ctor(io->memory);

    return io;
}

void vmcu_io_dtor(vmcu_io_t *this) {

    vmcu_timer8_dtor(this->timer0);
    vmcu_eeprom_dtor(this->eeprom);
    vmcu_irq_dtor(this->irq);

    free(this);
}

void vmcu_io_update(vmcu_io_t *this, const uint32_t cpu_clk, const uint64_t dc) {

    /* Timer0 Update */

    if(vmcu_timer8_is_busy(this->timer0) == true)
        vmcu_timer8_update(this->timer0, this->irq, dc);

    /* EEPROM Update */

    if(vmcu_eeprom_is_busy(this->eeprom) == true)
        vmcu_eeprom_update(this->eeprom, this->irq, cpu_clk, dc);
}

int vmcu_io_check_irq(const vmcu_io_t *this) {

    if(this->irq->size < 1)
        return -1;

    uint16_t isr = 0x0000;
    vmcu_irq_pop(this->irq, &isr);

    switch(isr) {

        case RST_VECT:        /* not yet implemented */             break;
        case INT0_VECT:       /* not yet implemented */             break;
        case INT1_VECT:       /* not yet implemented */             break;
        case PCINT0_VECT:     /* not yet implemented */             break;
        case PCINT1_VECT:     /* not yet implemented */             break;
        case PCINT2_VECT:     /* not yet implemented */             break;
        case WDT_VECT:        /* not yet implemented */             break;
        case OC2A_VECT:       /* not yet implemented */             break;
        case OC2B_VECT:       /* not yet implemented */             break;
        case OVF2_VECT:       /* not yet implemented */             break;
        case CAPT1_VECT:      /* not yet implemented */             break;
        case OC1A_VECT:       /* not yet implemented */             break;
        case OC1B_VECT:       /* not yet implemented */             break;
        case OVF1_VECT:       /* not yet implemented */             break;
        case OC0A_VECT:       clearbit(this->memory[TIFR0], OCFA);  break;
        case OC0B_VECT:       clearbit(this->memory[TIFR0], OCFB);  break;
        case OVF0_VECT:       clearbit(this->memory[TIFR0], TOV);   break;
        case STC_VECT:        /* not yet implemented */             break;
        case USART_RX_VECT:   /* not yet implemented */             break;
        case USART_UDRE_VECT: /* not yet implemented */             break;
        case USART_TX_VECT:   /* not yet implemented */             break;
        case ADC_VECT:        /* not yet implemented */             break;
        case ERDY_VECT:       /* no flag clearing needed */         break;
        case ACOMP_VECT:      /* not yet implemented */             break;

        default:              /* should not happen */               break;
    }

    return (int) isr;
}

void vmcu_io_reboot(const vmcu_io_t *this) {

    vmcu_irq_reboot(this->irq);
    
    vmcu_timer8_reboot(this->timer0);
    vmcu_eeprom_reboot(this->eeprom);
}

int8_t* vmcu_io_dump_eeprom(const vmcu_io_t *this) {

    return vmcu_eeprom_dump(this->eeprom);
}

