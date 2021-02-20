.INCLUDE "m32def.inc"

; Using LD -Y Instruction to swap Values of R16 and R17

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
ld r17, -Y
ld r16, -Y
rjmp exit

load_y_addr:
ldi YL, LOW(RAMEND)
ldi YH, HIGH(RAMEND)
inc YL
ret

exit:
rjmp exit