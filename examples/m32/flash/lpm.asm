; Testing lpm(R0) and 32-bit jmp
; Reading opcode 0xBF2D and pushing these
; 2 bytes onto stack.

.INCLUDE "m32def.inc"

ldi ZL, 0x08
ldi ZH, 0x00
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp test_lpm

test_lpm:
lpm
push r0
adiw ZH:ZL, 0x01
lpm
push r0
jmp exit

exit:
jmp exit
