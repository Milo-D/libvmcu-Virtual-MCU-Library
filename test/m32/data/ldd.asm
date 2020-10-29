.INCLUDE "m32def.inc"

.CSEG
.org 0x0000

.def temp = r18
.def target = r20

gpr_init:
ldi r16, 0xaa
ldi r17, 0x21
ldi r18, 0x0f
ldi r19, 0xdf
ldi target, 0x01 
rjmp stack_init

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp main

main:
push r16
push r17
push r18
push r19
push target
rcall set_addr
ldd r16, Y+4
rjmp exit

set_addr:
ldi YL, 0x57
ldi YH, 0x08
ret

exit:
rjmp exit