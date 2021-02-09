/* System Header */

#ifndef SYSTEM_H
#define SYSTEM_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

// Project Headers (engine) (exposed)
#include "engine/include/system/util/progmem.h"
#include "engine/include/system/util/memprop.h"
#include "engine/include/system/mcudef.h"

typedef struct _gpr gpr_t;
typedef struct _sreg sreg_t;
typedef struct _flash flash_t;
typedef struct _data data_t;

typedef struct _array array_t;
typedef struct _tuple tuple_t;

typedef struct _report report_t;

typedef struct _system {
    
    gpr_t *gpr;
    sreg_t *sreg;

    flash_t *flash;
    data_t *data;

    uint64_t cycles;
    uint32_t clock;
    
    int steps;

} system_t;

/* System Constructor + Destructor */

extern struct _system* sys_ctor(const report_t *report);
extern void sys_dtor(struct _system *this);

/* General System Functions */

extern int sys_step(struct _system *this);
extern void sys_backstep(struct _system *this);
extern void sys_reboot(struct _system *this);

/* GPR Operations */

extern void sys_write_gpr(struct _system *this, const int rx, const int8_t data);
extern int8_t sys_read_gpr(const struct _system *this, const int rx);
extern void sys_gpr_coi(const struct _system *this, array_t *buffer);
extern int8_t* sys_dump_gpr(const struct _system *this);

/* SREG Operations */

extern void sys_write_sreg(struct _system *this, const int flag, const bool bit);
extern bool sys_read_sreg(const struct _system *this, const int flag);
extern void sys_sreg_coi(const struct _system *this, array_t *buffer);
extern uint8_t sys_dump_sreg(const struct _system *this);

/* FLASH Operations */

extern progmem_t* sys_read_progmem(const struct _system *this, const int addr);
extern uint16_t sys_read_flash(const struct _system *this, const int addr);
extern void sys_move_pc(const struct _system *this, const int inc);
extern void sys_set_pc(struct _system *this, const int addr);
extern int sys_get_pc(const struct _system *this);

/* DATA Operations */

extern void sys_push_stack(struct _system *this, const int8_t value);
extern int8_t sys_pop_stack(const struct _system *this);
extern void sys_write_data(struct _system *this, const uint16_t addr, const int8_t value);
extern int8_t sys_read_data(const struct _system *this, const uint16_t addr);
extern void sys_data_coi(const struct _system *this, tuple_t *buffer);
extern int8_t* sys_dump_data(const struct _system *this);

/* IO/SFR Operations */

extern void sys_set_sfr(struct _system *this, const uint16_t addr, const int bit);
extern void sys_clear_sfr(struct _system *this, const uint16_t addr, const int bit);

/* EEPROM Operations */

extern int8_t* sys_dump_eeprom(const struct _system *this);

#endif