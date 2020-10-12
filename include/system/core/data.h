/* AVR Data Memory Header */

#ifndef DATA_H
#define DATA_H

// C Headers
#include <inttypes.h>

typedef struct _tuple tuple_t;

struct _private;

typedef struct _data {
    
    int size;
    struct _private *p;
    
} data_t;

extern struct _data* data_ctor(void);
extern void data_dtor(struct _data *this);

/* General DATA Operations */

extern void data_push(struct _data *this, const int8_t value);
extern int8_t data_pop(const struct _data *this);
extern void data_write(struct _data *this, const uint16_t addr, const int8_t value);
extern int8_t data_read(const struct _data *this, const uint16_t addr);
extern void data_coi(const struct _data *this, tuple_t *buffer);
extern int8_t* data_dump(const struct _data *this);
extern void data_reboot(const struct _data *this);

/* IO Operations */

extern void data_update_io(const struct _data *this);
extern int data_check_irq(const struct _data *this);

#endif
