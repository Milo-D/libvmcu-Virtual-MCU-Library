/* Implementation of AVR Data Memory */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/core/data.h"
#include "system/core/io.h"
#include "system/util/memprop.h"
#include "system/mcudef.h"
#include "collections/tuple.h"
#include "misc/bitmanip.h"

/* Forward Declaration of static DATA Functions */

static void data_set_coi(const struct _data *this, const uint16_t cell, const MEMPROP prop);
static void data_clear_coi(const struct _data *this);

/* --- Public --- */

struct _data* data_ctor(void) {

    struct _data *data;

    if((data = malloc(sizeof(struct _data))) == NULL)
        return NULL;  

    data->memory = malloc((RAM_END + 1) * sizeof(int8_t));
    memset(data->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    data->io = io_ctor(&data->memory[GPR_SIZE]);

    data->coi = tuple_ctor(2, UINT16, INT);
    data_set_coi(data, 0x0000, NONE);
    
    data->memory[SPL] = spl(RAM_END);
    data->memory[SPH] = sph(RAM_END);

    return data;
}

void data_dtor(struct _data *this) {

    io_dtor(this->io);
    tuple_dtor(this->coi);

    free(this->memory);
    free(this);
}

void data_push(struct _data *this, const int8_t value) {

    const uint8_t spl = this->memory[SPL];
    const uint8_t sph = this->memory[SPH];

    uint16_t sp = sp(spl, sph);

    if(sp > RAM_END)
        return;

    this->memory[sp--] = value;
    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    data_set_coi(this, sp + 1, DEST);
}

int8_t data_pop(const struct _data *this) {

    const uint8_t spl = this->memory[SPL];
    const uint8_t sph = this->memory[SPH];

    uint16_t sp = sp(spl, sph);

    if(sp >= RAM_END)
        return 0xff;

    const int8_t value = this->memory[++sp];

    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    data_set_coi(this, sp, SRC);
    return value;
}

void data_write(struct _data *this, const uint16_t addr, const int8_t value) {

    if(addr > RAM_END)
        return;

    if(addr >= SFR_START && addr <= SFR_END) {

        io_write_sfr(this->io, addr, value);
        data_set_coi(this, addr, DEST);

        return;
    }

    this->memory[addr] = value;
    data_set_coi(this, addr, DEST);
}

int8_t data_read(const struct _data *this, const uint16_t addr) {

    if(addr > RAM_END)
        return 0xff;

    if(addr >= SFR_START && addr <= SFR_END) {

        data_set_coi(this, addr, SRC);
        return io_read_sfr(this->io, addr);
    }

    data_set_coi(this, addr, SRC);
    return this->memory[addr];
}

void data_coi(const struct _data *this, tuple_t *buffer) {

    const uint16_t cell = *((uint16_t*) tuple_get(this->coi, 0));
    const MEMPROP prop = *((MEMPROP*) tuple_get(this->coi, 1));

    tuple_set(buffer, (void*) &cell, sizeof(uint16_t), 0);
    tuple_set(buffer, (void*) &prop, sizeof(MEMPROP), 1);

    data_clear_coi(this);
}

int8_t* data_dump(const struct _data *this) {

    return this->memory;
}

void data_reboot(const struct _data *this) {

    io_reboot(this->io);

    memset(this->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));
    data_set_coi(this, 0x0000, NONE);

    this->memory[SPL] = spl(RAM_END);
    this->memory[SPH] = sph(RAM_END);
}

void data_update_io(const struct _data *this, const uint32_t cpu_clk, const uint64_t dc) {

    io_update(this->io, cpu_clk, dc);
}

int data_check_irq(const struct _data *this) {

    return io_check_irq(this->io);
}

void data_set_sfr(struct _data *this, const uint16_t addr, const int bit) {
    
    if(addr < SFRL_START || addr > SFRL_END)
        return;

    io_set_sfr(this->io, addr, bit);
    data_set_coi(this, addr, DEST);
}

void data_clear_sfr(struct _data *this, const uint16_t addr, const int bit) {
    
    if(addr < SFRL_START || addr > SFRL_END)
        return;

    io_clear_sfr(this->io, addr, bit);
    data_set_coi(this, addr, DEST);
}

int8_t* data_dump_eeprom(const struct _data *this) {
    
    return io_dump_eeprom(this->io);
}

/* --- Private --- */

static void data_set_coi(const struct _data *this, const uint16_t cell, const MEMPROP prop) {

    tuple_set(this->coi, (void*) &cell, sizeof(uint16_t), 0);
    tuple_set(this->coi, (void*) &prop, sizeof(MEMPROP), 1);
}

static void data_clear_coi(const struct _data *this) {

    data_set_coi(this, 0x0000, NONE);
}
