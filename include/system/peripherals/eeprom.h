/* EEPROM Header */

#ifndef EEPROM_H
#define EEPROM_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _irq irq_t;

typedef struct _eeprom {

    uint8_t *eecr;          /* EEPROM Control Register */
    uint8_t *spmcsr;        /* Store Program Memory Control Register */

    uint8_t *eedr;          /* EEPROM Data Register */
    uint8_t *eearl;         /* EEPROM Address Register (LOW) */
    uint8_t *eearh;         /* EEPROM Address Register (HIGH) */

    int8_t *memory;         /* EEPROM Memory */
    
    uint8_t data;           /* EEPROM Data Storage */
    uint16_t addr;          /* EEPROM Address Storage */

    int wapc;               /* Write Access Progress Counter */
    double cycle;           /* EEPROM Cycle Counter */
    
    bool mw_enabled;        /* Master Write Status */
    bool mw_lock;           /* Master Write Tick Enabled */
    int8_t mw_counter;      /* Master Write Counter */
    
} eeprom_t;

extern struct _eeprom* eeprom_ctor(int8_t *memory);
extern void eeprom_dtor(struct _eeprom *this);

extern void eeprom_update(struct _eeprom *this, irq_t *irq, const uint32_t cpu_clk, const uint64_t dc);
extern void eeprom_enable_write(struct _eeprom *this);

extern void eeprom_try_read(struct _eeprom *this);
extern int eeprom_try_write(struct _eeprom *this);

extern bool eeprom_is_busy(struct _eeprom *this);
extern int8_t* eeprom_dump(const struct _eeprom *this);

extern void eeprom_reboot(struct _eeprom *this);

#endif
