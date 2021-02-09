/* System Flash Header */

#ifndef FLASH_H
#define FLASH_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _progmem progmem_t;
typedef struct _report report_t;

typedef struct _flash {

    int pc;
    
    progmem_t *memory;
    unsigned int mem_usage;
    
} flash_t;

/* FLASH Constructor + Destructor */

extern struct _flash* flash_ctor(const report_t *report);
extern void flash_dtor(struct _flash *this);

/* General FLASH Functions */

extern progmem_t* flash_fetch(const struct _flash *this);
extern progmem_t* flash_read_progmem(const struct _flash *this, const int addr);

extern uint16_t flash_read(const struct _flash *this, const int addr);

extern void flash_move_pc(struct _flash *this, const int inc);
extern void flash_set_pc(struct _flash *this, const int addr);
extern int flash_get_pc(const struct _flash *this);

extern void flash_reboot(struct _flash *this);

#endif
