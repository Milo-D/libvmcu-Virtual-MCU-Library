/* AVR ALU Header */

#ifndef ALU_H
#define ALU_H

// C Headers
#include <inttypes.h>

typedef struct _flash flash_t;
typedef struct _gpr gpr_t;
typedef struct _sreg sregt_t;
typedef struct _system system_t;
typedef struct _table table_t;
typedef struct _entry entry_t;
typedef struct _array array_t;

struct _private;

typedef struct _alu {

    struct _private *p;

} alu_t;

/* ALU Constructor + Destructor */

extern struct _alu* alu_ctor(const char *file);
extern void alu_dtor(struct _alu *this);

/* General ALU Functions */

extern int alu_fetch(struct _alu *this, system_t *sys);
extern void alu_reboot(const struct _alu *this);

/* FLASH Operations */

extern int alu_get_pc(const struct _alu *this);
extern void alu_set_pc(struct _alu *this, const int addr);

/* GPR Operations */

extern void alu_write_gpr(struct _alu *this, const int rx, const int8_t data);
extern int8_t alu_read_gpr(const struct _alu *this, const int rx);
extern void alu_gpr_coi(const struct _alu *this, array_t *buffer);
extern void alu_dump_gpr(const struct _alu *this, array_t *buffer);

/* SREG Operations */

extern void alu_write_sreg(struct _alu *this, const int flag, const bool bit);
extern bool alu_read_sreg(const struct _alu *this, const int flag);
extern void alu_sreg_coi(const struct _alu *this, array_t *buffer);
extern uint8_t alu_dump_sreg(const struct _alu *this);

/* Table Operations */

extern int alu_add_breakp(const struct _alu *this, const char *point);
extern int alu_del_breakp(const struct _alu *this, const char *point);
extern bool alu_on_breakp(const struct _alu *this);
extern int alu_table_size(const struct _alu *this);
extern entry_t* alu_dump_table(const struct _alu *this);

#endif
