/* IO Memory Header */

#ifndef IO_H
#define IO_H

// C Headers
#include <inttypes.h>

// Project Headers
#include "system/mcudef.h"

typedef struct _irq irq_t;

typedef struct _timer8 timer8_t;
typedef struct _eeprom eeprom_t;

typedef struct _io {

    /* IRQ-Handler */
    irq_t *irq;

    /* Peripherals */
    timer8_t *timer0;
    eeprom_t *eeprom;

    /* IO Memory */
    int8_t *memory;

} io_t;

extern struct _io* io_ctor(int8_t *io_start);
extern void io_dtor(struct _io *this);

/* IO Operations */

extern void io_update(struct _io *this, const uint32_t cpu_clk, const uint64_t dc);
extern int io_check_irq(const struct _io *this);
extern void io_reboot(const struct _io *this);

/* EEPROM Operations */

extern int8_t* io_dump_eeprom(const struct _io *this);

/* SFR Access Functions */

extern void (*io_write[SFR_SIZE]) (struct _io *this, const int8_t value);
extern int8_t (*io_read[SFR_SIZE]) (struct _io *this);

#endif
