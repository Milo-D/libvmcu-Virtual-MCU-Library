/* 8-bit Timer Implementation */

// C Headers
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// Project Headers
#include "system/peripherals/timer8.h"
#include "system/core/irq.h"
#include "system/mcudef.h"

#define NMOD 4

/*
*
*   c   := cycles
*   t   := time
*   tx  := timerx
*   clk := frequency
*   ∆tc := counter change
*
*   --- Elapsed time ---
*   t = (c / clk(cpu)) [s]
*
*   --- Elapsed time between steps ---
*   t(1) - t(0) = (c(1) / clk(cpu)) - (c(0) / clk(cpu)) [s] 
* = ∆t = (∆c / clk(cpu)) [s]
*
*   --- ∆tc between steps --- 
*   clk(tx) * (∆c / clk(cpu))
* = (clk(tx) / clk(cpu)) * ∆c
* = (clk(tx) / clk(cpu)) * (c(1) - c(0))
*
*/

/* Forward Declarations of static Functions */

static double prescale(const uint32_t clock, const uint8_t tccr);
static void timer8_tick_normal(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt);
static void timer8_tick_ctc(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt);

/* Forward Declarations of static Members */

static void (*tick[NMOD]) (struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt);

/* --- Extern --- */

struct _timer8* timer8_ctor(const TCX timer_id, int8_t *memory) {

    struct _timer8 *timer;

    if((timer = malloc(sizeof(struct _timer8))) == NULL)
        return NULL;

    switch(timer_id) {

        case TC0:

            timer->timsk = &memory[TIMSK];

            timer->tcnt = &memory[TCNT0];
            timer->tccr = &memory[TCCR0];
            timer->tifr = &memory[TIFR];
            timer->ocr  = &memory[OCR0];

            timer->tov = TOV0;
            timer->ocf = OCF0;

        break;

        case TC2: 

            timer->timsk = &memory[TIMSK];

            timer->tcnt = &memory[TCNT2];
            timer->tccr = &memory[TCCR2];
            timer->tifr = &memory[TIFR];
            timer->ocr  = &memory[OCR2];

            timer->tov = TOV2;
            timer->ocf = OCF2;

        break;

        default: free(timer); return NULL;
    };

    timer->borrow = 0.00;
    return timer;
}

void timer8_dtor(struct _timer8 *this) {

    free(this);
}

void timer8_tick(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt) {
 
    const int mode = wgmtc8(*(this->tccr));
    (*tick[mode])(this, irq, cpu_clk, dt);
}

void timer8_reboot(struct _timer8 *this) {

    this->borrow = 0.00;
}

/* --- Static --- */

static double prescale(const uint32_t clock, const uint8_t tccr) {

    /* Clock Source (CSx2, CSx1, CSx0) */

    switch(tccr & CSX_MSK) {

        case 0x00: return 0;
        case 0x01: return clock;

        case 0x02: return (clock / 8.0);
        case 0x03: return (clock / 64.0);
        case 0x04: return (clock / 256.0);
        case 0x05: return (clock / 1024.0);

        case 0x06: return 0; /* not supported */
        case 0x07: return 0; /* not supported */

        default: /* not possible */ break;
    }

    return 0;
}

static void timer8_tick_normal(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt) {

    const double tclk = prescale(cpu_clk, *(this->tccr));
    const double dtc = ((dt * tclk) + this->borrow);

    const uint8_t tcnt_prev = *(this->tcnt);

    *(this->tcnt) += (uint8_t) dtc;
    this->borrow = dtc - ((long) dtc);

    if(*(this->tcnt) < tcnt_prev) {

        *(this->tifr) |= (0x01 << this->tov);

        if(((0x01 << this->tov) & *(this->timsk)))
            irq_enable(irq, OVF0_VECT);
    }
}

static void timer8_tick_ctc(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt) {

    const double tclk = prescale(cpu_clk, *(this->tccr));
    const double dtc = ((dt * tclk) + this->borrow);

    *(this->tcnt) += (uint8_t) dtc;
    this->borrow = dtc - ((long) dtc);

    if(*(this->tcnt) >= *(this->ocr)) {

        *(this->tifr) |= (0x01 << this->ocf);

        if(((0x01 << this->ocf) & *(this->timsk)))
            irq_enable(irq, OC0_VECT);

        *(this->tcnt) = 0;
    }
}

static void timer8_tick_pwm_correct(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt) {

    /* in progress */
    return;
}

static void timer8_tick_pwm_fast(struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt) {

    /* in progress */
    return;
}

static void (*tick[NMOD]) (struct _timer8 *this, irq_t *irq, const uint32_t cpu_clk, const double dt) = {

    timer8_tick_normal,
    timer8_tick_ctc,
    timer8_tick_pwm_correct,
    timer8_tick_pwm_fast
};
