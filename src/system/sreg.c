/* Status Register Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/sreg.h"
#include "system/mcudef.h"
#include "printer/memprop.h"
#include "collections/array.h"

struct _private {

    uint8_t status;
    int *coi;
};

/* Forward Declaration of static SREG Functions */

static void sreg_clear_coi(const struct _sreg *this);

/* --- Public --- */

struct _sreg* sreg_ctor(void) {

    struct _sreg *sreg;
    
    if((sreg = malloc(sizeof(struct _sreg))) == NULL)
        return NULL;
        
    if((sreg->p = malloc(sizeof(struct _private))) == NULL) {
    
        free(sreg);
        return NULL;
    }
    
    sreg->p->coi = malloc(SREG_SIZE * sizeof(int));
    memset(sreg->p->coi, NONE, SREG_SIZE * sizeof(int));
    
    sreg->p->status = 0x00;
    return sreg;
}

void sreg_dtor(struct _sreg *this) {
    
    free(this->p->coi);
    free(this->p);
    free(this);
}

void sreg_write(struct _sreg *this, const int flag, const bool bit) {

    if(flag < 0 || flag >= SREG_SIZE)
        return;

    this->p->coi[flag] = DEST;

    if(bit == true) {
    
        this->p->status |= (0x01 << flag);
        return;
    }
	
    this->p->status &= ~(0x01 << flag);
}

bool sreg_read(const struct _sreg *this, const int flag) {

    if(flag < 0 || flag >= SREG_SIZE)
        return false;
    
    this->p->coi[flag] = SRC;
    return ((this->p->status >> flag) & 0x01); 
}

void sreg_clear(const struct _sreg *this) {

    this->p->status = 0x00;
}

void sreg_coi(const struct _sreg *this, array_t *buffer) {

    for(int i = 0; i < SREG_SIZE; i++) {
    
        const int prop = this->p->coi[i];
        array_push(buffer, (void*) &prop, sizeof(int));
    }

    sreg_clear_coi(this);
}

uint8_t sreg_dump(const struct _sreg *this) {

    return this->p->status;
}

void sreg_reboot(const struct _sreg *this) {

    this->p->status = 0x00;
    memset(this->p->coi, NONE, SREG_SIZE * sizeof(int));
}

/* --- Private --- */

static void sreg_clear_coi(const struct _sreg *this) {

    for(int i = 0; i < SREG_SIZE; i++)
        this->p->coi[i] = NONE;
}
