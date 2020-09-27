/* IO Memory Header */

#ifndef IO_H
#define IO_H

// C Headers
#include <inttypes.h>

struct _private;

typedef struct _io {

    struct _private *p;

} io_t;

extern struct _io* io_ctor(int8_t *io_start);
extern void io_dtor(struct _io *this);

extern void io_update(struct _io *this, const uint32_t clock, const uint64_t dc);
extern void io_reboot(const struct _io *this);

#endif