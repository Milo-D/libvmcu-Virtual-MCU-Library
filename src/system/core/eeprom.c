/* EEPROM Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/core/eeprom.h"
#include "system/mcudef.h"
#include "printer/memprop.h"
#include "collections/tuple.h"

/* Forward Declaration of static EEPROM Functions */

static void eeprom_set_coi(const struct _eeprom *this, const uint16_t cell, const int prop);
static void eeprom_clear_coi(const struct _eeprom *this);

/* --- Extern --- */

struct _eeprom* eeprom_ctor(void) {

    struct _eeprom *eeprom;
    
    if((eeprom = malloc(sizeof(struct _eeprom))) == NULL)
        return NULL;

    eeprom->memory = malloc(EEPROM_SIZE * sizeof(int8_t));
    memset(eeprom->memory, 0x00, EEPROM_SIZE * sizeof(int8_t));

    eeprom->coi = tuple_ctor(2, UINT16, INT);
    eeprom_set_coi(eeprom, 0x0000, NONE);

    return eeprom;
}

void eeprom_dtor(struct _eeprom *this) {

    tuple_dtor(this->coi);
    
    free(this->memory);
    free(this);
}

void eeprom_write(struct _eeprom *this, const uint16_t addr, const int8_t value) {

    if(addr >= EEPROM_SIZE)
        return;

    this->memory[addr] = value;
    eeprom_set_coi(this, addr, DEST);
}

int8_t eeprom_read(const struct _eeprom *this, const uint16_t addr) {

    if(addr >= EEPROM_SIZE)
        return 0xff;

    eeprom_set_coi(this, addr, SRC);
    return this->memory[addr];
}

void eeprom_coi(const struct _eeprom *this, tuple_t *buffer) {

    const int cell = *((uint16_t*) tuple_get(this->coi, 0));
    const int prop = *((int*) tuple_get(this->coi, 1));
    
    tuple_set(buffer, (void*) &cell, sizeof(uint16_t), 0);
    tuple_set(buffer, (void*) &prop, sizeof(int), 1);

    eeprom_clear_coi(this);
}

int8_t* eeprom_dump(const struct _eeprom *this) {

    return this->memory;
}

void eeprom_reboot(const struct _eeprom *this) {

    memset(this->memory, 0x00, EEPROM_SIZE * sizeof(int8_t));
    eeprom_set_coi(this, 0x0000, NONE);
}

/* --- Static --- */

static void eeprom_set_coi(const struct _eeprom *this, const uint16_t cell, const int prop) {

    tuple_set(this->coi, (void*) &cell, sizeof(uint16_t), 0);
    tuple_set(this->coi, (void*) &prop, sizeof(int), 1);
}

static void eeprom_clear_coi(const struct _eeprom *this) {

    eeprom_set_coi(this, 0x0000, NONE);
}
