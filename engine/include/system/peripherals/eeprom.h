/* EEPROM Header */

#ifndef VMCU_EEPROM_H
#define VMCU_EEPROM_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct vmcu_irq vmcu_irq_t;

typedef struct vmcu_eeprom {

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
    
} vmcu_eeprom_t;

extern vmcu_eeprom_t* vmcu_eeprom_ctor(int8_t *memory);
extern void vmcu_eeprom_dtor(vmcu_eeprom_t *this);

extern void vmcu_eeprom_update(vmcu_eeprom_t *this, vmcu_irq_t *irq, uint32_t cpu_clk, uint64_t dc);
extern void vmcu_eeprom_enable_write(vmcu_eeprom_t *this);

extern void vmcu_eeprom_try_read(vmcu_eeprom_t *this);
extern int vmcu_eeprom_try_write(vmcu_eeprom_t *this);

extern bool vmcu_eeprom_is_busy(vmcu_eeprom_t *this);
extern int8_t* vmcu_eeprom_dump(const vmcu_eeprom_t *this);

extern void vmcu_eeprom_reboot(vmcu_eeprom_t *this);

#endif
