.INCLUDE "m32def.inc"

.def counter = r18

.ORG 0x0000
jmp init_counter

.ORG OVF0addr
jmp isr_ovf

init_counter:
ldi counter, 0x02
rjmp init

init:
ldi r16, LOW(RAMEND)
out SPL, r16
ldi r16, HIGH(RAMEND)
out SPH, r16
rjmp enable_timer

enable_timer:
ldi r16, (0x01 << TOIE0)
out TIMSK, r16
sei
ldi r16, (0x01 << CS00)
out TCCR0, r16
rjmp wait

wait:
rjmp wait

isr_ovf:
dec counter		; compare predefined counter
breq isr_return		; return from interrupt
sei			; allow nested interrupts
ldi r16, 0xf0		; tcnt value
out TCNT0, r16
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

isr_return:
reti
