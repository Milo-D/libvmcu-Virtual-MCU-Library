.INCLUDE "m32def.inc"

.def data = r16

gpr_init:
ldi YH, 0x08
ldi YL, 0x1f
ldi data, 0xef
rjmp test

test:
std Y+0, data
std Y+16, data
std Y+21, data
std Y+33, data
std Y+50, data
std Y+63, data
rjmp exit

exit:
rjmp exit

