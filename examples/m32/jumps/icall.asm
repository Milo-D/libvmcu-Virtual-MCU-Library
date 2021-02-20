.INCLUDE "m32def.inc"

stack:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp init

init:
ldi ZL, 0x05
ldi ZH, 0x00
cpi r16, 0x01
breq ret_from_icall
rjmp test

test:
ldi r16, 0x01
icall
rjmp exit

ret_from_icall:
ret

exit:
rjmp exit
