/* 8-bit Timer Implementation */

// C Headers
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// Project Headers
#include "system/peripherals/timer8.h"
#include "system/core/irq.h"
#include "system/mcudef.h"
#include "misc/bitmanip.h"

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

static uint16_t prescale(const uint8_t tccr);

static void timer8_tick_normal(struct _timer8 *this, irq_t *irq);
static void timer8_tick_ctc(struct _timer8 *this, irq_t *irq);
static void timer8_tick_pwm_correct(struct _timer8 *this, irq_t *irq);
static void timer8_tick_pwm_fast(struct _timer8 *this, irq_t *irq);

static void trigger_ocpa(struct _timer8 *this);
static void trigger_ocpb(struct _timer8 *this);

static void trigger_ocpa_pwm_fast(struct _timer8 *this);
static void trigger_ocpb_pwm_fast(struct _timer8 *this);

static void trigger_ocpa_pwm_correct(struct _timer8 *this);
static void trigger_ocpb_pwm_correct(struct _timer8 *this);

/* Forward Declarations of static Members */

static void (*tick[NMODT8]) (struct _timer8 *this, irq_t *irq);

/* --- Extern --- */

struct _timer8* timer8_ctor(const TCX timer_id, int8_t *memory) {

    struct _timer8 *timer;

    if((timer = malloc(sizeof(struct _timer8))) == NULL)
        return NULL;

    switch(timer_id) {

        case TC0:

            timer->timsk = &memory[TIMSK0];
            timer->tccra = &memory[TCCR0A];
            timer->tccrb = &memory[TCCR0B];

            timer->tcnt  = &memory[TCNT0];
            timer->tifr  = &memory[TIFR0];
            timer->ocra  = &memory[OCR0A];
            timer->ocrb  = &memory[OCR0B];

            timer->ocpa  = &memory[OC0A_P];
            timer->ocpb  = &memory[OC0B_P];
            
            timer->ddrxa = &memory[OC0A_DDR];
            timer->ddrxb = &memory[OC0B_DDR];

            timer->oca   = OC0A;
            timer->ocb   = OC0B;

        break;

        case TC2: 

            timer->timsk = &memory[TIMSK2];
            timer->tccra = &memory[TCCR2A];
            timer->tccrb = &memory[TCCR2B];

            timer->tcnt  = &memory[TCNT2];
            timer->tifr  = &memory[TIFR2];
            timer->ocra  = &memory[OCR2A];
            timer->ocrb  = &memory[OCR2B];

            timer->ocpa  = &memory[OC2A_P];
            timer->ocpb  = &memory[OC2B_P];
            
            timer->ddrxa = &memory[OC2A_DDR];
            timer->ddrxb = &memory[OC2B_DDR];

            timer->oca   = OC2A;
            timer->ocb   = OC2B;

        break;

        default: free(timer); return NULL;
    };

    timer->prescaler = 0;
    timer->countdown = 0;

    return timer;
}

void timer8_dtor(struct _timer8 *this) {

    free(this);
}

void timer8_update(struct _timer8 *this, irq_t *irq, const uint64_t dc) {

    const uint16_t csx = prescale(*(this->tccrb));

    if(csx == 0)
        return;

    if(csx != this->prescaler) {

        this->prescaler = csx;
        this->countdown = csx;
    }

    for(int i = 0; i < dc; i++) {

        if(--(this->countdown) > 0)
            continue;
            
        const uint8_t a = *(this->tccra);
        const uint8_t b = *(this->tccrb);

        (*tick[ wgmtc8(a, b) ])(this, irq);
        this->countdown = this->prescaler;
    }
}

void timer8_force_ocpa(struct _timer8 *this) {

    const uint8_t a = *(this->tccra);
    const uint8_t b = *(this->tccrb);

    switch( wgmtc8(a, b) ) {

        case 0x01: return;
        case 0x03: return;
        case 0x04: return;
        case 0x05: return;
        case 0x06: return;
        case 0x07: return;

        default: /* non-PWM mode */ break;
    }

    trigger_ocpa(this);
}

void timer8_force_ocpb(struct _timer8 *this) {
    
    const uint8_t a = *(this->tccra);
    const uint8_t b = *(this->tccrb);
    
    switch( wgmtc8(a, b) ) {

        case 0x01: return;
        case 0x03: return;
        case 0x04: return;
        case 0x05: return;
        case 0x06: return;
        case 0x07: return;

        default: /* non-PWM mode */ break;
    }
    
    trigger_ocpb(this);
}

bool timer8_is_busy(struct _timer8 *this) {
    
    return ((*(this->tccrb) & CSX_MSK) != 0x00);
}

void timer8_reboot(struct _timer8 *this) {

    this->prescaler = 0;
    this->countdown = 0;
}

/* --- Static --- */

static uint16_t prescale(const uint8_t tccr) {

    /* Clock Source (CSx2, CSx1, CSx0) */

    switch(tccr & CSX_MSK) {

        case 0x00: return 0U;
        case 0x01: return 1U;

        case 0x02: return 8U;
        case 0x03: return 64U;
        case 0x04: return 256U;
        case 0x05: return 1024U;

        case 0x06: return 0U; /* not supported */
        case 0x07: return 0U; /* not supported */

        default: /* not possible */ break;
    }

    return 0U;
}

static void timer8_tick_normal(struct _timer8 *this, irq_t *irq) {

    if(++(*(this->tcnt)) == 0x00) {

        setbit(*(this->tifr), TOV);

        if(bit(*(this->timsk), TOV) == 0x01)
            irq_enable(irq, OVF0_VECT);
    }

    if(*(this->tcnt) == *(this->ocra)) {

        setbit(*(this->tifr), OCFA);

        if(bit(*(this->timsk), OCFA) == 0x01)
            irq_enable(irq, OC0A_VECT); // currently only tc0

        trigger_ocpa(this);
    }
    
    if(*(this->tcnt) == *(this->ocrb)) {
        
        setbit(*(this->tifr), OCFB);
        
        if(bit(*(this->timsk), OCFB) == 0x01)
            irq_enable(irq, OC0B_VECT); // currently only tc0
            
        trigger_ocpb(this);
    }
}

static void timer8_tick_pwm_correct(struct _timer8 *this, irq_t *irq) {

    /* in progress */
    return;
}

static void timer8_tick_ctc(struct _timer8 *this, irq_t *irq) {
    
    if(++(*(this->tcnt)) == 0x00) {

        setbit(*(this->tifr), TOV);

        if(bit(*(this->timsk), TOV) == 0x01)
            irq_enable(irq, OVF0_VECT);
    }

    if(*(this->tcnt) == *(this->ocrb)) {
        
        setbit(*(this->tifr), OCFB);
        
        if(bit(*(this->timsk), OCFB) == 0x01)
            irq_enable(irq, OC0B_VECT); // currently only tc0
            
        trigger_ocpb(this);
    }

    if(*(this->tcnt) == *(this->ocra)) {

        setbit(*(this->tifr), OCFA);
        *(this->tcnt) = 0x00;

        if(bit(*(this->timsk), OCFA) == 0x01)
            irq_enable(irq, OC0A_VECT); // currently only tc0

        trigger_ocpa(this);
    }
}

static void timer8_tick_pwm_fast(struct _timer8 *this, irq_t *irq) {

    /* in progress */
    return;
}

static void timer8_tick_reserved(struct _timer8 *this, irq_t *irq) {
    
    /* accessing reserved waveform generation mode */
    return;
}

static void trigger_ocpa(struct _timer8 *this) {

    if(bit(*(this->ddrxa), this->oca) == 0x00)
        return;

    switch( comtc8a(*(this->tccra)) ) {

        case 0x00: /* OC disconnected */                  break;
        case 0x01: *(this->ocpa) ^= (0x01 << this->oca);  break;
        case 0x02: *(this->ocpa) &= ~(0x01 << this->oca); break;
        case 0x03: *(this->ocpa) |= (0x01 << this->oca);  break;

        default: /* not possible */                       break;
    }
}

static void trigger_ocpb(struct _timer8 *this) {

    if(bit(*(this->ddrxb), this->ocb) == 0x00)
        return;

    switch( comtc8b(*(this->tccra)) ) {

        case 0x00: /* OC disconnected */                  break;
        case 0x01: *(this->ocpb) ^= (0x01 << this->ocb);  break;
        case 0x02: *(this->ocpb) &= ~(0x01 << this->ocb); break;
        case 0x03: *(this->ocpb) |= (0x01 << this->ocb);  break;

        default: /* not possible */                       break;
    }
}

static void trigger_ocpa_pwm_fast(struct _timer8 *this) {

    if(bit(*(this->ddrxa), this->oca) == 0x00)
        return;

    switch( comtc8a(*(this->tccra)) ) {

        case 0x00: /* OC disconnected */                  break;
        case 0x01: /* reserved */                         break;
        case 0x02: /* not supported */                    break;
        case 0x03: /* not supported */                    break;

        default: /* not possible */                       break;
    }
}

static void trigger_ocpb_pwm_fast(struct _timer8 *this) {

    if(bit(*(this->ddrxb), this->ocb) == 0x00)
        return;

    switch( comtc8b(*(this->tccra)) ) {

        case 0x00: /* OC disconnected */                  break;
        case 0x01: /* reserved */                         break;
        case 0x02: /* not supported */                    break;
        case 0x03: /* not supported */                    break;

        default: /* not possible */                       break;
    }
}

static void trigger_ocpa_pwm_correct(struct _timer8 *this) {

    if(bit(*(this->ddrxa), this->oca) == 0x00)
        return;

    switch( comtc8a(*(this->tccra)) ) {

        case 0x00: /* OC disconnected */                  break;
        case 0x01: /* reserved */                         break;
        case 0x02: /* not supported */                    break;
        case 0x03: /* not supported */                    break;

        default: /* not possible */                       break;
    }
}

static void trigger_ocpb_pwm_correct(struct _timer8 *this) {

    if(bit(*(this->ddrxb), this->ocb) == 0x00)
        return;

    switch( comtc8b(*(this->tccra)) ) {

        case 0x00: /* OC disconnected */                  break;
        case 0x01: /* reserved */                         break;
        case 0x02: /* not supported */                    break;
        case 0x03: /* not supported */                    break;

        default: /* not possible */                       break;
    }
}

static void (*tick[NMODT8]) (struct _timer8 *this, irq_t *irq) = {

    timer8_tick_normal,
    timer8_tick_pwm_correct,
    timer8_tick_ctc,
    timer8_tick_pwm_fast,
    timer8_tick_reserved,
    timer8_tick_pwm_correct,
    timer8_tick_reserved,
    timer8_tick_pwm_fast
};
