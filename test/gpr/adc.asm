.INCLUDE "m32def.inc"

ldi r16, 0x00
ldi r17, 0xff
sec
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp main

main:
adc r16, r17
rjmp exit

exit:
nop