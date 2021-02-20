/* IRQ Header */

#ifndef VMCU_IRQ_H
#define VMCU_IRQ_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_request {

    uint16_t isr;
    struct vmcu_request *next;

} vmcu_request_t;

typedef struct vmcu_irq {

    int size;
    vmcu_request_t *head;

} vmcu_irq_t;

extern vmcu_irq_t* vmcu_irq_ctor(void);
extern void vmcu_irq_dtor(vmcu_irq_t *this);

extern int vmcu_irq_enable(vmcu_irq_t *this, const uint16_t isr_vect);
extern int vmcu_irq_disable(vmcu_irq_t *this, const uint16_t isr_vect);

extern int vmcu_irq_pop(vmcu_irq_t *this, uint16_t *buffer);
extern int vmcu_irq_contains(const vmcu_irq_t *this, const uint16_t isr_vect);

extern void vmcu_irq_reboot(vmcu_irq_t *this);

#endif