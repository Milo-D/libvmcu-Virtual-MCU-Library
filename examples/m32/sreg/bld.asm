.INCLUDE "m32def.inc"

ldi r16, 0x00
ldi r17, 0x00
set
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp main

main:
rcall load_0
rcall load_1
rcall load_2
rcall load_3
rcall load_4
rcall load_5
rcall load_6
rcall load_7
cpi r17, 0x01
breq exit
bclr 0x06
inc r17
rjmp main

load_0:
bld r16, 0
ret

load_1:
bld r16, 1
ret

load_2:
bld r16, 2
ret

load_3:
bld r16, 3
ret

load_4:
bld r16, 4
ret

load_5:
bld r16, 5
ret

load_6:
bld r16, 6
ret

load_7:
bld r16, 7
ret

exit:
rjmp exit