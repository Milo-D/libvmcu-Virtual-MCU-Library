/* System Header */

#ifndef VMCU_SYSTEM_H
#define VMCU_SYSTEM_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

// Project Headers (engine) (exposed)
#include "engine/include/system/util/progmem.h"

typedef struct vmcu_gpr vmcu_gpr_t;
typedef struct vmcu_sreg vmcu_sreg_t;
typedef struct vmcu_flash vmcu_flash_t;
typedef struct vmcu_data vmcu_data_t;

typedef struct vmcu_report vmcu_report_t;

typedef struct vmcu_system {
    
    vmcu_gpr_t *gpr;
    vmcu_sreg_t *sreg;

    vmcu_flash_t *flash;
    vmcu_data_t *data;

    uint64_t cycles;
    uint32_t clock;
    
    int steps;

} vmcu_system_t;

/* System Constructor + Destructor */

extern vmcu_system_t* vmcu_system_ctor(const vmcu_report_t *report);
extern void vmcu_system_dtor(vmcu_system_t *this);

/* General System Functions */

extern int vmcu_system_step(vmcu_system_t *this);
extern void vmcu_system_backstep(vmcu_system_t *this);
extern void vmcu_system_reboot(vmcu_system_t *this);

/* GPR Operations */

extern void vmcu_system_write_gpr(vmcu_system_t *this, const int rx, const int8_t data);
extern int8_t vmcu_system_read_gpr(const vmcu_system_t *this, const int rx);
extern int8_t* vmcu_system_dump_gpr(const vmcu_system_t *this);

/* SREG Operations */

extern void vmcu_system_write_sreg(vmcu_system_t *this, const int flag, const bool bit);
extern bool vmcu_system_read_sreg(const vmcu_system_t *this, const int flag);
extern uint8_t vmcu_system_dump_sreg(const vmcu_system_t *this);

/* FLASH Operations */

extern vmcu_progmem_t* vmcu_system_read_progmem(const vmcu_system_t *this, const int addr);
extern uint16_t vmcu_system_read_flash(const vmcu_system_t *this, const int addr);
extern void vmcu_system_move_pc(const vmcu_system_t *this, const int inc);
extern void vmcu_system_set_pc(vmcu_system_t *this, const int addr);
extern int vmcu_system_get_pc(const vmcu_system_t *this);

/* DATA Operations */

extern void vmcu_system_push_stack(vmcu_system_t *this, const int8_t value);
extern int8_t vmcu_system_pop_stack(const vmcu_system_t *this);
extern void vmcu_system_write_data(vmcu_system_t *this, const uint16_t addr, const int8_t value);
extern int8_t vmcu_system_read_data(const vmcu_system_t *this, const uint16_t addr);
extern int8_t* vmcu_system_dump_data(const vmcu_system_t *this);

/* IO/SFR Operations */

extern void vmcu_system_set_sfr(vmcu_system_t *this, const uint16_t addr, const int bit);
extern void vmcu_system_clear_sfr(vmcu_system_t *this, const uint16_t addr, const int bit);

/* EEPROM Operations */

extern int8_t* vmcu_system_dump_eeprom(const vmcu_system_t *this);

#endif
