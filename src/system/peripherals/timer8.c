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
* */

/* Forward Declarations of static Functions */

static void timer8_setup_tc0(struct _timer8 *this, int8_t *memory);
static void timer8_setup_tc2(struct _timer8 *this, int8_t *memory);

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

        case TC0: timer8_setup_tc0(timer, memory); break;
        case TC2: timer8_setup_tc2(timer, memory); break;

        default: free(timer);                return NULL;
    };

    timer->prescaler = 0;
    timer->countdown = 0;

    return timer;
}

void timer8_dtor(struct _timer8 *this) {

    free(this);
}

void timer8_update(struct _timer8 *this, irq_t *irq, const uint64_t dc) {

    if(this->prescaler == 0)
        return;

    for(int i = 0; i < dc; i++) {

        if(--(this->countdown) > 0)
            continue;
            
        const uint8_t a = *(this->tccra);
        const uint8_t b = *(this->tccrb);

        (*tick[ wgmtc8(a, b) ])(this, irq);
        this->countdown = this->prescaler;
    }
}

void timer8_update_prescaler(struct _timer8 *this) {

    uint16_t csx_table[8] = {
        
        0,
        1,
        8,
        64,
        256,
        1024,
        0,
        0
    };
    
    const uint8_t p = *(this->tccrb) & CSX_MSK;
    
    this->prescaler = csx_table[p];
    this->countdown = csx_table[p];
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

static void timer8_setup_tc0(struct _timer8 *this, int8_t *memory) {

    this->timsk = &memory[TIMSK0];
    this->tccra = &memory[TCCR0A];
    this->tccrb = &memory[TCCR0B];

    this->tcnt  = &memory[TCNT0];
    this->tifr  = &memory[TIFR0];
    this->ocra  = &memory[OCR0A];
    this->ocrb  = &memory[OCR0B];

    this->ocpa  = &memory[OC0A_P];
    this->ocpb  = &memory[OC0B_P];

    this->ddrxa = &memory[OC0A_DDR];
    this->ddrxb = &memory[OC0B_DDR];

    this->oca   = OC0A;
    this->ocb   = OC0B;
}

static void timer8_setup_tc2(struct _timer8 *this, int8_t *memory) {

    this->timsk = &memory[TIMSK2];
    this->tccra = &memory[TCCR2A];
    this->tccrb = &memory[TCCR2B];

    this->tcnt  = &memory[TCNT2];
    this->tifr  = &memory[TIFR2];
    this->ocra  = &memory[OCR2A];
    this->ocrb  = &memory[OCR2B];

    this->ocpa  = &memory[OC2A_P];
    this->ocpb  = &memory[OC2B_P];

    this->ddrxa = &memory[OC2A_DDR];
    this->ddrxb = &memory[OC2B_DDR];

    this->oca   = OC2A;
    this->ocb   = OC2B;
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
