.INCLUDE "m32def.inc"

ldi r16, 0x00
ldi r17, 0x00
ldi r18, 0x00
rjmp stack_init

stack_init:
ldi r19, LOW(RAMEND)
out SPL, r19
ldi r19, HIGH(RAMEND)
out SPH, r19
rjmp outer

outer:
rcall inner
cpi r16, 0xff
breq exit
rjmp inc_outer

inner:
mov r18, r16
adc r18, r17
cpi r17, 0xff 
brne inc_inner
ret

inc_outer:
inc r16
rjmp outer

inc_inner:
inc r17
rjmp inner

exit:
nop