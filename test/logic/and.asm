.INCLUDE "m32def.inc"

init:
ldi r16, 0x01          
ldi r17, 0x01            
ldi r18, 0x01     
rjmp stack_init   

stack_init:
ldi r21, LOW(RAMEND)
out SPL, r21
ldi r21, HIGH(RAMEND)
out SPH, r21
rjmp loop

loop:
cpi r16, 0x4f
breq exit
and r16, r17
brne put_one
rjmp put_zero

put_zero:
ldi r20, 0x00
push r20
inc r18
mov r16, r18
rjmp loop

put_one:
ldi r20, 0x01
push r20
inc r18
mov r16, r18
rjmp loop

exit:
nop