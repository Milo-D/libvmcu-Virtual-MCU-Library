.INCLUDE "m32def.inc"

ldi r16, 0x00
rjmp inc_loop

inc_loop:
inc r16
brmi dec_loop
rjmp inc_loop

dec_loop:
dec r16
brmi exit
rjmp dec_loop

exit:
rjmp exit
