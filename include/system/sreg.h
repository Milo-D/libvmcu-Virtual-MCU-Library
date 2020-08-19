/* Status Register Header */

#ifndef SREG_H
#define SREG_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _array array_t;
struct _private;

typedef struct _sreg {

    struct _private *p;

} sreg_t;

extern struct _sreg* sreg_ctor(void);                                           // SREG ctor
extern void sreg_dtor(struct _sreg *this);                                      // SREG dtor

extern void sreg_write(struct _sreg *this, const int flag, const bool bit);     // write to SREG
extern bool sreg_read(const struct _sreg *this, const int flag);                // read from SREG
extern void sreg_clr(struct _sreg *this);                                       // clear SREG

extern void sreg_coi(const struct _sreg *this, array_t *buffer);                // get SREG coi
extern uint8_t sreg_dump(const struct _sreg *this);                             // dump SREG

#endif
