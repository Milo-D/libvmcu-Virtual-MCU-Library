.INCLUDE "m32def.inc"

gpr_init:
ldi r16, 0x0a
brvs exit
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp test

test:
dec r16
brne test
rcall set
brvs exit

set:
sev
ret

exit:
rjmp exit
