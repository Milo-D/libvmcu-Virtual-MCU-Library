/* (G)eneral (P)urpose (R)egisters Header */

#ifndef GPR_H
#define GPR_H

// C Headers
#include <inttypes.h>

// Project Headers
#include "system/util/memprop.h"

typedef struct _array array_t;

typedef struct _gpr {

    int8_t *regfile;
    MEMPROP *coi;
    
} gpr_t;

extern struct _gpr* gpr_ctor(void);
extern void gpr_dtor(struct _gpr *this);

extern void gpr_write(struct _gpr *this, const int rx, const int8_t data);
extern int8_t gpr_read(const struct _gpr *this, const int rx);

extern void gpr_coi(const struct _gpr *this, array_t *buffer);
extern int8_t* gpr_dump(const struct _gpr *this);

extern void gpr_reboot(const struct _gpr *this);

#endif
