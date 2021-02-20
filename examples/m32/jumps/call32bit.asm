.INCLUDE "m32def.inc"

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
jmp gpr_init

gpr_init:
clr r16
rjmp loop

loop:
push r16
cpi r16, 0x0a
breq exit
call subroutine
jmp loop

subroutine:
inc r16
ret

exit:
jmp exit
