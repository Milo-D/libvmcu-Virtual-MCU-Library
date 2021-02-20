/* System Flash Header */

#ifndef VMCU_FLASH_H
#define VMCU_FLASH_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct vmcu_progmem vmcu_progmem_t;
typedef struct vmcu_report vmcu_report_t;

typedef struct vmcu_flash {

    int pc;
    vmcu_progmem_t *memory;
    
} vmcu_flash_t;

/* FLASH Constructor + Destructor */

extern vmcu_flash_t* vmcu_flash_ctor(const vmcu_report_t *report);
extern void vmcu_flash_dtor(vmcu_flash_t *this);

/* General FLASH Functions */

extern vmcu_progmem_t* vmcu_flash_fetch(const vmcu_flash_t *this);
extern vmcu_progmem_t* vmcu_flash_read_progmem(const vmcu_flash_t *this, const int addr);

extern uint16_t vmcu_flash_read(const vmcu_flash_t *this, const int addr);

extern void vmcu_flash_move_pc(vmcu_flash_t *this, const int inc);
extern void vmcu_flash_set_pc(vmcu_flash_t *this, const int addr);
extern int vmcu_flash_get_pc(const vmcu_flash_t *this);

extern void vmcu_flash_reboot(vmcu_flash_t *this);

#endif
