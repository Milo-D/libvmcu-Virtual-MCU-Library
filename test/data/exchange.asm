.INCLUDE "m32def.inc"

; Won't find XCH Instruction for ATmega32(L) 

.def temp = r18

gpr_init:
clr r17
ldi r16, 0xff
rjmp stack_init

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp main

main:
ldi XL, 0x00
ldi XH, SPH
ld ZL, -X
ld ZH, X
push r16
xch Z, r17
ld r17, -X
cpi r16, 0x00
breq exit
rcall decrement
rjmp main

decrement:
dec r16
ret

exit:
rjmp exit