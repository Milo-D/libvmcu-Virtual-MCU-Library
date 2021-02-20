.INCLUDE "m32def.inc"

;
; Demonstrating CTC Mode with a 8-bit Timer0 in ATmega32.
; 
; The following program initialize TCNT0, and
; enables Hardware Interrupts (OCIE0 in TIMSK).
; Whenever TCNT0 matches OCR, the
; ISR (isr_tim0_oc) is executed. The ISR is
; writing to PORTB.
;
; Mode: Timer0 (8-bit), CTC Mode
; OCR: 0x80
; Prescaler: 1
; CPU Clock: 1.000.000 Hz
; 

.def temp = r18

.ORG 0x0000
rjmp stack_init

.ORG 0x0014
rjmp isr_tim0_oc

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp port_init

port_init:
ldi r17, 0xff
out DDRB, r17
out PORTB, r17
rjmp set_comp

set_comp:
ldi r16, 0x80
out OCR0, r16
rjmp enable_irq

enable_irq:
ldi r16, (0x01 << OCIE0)
out TIMSK, r16
sei
rjmp enable_timer

enable_timer:
ldi r16, (0x01 << CS00) | (0x01 << WGM01)
out TCCR0, r16
rjmp loop

loop:
rjmp loop

; ISR (OC Match)

isr_tim0_oc:
out PORTB, r17
com r17
reti


