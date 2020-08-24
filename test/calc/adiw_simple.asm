.INCLUDE "m32def.inc"

loop:
adiw XH:XL, 0x01
cpi XL, 0xff
brne loop
cpi XH, 0xff
brne loop
rjmp exit

exit:
nop
