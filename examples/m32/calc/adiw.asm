.INCLUDE "m32def.inc"

x_init:
ldi XL, 0x00
ldi XH, 0x00
rjmp y_init

y_init:
ldi YL, 0xff
ldi YH, 0xff
rjmp stack_init

stack_init:
ldi r16, LOW(RAMEND)
out SPL, r16
ldi r16, HIGH(RAMEND)
out SPH, r16
rjmp test_adiw_sbiw

test_adiw_sbiw:
adiw XH:XL, 0x01
sbiw YH:YL, 0x3f
breq exit
rjmp test_adiw_sbiw

exit:
push XL
push XH
rjmp exit
