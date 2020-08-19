/* AVR Flash Header */

#ifndef FLASH_H
#define FLASH_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _table table_t;
typedef struct _tuple tuple_t;

struct _private;

typedef struct _flash {
    
    struct _private *p;
    
} flash_t;

extern struct _flash* flash_ctor(table_t *table);
extern void flash_dtor(struct _flash *this);

extern int flash_fetch(const struct _flash *this, tuple_t *buffer);

extern int flash_pc_next(struct _flash *this);
extern int flash_pc_set(struct _flash *this, const int addr);
extern int flash_pc(const struct _flash *this);

extern int flash_table_step(const struct _flash *this);
extern bool flash_table_is_sync(const struct _flash *this);

#endif
