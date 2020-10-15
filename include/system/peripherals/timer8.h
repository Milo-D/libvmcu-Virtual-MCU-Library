/* 8-bit Timer Header */

#ifndef TIMER8_H
#define TIMER8_H

// C Headers
#include <inttypes.h>

typedef struct _irq irq_t;
typedef enum { TC0, TC2 } TCX;

typedef struct _timer8 {

    uint8_t *tcnt;			/* Timer/Counter */
    uint8_t *tccr;			/* Control Register */
    uint8_t *tifr;			/* Timer Interrupt Flags */
    uint8_t *timsk;			/* Timer Interrupt Mask */
    uint8_t *ocr;			/* Output Compare Register */

    uint8_t *oc;			/* Output Compare Pin */
    uint8_t *ddr;			/* Data Direction Register (OC) */

    uint8_t tov;			/* Timer Overflow Flag (Bitposition) */
    uint8_t ocf;			/* Output Compare Flag (Bitposition) */
    uint8_t ocx; 			/* Output Compare Pin  (Bitposition) */

    uint16_t prescaler;		/* Clocksource's Prescaler */
    uint16_t countdown;		/* Timer Countdown */

} timer8_t;

extern struct _timer8* timer8_ctor(const TCX timer_id, int8_t *memory);
extern void timer8_dtor(struct _timer8 *this);

extern void timer8_tick(struct _timer8 *this, irq_t *irq, const uint64_t dc);
extern void timer8_reboot(struct _timer8 *this);

#endif