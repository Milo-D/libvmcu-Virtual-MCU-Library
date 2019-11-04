.INCLUDE "m32def.inc"

; Simple GPR Instruction Test

ldi r16, 0x11
ldi r17, 21
mov r16, r17
mov r0, r16
ldi r20, 0x01