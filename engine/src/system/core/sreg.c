/* Status Register Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/system/core/sreg.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"

/* --- Extern --- */

vmcu_sreg_t* vmcu_sreg_ctor(void) {

    vmcu_sreg_t *sreg;
    
    if((sreg = malloc(sizeof(vmcu_sreg_t))) == NULL)
        return NULL;
    
    sreg->status = 0x00;
    return sreg;
}

void vmcu_sreg_dtor(vmcu_sreg_t *this) {

    free(this);
}

void vmcu_sreg_write(vmcu_sreg_t *this, const int flag, const bool bit) {

    if(flag < 0 || flag >= SREG_SIZE)
        return;

    if(bit == true) {
    
        this->status |= (0x01 << flag);
        return;
    }
	
    this->status &= ~(0x01 << flag);
}

bool vmcu_sreg_read(const vmcu_sreg_t *this, const int flag) {

    if(flag < 0 || flag >= SREG_SIZE)
        return false;

    return ((this->status >> flag) & 0x01); 
}

void vmcu_sreg_write_byte(vmcu_sreg_t *this, const uint8_t byte) {

    this->status = byte;
}

uint8_t vmcu_sreg_read_byte(vmcu_sreg_t *this) {

    return this->status;
}

void vmcu_sreg_clear(vmcu_sreg_t *this) {

    this->status = 0x00;
}

uint8_t vmcu_sreg_dump(const vmcu_sreg_t *this) {

    return this->status;
}

void vmcu_sreg_reboot(vmcu_sreg_t *this) {

    this->status = 0x00;
}

