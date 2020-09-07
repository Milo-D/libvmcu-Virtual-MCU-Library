/* Implementation of AVR Data Memory */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/data.h"
#include "system/mcudef.h"
#include "printer/memprop.h"
#include "collections/array.h"
#include "collections/tuple.h"
#include "misc/bitmanip.h"

struct _private {

    int8_t *memory;
    tuple_t *coi;
};

/* Forward Declaration of static DATA Functions */

static void data_set_coi(const struct _data *this, const uint16_t cell, const int prop);
static void data_clear_coi(const struct _data *this);

/* --- Public --- */

struct _data* data_ctor(void) {

    struct _data *data;

    if((data = malloc(sizeof(struct _data))) == NULL)
        return NULL;

    if((data->p = malloc(sizeof(struct _private))) == NULL) {

        free(data);
        return NULL;
    }       

    data->p->memory = malloc((RAM_END + 1) * sizeof(int8_t));
    memset(data->p->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    data->p->coi = tuple_ctor(2, UINT16, INT);
    data_set_coi(data, 0x0000, NONE);

    data->size = (RAM_END + 1);
    return data;
}

void data_dtor(struct _data *this) {

    tuple_dtor(this->p->coi);

    free(this->p->memory);
    free(this->p);
    free(this);    
}

void data_push(struct _data *this, const int8_t value) {

    const uint8_t spl = this->p->memory[SPL];
    const uint8_t sph = this->p->memory[SPH];

    uint16_t sp = sp(spl, sph);

    if(sp <= SRAM_START)
        return;

    this->p->memory[sp--] = value;
    this->p->memory[SPL] = spl(sp);
    this->p->memory[SPH] = sph(sp);

    data_set_coi(this, sp + 1, DEST);
}

int8_t data_pop(const struct _data *this) {

    const uint8_t spl = this->p->memory[SPL];
    const uint8_t sph = this->p->memory[SPH];

    uint16_t sp = sp(spl, sph);

    if(sp >= RAM_END)
        return 0xff;

    const int8_t value = this->p->memory[++sp];

    this->p->memory[SPL] = spl(sp);
    this->p->memory[SPH] = sph(sp);

    data_set_coi(this, sp, SRC);
    return value;
}

void data_write(struct _data *this, const uint16_t addr, const int8_t value) {

    if(addr > RAM_END)
        return;

    this->p->memory[addr] = value;
    data_set_coi(this, addr, DEST);
}

int8_t data_read(const struct _data *this, const uint16_t addr) {

    if(addr > RAM_END)
        return 0xff;

    data_set_coi(this, addr, SRC);
    return this->p->memory[addr];
}

void data_coi(const struct _data *this, tuple_t *buffer) {

    const int cell = *((uint16_t*) tuple_get(this->p->coi, 0));
    const int prop = *((int*) tuple_get(this->p->coi, 1));

    tuple_set(buffer, (void*) &cell, sizeof(uint16_t), 0);
    tuple_set(buffer, (void*) &prop, sizeof(int), 1);

    data_clear_coi(this);
}

void data_dump(const struct _data *this, array_t *buffer) {

    for(int i = 0; i < (RAM_END + 1); i++)
        array_push(buffer, (void*) &this->p->memory[i], sizeof(int8_t));
}

void data_reboot(const struct _data *this) {

    memset(this->p->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));
    data_set_coi(this, 0x0000, NONE);
}

/* --- Private --- */

static void data_set_coi(const struct _data *this, const uint16_t cell, const int prop) {

    tuple_set(this->p->coi, (void*) &cell, sizeof(uint16_t), 0);
    tuple_set(this->p->coi, (void*) &prop, sizeof(int), 1);
}

static void data_clear_coi(const struct _data *this) {

    data_set_coi(this, 0x0000, NONE);
}
