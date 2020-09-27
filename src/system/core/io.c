/* IO Memory Header */

// C Headers
#include <stdlib.h>

// Project Headers
#include "system/core/io.h"
#include "system/peripherals/timer8.h"
#include "system/mcudef.h"

struct _private {

    int8_t *memory;

    /* Peripherals */
    timer8_t *timer0;
};

struct _io* io_ctor(int8_t *io_start) {

    struct _io *io;

    if((io = malloc(sizeof(struct _io))) == NULL)
        return NULL;

    if((io->p = malloc(sizeof(struct _private))) == NULL) {

        free(io);
        return NULL;
    }

    io->p->memory = io_start;
    io->p->timer0 = timer8_ctor(TC0, io->p->memory);

    return io;
}

void io_dtor(struct _io *this) {

    timer8_dtor(this->p->timer0);

    free(this->p);
    free(this);
}

void io_update(struct _io *this, const uint32_t clock, const uint64_t dc) {

    if((this->p->memory[TCCR0] & CSX_MASK) != 0x00)
        timer8_tick(this->p->timer0, clock, dc);
}

void io_reboot(const struct _io *this) {

    timer8_reboot(this->p->timer0);
}