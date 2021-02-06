/* IRQ Header */

#ifndef IRQ_H
#define IRQ_H

// C Headers
#include <inttypes.h>

typedef struct _request {

    uint16_t isr;
    struct _request *next;

} request_t;

typedef struct _irq {

    int size;
    struct _request *head;

} irq_t;

extern struct _irq* irq_ctor(void);
extern void irq_dtor(struct _irq *this);

extern int irq_enable(struct _irq *this, const uint16_t isr_vect);
extern int irq_disable(struct _irq *this, const uint16_t isr_vect);

extern int irq_pop(struct _irq *this, uint16_t *buffer);
extern int irq_contains(const struct _irq *this, const uint16_t isr_vect);

extern void irq_reboot(struct _irq *this);

#endif