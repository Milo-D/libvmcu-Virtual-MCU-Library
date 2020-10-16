.INCLUDE "m32def.inc"

;
; FOC0 (Force Output Compare) Test
;
; This small test covers the FOC0 bit
; in TCCR0 of Timer0.
;
; Writing a logical one to the FOC0 bit
; triggers an immediate Output Compare Match
; without generating an IRQ or setting the OC
; Flag.
;
; The FOCx bits are implemented as strobe bits,
; so writing a logical one will cause an action on
; the specified OC-Pin (if COM01:00 is connected).
;
; In this case, the FOC0 bit only works in a non-PWM
; Mode. The following table shows the functionality of
; COM01:00 in non-PWM Mode:
;
; COM01 | COM00 | Action
;-----------------------------------
;   0   |   0   | OC disconnected
;   0   |   1   | toggle OC
;   1   |   0   | clear OC 
;   1   |   1   | set OC
;
; The trigger_oc function for non-PWM mode was already tested
; successfully, that's why this test wont focus on all these
; functionalities.
;
; This little test generates an interrupt, caused by the overflow
; of timer0. The ISR will then trigger the FOC0 strobe. OCR is set
; to 0x80. So the OC should toggle at 0x80 and ~ 0x00.
;
; That should give us a PWM signal on PINB3 (OC).
;
; --- Specs ---
;
; Timer: 8-bit timer0
; WGM: Normal Mode
; COM: toggle OC (0x01)
; CPU Clock: 1.000.000 Hz
; Prescaler: 1
; IRQs: Enabled (Overflow)
;

.ORG 0x0000
jmp port_init

.ORG OVF0addr
jmp isr_timer0_ovf

port_init:
ldi r16, (0x01 << PB3)
out DDRB, r16
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp enable_irq

enable_irq:
ldi r16, (0x01 << TOIE0)
out TIMSK, r16
sei
rjmp enable_timer

enable_timer:
ldi r16, 0x80
out OCR0, r16
ldi r16, (0x01 << CS00) | (0x01 << COM00)
out TCCR0, r16
rjmp loop

loop:
rjmp loop

isr_timer0_ovf:
ldi r16, (0x01 << CS00) | (0x01 << COM00) | (0x01 << FOC0)
out TCCR0, r16
reti

