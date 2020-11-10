/* EEPROM Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Project Headers
#include "system/peripherals/eeprom.h"
#include "system/core/irq.h"
#include "system/mcudef.h"
#include "misc/bitmanip.h"

#define NMOD 4
#define UNDEFINED -1

/* Forward Declaration of static Functions */

static void eeprom_exec_atomic_ew(struct _eeprom *this);
static void eeprom_exec_erase(struct _eeprom *this);
static void eeprom_exec_write(struct _eeprom *this);
static void eeprom_exec_reserved(struct _eeprom *this);

/* Forward Declaration of static Members */

static void (*exec[NMOD]) (struct _eeprom *this);

/* --- Extern --- */

struct _eeprom* eeprom_ctor(int8_t *io_memory) {

    struct _eeprom *eeprom;
    
    if((eeprom = malloc(sizeof(struct _eeprom))) == NULL)
        return NULL;
        
    eeprom->memory = malloc(EEPROM_SIZE * sizeof(int8_t));    
    memset(eeprom->memory, 0xff, EEPROM_SIZE * sizeof(int8_t));

    eeprom->spmcsr = &io_memory[SPMCSR];
    
    eeprom->eecr = &io_memory[EECR];
    eeprom->eedr = &io_memory[EEDR];
    
    eeprom->eearl = &io_memory[EEARL];
    eeprom->eearh = &io_memory[EEARH];
    
    eeprom->mw_counter = 0;
    eeprom->mw_enabled = false;
    eeprom->mw_lock = false;
    
    eeprom->cycle = 0.00;
    
    eeprom->wapc = UNDEFINED;
    eeprom->data = UNDEFINED;
    eeprom->addr = UNDEFINED;
    
    return eeprom;
}

void eeprom_dtor(struct _eeprom *this) {

    free(this->memory);
    free(this);
}

void eeprom_update(struct _eeprom *this, irq_t *irq, const uint32_t cpu_clk, const uint64_t dc) {

    this->cycle += (((double) EEP_CLK / (double) cpu_clk) * (double) dc);
    const int rounded = floor(this->cycle);

    if(this->mw_enabled == true && this->mw_lock == false)
        this->mw_counter -= dc;

    if(this->mw_counter <= 0) {

        *(this->eecr) &= ~(0x01 << EEMPE);
        this->mw_enabled = false;
    }

    if(this->wapc != UNDEFINED) {

        if((this->wapc -= rounded) <= 0) {

            (*exec[ eep_mode(*(this->eecr)) ])(this);
            clearbit(*(this->eecr), EEPE);

            this->wapc = UNDEFINED;
            this->data = UNDEFINED;
            this->addr = UNDEFINED;   
        }
    }
    
    if(bit(*(this->eecr), EERIE) == 0x01) {

        if(bit(*(this->eecr), EEPE) == 0x00)
            irq_enable(irq, ERDY_VECT);
    }

    this->mw_lock = false;
    this->cycle -= rounded;
}

void eeprom_enable_write(struct _eeprom *this) {
    
    this->mw_counter = 4;
    this->mw_enabled = true;
    this->mw_lock = true;
}

void eeprom_try_read(struct _eeprom *this) {

    if(bit(*(this->eecr), EEPE) == 0x01)
        return;
        
    const uint8_t al = *(this->eearl);
    const uint8_t ah = *(this->eearh);
    
    const uint16_t addr = ((ah << 8) + al);

    if(addr < EEPROM_SIZE)
        *(this->eedr) = this->memory[addr];

    /* missing: cpu halt (4 cycles) */
}

void eeprom_try_write(struct _eeprom *this) {

    if(bit(*(this->eecr), EEPE) == 0x00)
        return;

    if(bit(*(this->spmcsr), SPMEN) == 0x01) {

        clearbit(*(this->eecr), EEPE);       
        return;
    }
        
    if(this->mw_enabled == false) {
     
        clearbit(*(this->eecr), EEPE);
        return;
    }
    
    switch( eep_mode(*(this->eecr)) ) {
        
        case 0x00: this->wapc = EEEW_CYCLES; break;
        case 0x01: this->wapc = EEE_CYCLES;  break;
        case 0x02: this->wapc = EEW_CYCLES;  break;
        case 0x03: this->wapc = UNDEFINED;   break;

        default:   /* not possible */        break;
    }
    
    const uint8_t al = *(this->eearl);
    const uint8_t ah = *(this->eearh);

    this->data = *(this->eedr);
    this->addr = ((ah << 8) + al);

    // missing: cpu halt (2 cycles)
}

bool eeprom_is_busy(struct _eeprom *this) {

    if((*(this->eecr) & (0x01 << EEPE)) != 0x00)
        return true;

    if((*(this->eecr) & (0x01 << EERIE)) != 0x00)
        return true;

    if(this->mw_enabled == true)
        return true;
        
    return false;
}

int8_t* eeprom_dump(const struct _eeprom *this) {
    
    return this->memory;
}

void eeprom_reboot(struct _eeprom *this) {
    
    memset(this->memory, 0xff, EEPROM_SIZE * sizeof(int8_t));
    
    this->mw_counter = 0;
    this->mw_enabled = false;
    this->mw_lock = false;
    
    this->cycle = 0.00;
    
    this->wapc = UNDEFINED;
    this->data = UNDEFINED;
    this->addr = UNDEFINED;
}

/* --- Static --- */

static void eeprom_exec_atomic_ew(struct _eeprom *this) {
    
    if(this->addr >= EEPROM_SIZE)
        return;

    this->memory[ this->addr ] = this->data;
}

static void eeprom_exec_erase(struct _eeprom *this) {

    if(this->addr >= EEPROM_SIZE)
        return;

    this->memory[ this->addr ] = 0xff;
}

static void eeprom_exec_write(struct _eeprom *this) {
    
    /* currently not supported */
}

static void eeprom_exec_reserved(struct _eeprom *this) {
    
    /* reserved EEPROM mode */
    return;
}

static void (*exec[NMOD]) (struct _eeprom *this) = {
    
    eeprom_exec_atomic_ew,
    eeprom_exec_erase,
    eeprom_exec_write,
    eeprom_exec_reserved
};
