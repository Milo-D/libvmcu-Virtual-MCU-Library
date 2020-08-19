/* (G)eneral (P)urpose (R)egisters Header */

#ifndef GPR_H
#define GPR_H

// C Headers
#include <inttypes.h>

typedef struct _array array_t;
struct _private;

typedef struct _gpr {

    struct _private *p;
    
} gpr_t;

extern struct _gpr* gpr_ctor(void);                                              // GPR ctor
extern void gpr_dtor(struct _gpr *this);                                         // GPR dtor

extern void gpr_write(struct _gpr *this, const int rx, const int8_t data);       // write to GPR File
extern int8_t gpr_read(const struct _gpr *this, const int rx);                   // read from GPR File

extern void gpr_coi(const struct _gpr *this, array_t *buffer);                   // get GPR coi
extern void gpr_dump(const struct _gpr *this, array_t *buffer);                  // dump GPR File to buffer

#endif
