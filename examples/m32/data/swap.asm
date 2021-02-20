.INCLUDE "m32def.inc"

.CSEG
.org 0x0000

.def temp = r18

gpr_init:
ldi r16, 0xdb
ldi r17, 0x21
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
rcall load_y_addr
ld r16, Y+
ld r17, Y
rjmp exit

load_y_addr:
ldi YL, LOW(RAMEND)
ldi YH, HIGH(RAMEND)
dec YL
ret

exit:
rjmp exit