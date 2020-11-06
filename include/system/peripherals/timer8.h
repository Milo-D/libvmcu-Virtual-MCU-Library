/* 8-bit Timer Header */

#ifndef TIMER8_H
#define TIMER8_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

typedef struct _irq irq_t;
typedef enum { TC0, TC2 } TCX;

typedef struct _timer8 {

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

} timer8_t;

extern struct _timer8* timer8_ctor(const TCX timer_id, int8_t *memory);
extern void timer8_dtor(struct _timer8 *this);

extern void timer8_tick(struct _timer8 *this, irq_t *irq, const uint64_t dc);

extern void timer8_force_ocpa(struct _timer8 *this);
extern void timer8_force_ocpb(struct _timer8 *this);

extern bool timer8_is_busy(struct _timer8 *this);
extern void timer8_reboot(struct _timer8 *this);

#endif
