/* Status Register Header */

#ifndef SREG_H
#define SREG_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _array array_t;

typedef struct _sreg {

    uint8_t status;
    int *coi;

} sreg_t;

extern struct _sreg* sreg_ctor(void);
extern void sreg_dtor(struct _sreg *this);

extern void sreg_write(struct _sreg *this, const int flag, const bool bit);
extern bool sreg_read(const struct _sreg *this, const int flag);

extern void sreg_clear(struct _sreg *this);

extern void sreg_coi(const struct _sreg *this, array_t *buffer);
extern uint8_t sreg_dump(const struct _sreg *this);

extern void sreg_reboot(struct _sreg *this);

#endif
