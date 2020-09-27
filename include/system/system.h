/* System Header */

#ifndef SYSTEM_H
#define SYSTEM_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

// Project Headers
#include "table/entry.h"

typedef struct _table table_t;
typedef struct _plain plain_t;
typedef struct _array array_t;
typedef struct _tuple tuple_t;

struct _private;

typedef struct _system {

    uint64_t cycles;
    uint32_t clock;

    struct _private *p;

} system_t;

/* System Constructor + Destructor */

extern struct _system* sys_ctor(const char *file);
extern void sys_dtor(struct _system *this);

/* General System Functions */

extern int sys_step(struct _system *this);
extern void sys_backstep(struct _system *this);
extern void sys_reboot(struct _system *this);
extern void sys_kill(const struct _system *this);
extern bool sys_is_term(const struct _system *this);

/* GPR Operations */

extern void sys_write_gpr(struct _system *this, const int rx, const int8_t data);
extern int8_t sys_read_gpr(const struct _system *this, const int rx);
extern void sys_gpr_coi(const struct _system *this, array_t *buffer);
extern void sys_dump_gpr(const struct _system *this, array_t *buffer);

/* SREG Operations */

extern void sys_write_sreg(struct _system *this, const int flag, const bool bit);
extern bool sys_read_sreg(const struct _system *this, const int flag);
extern void sys_sreg_coi(const struct _system *this, array_t *buffer);
extern uint8_t sys_dump_sreg(const struct _system *this);

/* FLASH Operations */

extern plain_t* sys_read_instr(const struct _system *this, const int addr);
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
extern void sys_dump_data(const struct _system *this, array_t *buffer);

/* IO Operations */

extern void sys_update_io(const struct _system *this, const uint64_t oldc);

/* EEPROM Operations */

extern void sys_write_eeprom(struct _system *this, const uint16_t addr, const int8_t value);
extern int8_t sys_read_eeprom(const struct _system *this, const uint16_t addr);
extern void sys_eeprom_coi(const struct _system *this, tuple_t *buffer);
extern void sys_dump_eeprom(const struct _system *this, array_t *buffer);

/* Table Operations */

extern int sys_add_breakp(const struct _system *this, const char *point);
extern int sys_del_breakp(const struct _system *this, const char *point);
extern bool sys_on_breakp(const struct _system *this);
extern int sys_table_size(const struct _system *this);
extern entry_t* sys_dump_table(const struct _system *this);

#endif
