/* 8-bit Timer Implementation */

// C Headers
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// Project Headers
#include "system/peripherals/timer8.h"
#include "system/mcudef.h"

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

/* --- Extern --- */

struct _timer8* timer8_ctor(const TCX timer_id, int8_t *memory) {

    struct _timer8 *timer;

    if((timer = malloc(sizeof(struct _timer8))) == NULL)
        return NULL;

    switch(timer_id) {

        case TC0:

            timer->tcnt = &memory[TCNT0];
            timer->tccr = &memory[TCCR0];
            timer->tifr = &memory[TIFR];
            timer->ocr  = &memory[OCR0];

            timer->tov = TOV0;
            timer->ocf = OCF0;

        break;

        case TC2: 

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

void timer8_tick(struct _timer8 *this, const uint32_t cpu_clk, const uint64_t dc) {

    double tclk;

    if((tclk = prescale(cpu_clk, *(this->tccr))) == 0)
        return;

    const double dtc = (((dc * 1.0) / (cpu_clk * 1.0)) * tclk) + this->borrow;
    this->borrow = dtc - ((long) dtc);

    const uint16_t value = *(this->tcnt);
    *(this->tcnt) += dtc;

    if((value + dtc) > 0xff)
        *(this->tifr) |= (0x01 << this->tov);
}

/* --- Static --- */

static double prescale(const uint32_t clock, const uint8_t tccr) {

    /* Clock Source (CSx2, CSx1, CSx0) */

    switch(tccr & 0x07) {

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
