.INCLUDE "m32def.inc"

gpr_init:
ldi YL, 0xff
ldi YH, 0xff
rjmp test_sbiw

test_sbiw:
sbiw YH:YL, 0x3f
breq exit
rjmp test_sbiw

exit:
rjmp exit
