/* EEPROM Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/eeprom.h"
#include "system/mcudef.h"
#include "printer/memprop.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    int8_t *memory;
    tuple_t *coi;
};

/* Forward Declaration of static EEPROM Functions */

static void eeprom_set_coi(const struct _eeprom *this, const int cell, const int prop);
static void eeprom_clear_coi(const struct _eeprom *this);

/* --- Public --- */

struct _eeprom* eeprom_ctor(void) {

    struct _eeprom *eeprom;
    
    if((eeprom = malloc(sizeof(struct _eeprom))) == NULL)
        return NULL;
        
    if((eeprom->p = malloc(sizeof(struct _private))) == NULL) {
    
        free(eeprom);
        return NULL;
    }

    eeprom->p->memory = malloc(EEPROM_SIZE * sizeof(int8_t));
    memset(eeprom->p->memory, 0x00, EEPROM_SIZE * sizeof(int8_t));

    eeprom->p->coi = tuple_ctor(2, INT, INT);
    eeprom_set_coi(eeprom, 0x0000, NONE);

    eeprom->size = EEPROM_SIZE;
    return eeprom;
}

void eeprom_dtor(struct _eeprom *this) {

    tuple_dtor(this->p->coi);
    
    free(this->p->memory);
    free(this->p);
    free(this);
}

void eeprom_write(struct _eeprom *this, const int addr, const int8_t value) {

    if(addr < 0x0000)
        return;

    if(addr >= EEPROM_SIZE)
        return;

    this->p->memory[addr] = value;
    eeprom_set_coi(this, addr, DEST);
}

int8_t eeprom_read(const struct _eeprom *this, const int addr) {

    if(addr < 0x0000)
        return 0xff;

    if(addr >= EEPROM_SIZE)
        return 0xff;

    eeprom_set_coi(this, addr, SRC);
    return this->p->memory[addr];
}

void eeprom_coi(const struct _eeprom *this, tuple_t *buffer) {

    const int cell = *((int*) tuple_get(this->p->coi, 0));
    const int prop = *((int*) tuple_get(this->p->coi, 1));
    
    tuple_set(buffer, (void*) &cell, sizeof(int), 0);
    tuple_set(buffer, (void*) &prop, sizeof(int), 1);

    eeprom_clear_coi(this);
}

void eeprom_dump(const struct _eeprom *this, array_t *buffer) {

    for(int i = 0; i < EEPROM_SIZE; i++) {
    
        const int8_t value = this->p->memory[i];
        array_push(buffer, (void*) &value, sizeof(int8_t));
    }
}

/* --- Private --- */

static void eeprom_set_coi(const struct _eeprom *this, const int cell, const int prop) {

    tuple_set(this->p->coi, (void*) &cell, sizeof(int), 0);
    tuple_set(this->p->coi, (void*) &prop, sizeof(int), 1);
}

static void eeprom_clear_coi(const struct _eeprom *this) {

    eeprom_set_coi(this, 0x0000, NONE);
}
