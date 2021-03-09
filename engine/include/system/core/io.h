/* IO Memory Header */

#ifndef VMCU_IO_H
#define VMCU_IO_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/system/core/sfreg.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"

#define vmcu_io_set_sfr(this, addr, bit) (*vmcu_sfr_set[addr - GPR_SIZE])(this, bit)
#define vmcu_io_clear_sfr(this, addr, bit) (*vmcu_sfr_clear[addr - GPR_SIZE])(this, bit)

#define vmcu_io_write_sfr(this, addr, value) (*vmcu_sfr_write[addr - GPR_SIZE])(this, value)
#define vmcu_io_read_sfr(this, addr) (*vmcu_sfr_read[addr - GPR_SIZE])(this)

typedef struct vmcu_irq vmcu_irq_t;

typedef struct vmcu_timer8 vmcu_timer8_t;
typedef struct vmcu_eeprom vmcu_eeprom_t;

typedef struct vmcu_io {

    /* IRQ-Handler */
    vmcu_irq_t *irq;

    /* Peripherals */
    vmcu_timer8_t *timer0;
    vmcu_eeprom_t *eeprom;

    /* IO Memory */
    int8_t *memory;

} vmcu_io_t;

extern vmcu_io_t* vmcu_io_ctor(int8_t *io_start);
extern void vmcu_io_dtor(vmcu_io_t *this);

/* IO Operations */

extern void vmcu_io_update(vmcu_io_t *this, const uint32_t cpu_clk, const uint64_t dc);
extern int vmcu_io_check_irq(const vmcu_io_t *this);
extern void vmcu_io_reboot(const vmcu_io_t *this);

/* EEPROM Operations */

extern int8_t* vmcu_io_dump_eeprom(const vmcu_io_t *this);

#endif
