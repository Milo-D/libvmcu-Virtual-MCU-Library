/* (G)eneral (P)urpose (R)egisters Header */

#ifndef VMCU_GPR_H
#define VMCU_GPR_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_gpr {

    int8_t *regfile;
    
} vmcu_gpr_t;

extern vmcu_gpr_t* vmcu_gpr_ctor(void);
extern void vmcu_gpr_dtor(vmcu_gpr_t *this);

extern void vmcu_gpr_write(vmcu_gpr_t *this, const int rx, const int8_t data);
extern int8_t vmcu_gpr_read(const vmcu_gpr_t *this, const int rx);

extern int8_t* vmcu_gpr_dump(const vmcu_gpr_t *this);
extern void vmcu_gpr_reboot(const vmcu_gpr_t *this);

#endif
