/* EEPROM Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Project Headers (engine)
#include "engine/include/system/peripherals/eeprom.h"
#include "engine/include/system/core/irq.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"
#include "engine/include/misc/bitmanip.h"

#define NMOD 4
#define UNDEFINED -1

/* Forward Declaration of static Functions */

static void eeprom_exec_atomic_ew(vmcu_eeprom_t *this);
static void eeprom_exec_erase(vmcu_eeprom_t *this);
static void eeprom_exec_write(vmcu_eeprom_t *this);
static void eeprom_exec_reserved(vmcu_eeprom_t *this);

/* Forward Declaration of static Members */

static void (*exec[NMOD]) (vmcu_eeprom_t *this);

/* --- Extern --- */

vmcu_eeprom_t* vmcu_eeprom_ctor(int8_t *io_memory) {

    vmcu_eeprom_t *eeprom;
    
    if((eeprom = malloc(sizeof(vmcu_eeprom_t))) == NULL)
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

void vmcu_eeprom_dtor(vmcu_eeprom_t *this) {

    free(this->memory);
    free(this);
}

void vmcu_eeprom_update(vmcu_eeprom_t *this, vmcu_irq_t *irq, uint32_t cpu_clk, uint64_t dc) {

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
            vmcu_irq_enable(irq, ERDY_VECT);
    }

    this->mw_lock = false;
    this->cycle -= rounded;
}

void vmcu_eeprom_enable_write(vmcu_eeprom_t *this) {
    
    this->mw_counter = 4;
    this->mw_enabled = true;
    this->mw_lock = true;
}

void vmcu_eeprom_try_read(vmcu_eeprom_t *this) {

    if(bit(*(this->eecr), EEPE) == 0x01)
        return;

    const uint8_t al = *(this->eearl);
    const uint8_t ah = *(this->eearh);
    
    const uint16_t addr = ((ah << 8) + al);

    if(addr < EEPROM_SIZE)
        *(this->eedr) = this->memory[addr];

    /* missing: cpu halt (4 cycles) */
}

int vmcu_eeprom_try_write(vmcu_eeprom_t *this) {

    if(bit(*(this->eecr), EEPE) == 0x00)
        return -1;

    if(bit(*(this->spmcsr), SPMEN) == 0x01) {

        clearbit(*(this->eecr), EEPE);       
        return -1;
    }
        
    if(this->mw_enabled == false) {
     
        clearbit(*(this->eecr), EEPE);
        return -1;
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
    return 0;
}

bool vmcu_eeprom_is_busy(vmcu_eeprom_t *this) {

    if((*(this->eecr) & (0x01 << EEPE)) != 0x00)
        return true;

    if((*(this->eecr) & (0x01 << EERIE)) != 0x00)
        return true;

    if(this->mw_enabled == true)
        return true;

    return false;
}

int8_t* vmcu_eeprom_dump(const vmcu_eeprom_t *this) {
    
    return this->memory;
}

void vmcu_eeprom_reboot(vmcu_eeprom_t *this) {
    
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

static void eeprom_exec_atomic_ew(vmcu_eeprom_t *this) {
    
    if(this->addr >= EEPROM_SIZE)
        return;

    this->memory[ this->addr ] = this->data;
}

static void eeprom_exec_erase(vmcu_eeprom_t *this) {

    if(this->addr >= EEPROM_SIZE)
        return;

    this->memory[ this->addr ] = 0xff;
}

static void eeprom_exec_write(vmcu_eeprom_t *this) {
    
    if(this->addr >= EEPROM_SIZE)
        return;

    this->memory[ this->addr ] &= this->data;
}

static void eeprom_exec_reserved(vmcu_eeprom_t *this) {
    
    /* reserved EEPROM mode */
    return;
}

static void (*exec[NMOD]) (vmcu_eeprom_t *this) = {
    
    eeprom_exec_atomic_ew,
    eeprom_exec_erase,
    eeprom_exec_write,
    eeprom_exec_reserved
};
