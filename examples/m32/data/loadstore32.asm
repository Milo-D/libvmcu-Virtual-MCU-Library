.INCLUDE "m32def.inc"

zero_test:
lds r16, 0x0000
sts 0x0000, r16
jmp ram_end_test

ram_end_test:
lds r16, RAMEND
sts RAMEND, r16
rjmp random_test

random_test:
lds r16, 0x01f0
sts 0x01f0, r16
jmp end

end:
rjmp end
