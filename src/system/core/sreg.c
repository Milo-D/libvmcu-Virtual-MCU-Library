/* Status Register Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/core/sreg.h"
#include "system/mcudef.h"
#include "printer/memprop.h"
#include "collections/array.h"

/* Forward Declaration of static SREG Functions */

static void sreg_clear_coi(const struct _sreg *this);

/* --- Public --- */

struct _sreg* sreg_ctor(void) {

    struct _sreg *sreg;
    
    if((sreg = malloc(sizeof(struct _sreg))) == NULL)
        return NULL;
    
    sreg->coi = malloc(SREG_SIZE * sizeof(int));
    memset(sreg->coi, NONE, SREG_SIZE * sizeof(int));
    
    sreg->status = 0x00;
    return sreg;
}

void sreg_dtor(struct _sreg *this) {
    
    free(this->coi);
    free(this);
}

void sreg_write(struct _sreg *this, const int flag, const bool bit) {

    if(flag < 0 || flag >= SREG_SIZE)
        return;

    this->coi[flag] = DEST;

    if(bit == true) {
    
        this->status |= (0x01 << flag);
        return;
    }
	
    this->status &= ~(0x01 << flag);
}

bool sreg_read(const struct _sreg *this, const int flag) {

    if(flag < 0 || flag >= SREG_SIZE)
        return false;
    
    this->coi[flag] = SRC;
    return ((this->status >> flag) & 0x01); 
}

void sreg_clear(struct _sreg *this) {

    this->status = 0x00;
}

void sreg_coi(const struct _sreg *this, array_t *buffer) {

    for(int i = 0; i < SREG_SIZE; i++) {
    
        const int prop = this->coi[i];
        array_push(buffer, (void*) &prop, sizeof(int));
    }

    sreg_clear_coi(this);
}

uint8_t sreg_dump(const struct _sreg *this) {

    return this->status;
}

void sreg_reboot(struct _sreg *this) {

    this->status = 0x00;
    memset(this->coi, NONE, SREG_SIZE * sizeof(int));
}

/* --- Private --- */

static void sreg_clear_coi(const struct _sreg *this) {

    for(int i = 0; i < SREG_SIZE; i++)
        this->coi[i] = NONE;
}
