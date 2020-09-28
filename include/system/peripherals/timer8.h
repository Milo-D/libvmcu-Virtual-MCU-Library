/* 8-bit Timer Header */

#ifndef TIMER8_H
#define TIMER8_H

// C Headers
#include <inttypes.h>

typedef struct _irq irq_t;
typedef enum { TC0, TC2 } TCX;

typedef struct _timer8 {

    uint8_t *tcnt;
    uint8_t *tccr;
    uint8_t *tifr;
    uint8_t *ocr;
    uint8_t *timsk;

    uint8_t tov;
    uint8_t ocf;

    double borrow;

} timer8_t;

extern struct _timer8* timer8_ctor(const TCX timer_id, int8_t *memory);
extern void timer8_dtor(struct _timer8 *this);

extern void timer8_tick(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt);
extern void timer8_reboot(struct _timer8 *this);

#endif