/* Status Register Header */

#ifndef VMCU_SREG_H
#define VMCU_SREG_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct vmcu_sreg {

    uint8_t status;

} vmcu_sreg_t;

extern vmcu_sreg_t* vmcu_sreg_ctor(void);
extern void vmcu_sreg_dtor(vmcu_sreg_t *this);

extern void vmcu_sreg_write(vmcu_sreg_t *this, const int flag, const bool bit);
extern bool vmcu_sreg_read(const vmcu_sreg_t *this, const int flag);

extern void vmcu_sreg_write_byte(vmcu_sreg_t *this, const uint8_t byte);
extern uint8_t vmcu_sreg_read_byte(vmcu_sreg_t *this);

extern void vmcu_sreg_clear(vmcu_sreg_t *this);

extern uint8_t vmcu_sreg_dump(const vmcu_sreg_t *this);
extern void vmcu_sreg_reboot(vmcu_sreg_t *this);

#endif
