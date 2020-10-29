.INCLUDE "m32def.inc"

;
; Demonstrating 8-bit Timer0 in ATmega32.
;
; MDX is now able to simulate hardware timers
; and interrupts. Currently, only timer0
; (TCNT0) is available and PWM is not supported.
; But I am working on it ;)
; 
; The following program initialize TCNT0, and
; enables Hardware Interrupts (TOIE0 in TIMSK).
; Whenever an overflow in TCNT0 occurs, the
; ISR (isr_tim0_ov) is executed. The ISR is
; writing to PORTB.
;
; Mode: Timer0 (8-bit), Normal Mode
; Prescaler: clk(cpu) / 1
; 
; This program was successfully simulated by MDX.
;


.def temp = r18

.ORG 0x0000
rjmp stack_init

.ORG OVF0addr
rjmp isr_tim0_ov

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
rjmp enable_irq

enable_irq:
ldi r16, 0x01
out TIMSK, r16
sei
rjmp enable_timer

enable_timer:
ldi r16, (0x01 << CS00)
out TCCR0, r16
rjmp loop

loop:
rjmp loop

; ISR (Timer0 Overflow)

isr_tim0_ov:
out PORTB, r17
com r17
reti


