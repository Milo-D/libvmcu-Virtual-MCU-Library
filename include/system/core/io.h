/* IO Memory Header */

#ifndef IO_H
#define IO_H

// C Headers
#include <inttypes.h>

// Project Headers
#include "system/core/sfr.h"
#include "system/mcudef.h"

#define io_write_sfr(this, addr, value) (*sfr_write[addr - GPR_SIZE])(this, value);
#define io_read_sfr(this, addr) (*sfr_read[addr - GPR_SIZE])(this);

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

#endif
