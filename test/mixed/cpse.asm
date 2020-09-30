.INCLUDE "m32def.inc"

jmp test_false
jmp end

test_false:
ldi r16, 0xff
ldi r17, 0x80
cpse r16, r17
rjmp test_true

test_true:
ldi r16, 0xa9
ldi r17, 0xa9
cpse r16, r17
rjmp test_true
rjmp test_32bit

test_32bit:
ldi r16, 0xff
ldi r17, 0xff
cpse r16, r17
jmp test_32bit
jmp test_wrap

end:
rjmp end

.ORG 0x3fff

test_wrap:
cpse r16, r17
