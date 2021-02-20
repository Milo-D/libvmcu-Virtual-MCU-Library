/* 8-bit Timer Header */

#ifndef VMCU_TIMER8_H
#define VMCU_TIMER8_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct vmcu_irq vmcu_irq_t;
typedef enum { VMCU_TC0, VMCU_TC2 } VMCU_TCX;

typedef struct vmcu_timer8 {

    uint8_t *tcnt;			/* Timer/Counter */
    
    uint8_t *tccra;			/* Control Register A */
    uint8_t *tccrb;         /* Control Register B */
    
    uint8_t *tifr;			/* Timer Interrupt Flags */
    uint8_t *timsk;			/* Timer Interrupt Mask */
    
    uint8_t *ocra;			/* Output Compare Register (A) */
    uint8_t *ocrb;          /* Output Compare Register (B) */

    uint8_t *ocpa;			/* Output Compare Pin (A) */
    uint8_t *ocpb;          /* Output Compare Pin (B) */
    
    uint8_t *ddrxa;			/* Data Direction Register (OCxA) */
    uint8_t *ddrxb;         /* Data Direction Register (OCxB) */

    uint8_t oca; 			/* Output Compare Pin (A) (Bitpos.) */
    uint8_t ocb;            /* Output Compare Pin (B) (Bitpos.) */

    uint16_t prescaler;		/* Clocksource's Prescaler */
    uint16_t countdown;		/* Timer Countdown */

} vmcu_timer8_t;

extern vmcu_timer8_t* vmcu_timer8_ctor(const VMCU_TCX timer_id, int8_t *memory);
extern void vmcu_timer8_dtor(vmcu_timer8_t *this);

extern void vmcu_timer8_update(vmcu_timer8_t *this, vmcu_irq_t *irq, uint64_t dc);
extern void vmcu_timer8_update_prescaler(vmcu_timer8_t *this);

extern void vmcu_timer8_force_ocpa(vmcu_timer8_t *this);
extern void vmcu_timer8_force_ocpb(vmcu_timer8_t *this);

extern bool vmcu_timer8_is_busy(vmcu_timer8_t *this);
extern void vmcu_timer8_reboot(vmcu_timer8_t *this);

#endif
