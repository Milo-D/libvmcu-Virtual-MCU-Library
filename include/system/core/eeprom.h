/* EEPROM Header */

#ifndef EEPROM_H
#define EEPROM_H

// C Headers
#include <inttypes.h>

typedef struct _tuple tuple_t;

typedef struct _eeprom {

    int8_t *memory;
    tuple_t *coi;

} eeprom_t;

extern struct _eeprom* eeprom_ctor(void);
extern void eeprom_dtor(struct _eeprom *this);

extern void eeprom_write(struct _eeprom *this, const uint16_t addr, const int8_t value);
extern int8_t eeprom_read(const struct _eeprom *this, const uint16_t addr);

extern void eeprom_coi(const struct _eeprom *this, tuple_t *buffer);
extern int8_t* eeprom_dump(const struct _eeprom *this);

extern void eeprom_reboot(const struct _eeprom *this);

#endif

