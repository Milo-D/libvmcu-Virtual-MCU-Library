.INCLUDE "m32def.inc"

gpr_init:
clr r16
clr r18
ldi r17, 0xff
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp loop

loop:
mov r16, r17
asr r16
push r16
cpi r17, 0x00
breq exit
dec r17
rjmp loop

exit:
rjmp exit