.INCLUDE "m32def.inc"

init:
ldi YL, 0xff
ldi YH, 0xff
rjmp loop

loop:
sbiw YH:YL, 0x01
breq exit
rjmp loop

exit:
rjmp exit
