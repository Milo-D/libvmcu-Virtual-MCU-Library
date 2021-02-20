.INCLUDE "m32def.inc"

ldi r16, 0b10101010
set
rjmp stack_init

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp main

main:
rcall store_0
rcall store_1
rcall store_2
rcall store_3
rcall store_4
rcall store_5
rcall store_6
rcall store_7
rjmp exit

store_0:
bst r16, 0
ret

store_1:
bst r16, 1
ret

store_2:
bst r16, 2
ret

store_3:
bst r16, 3
ret

store_4:
bst r16, 4
ret

store_5:
bst r16, 5
ret

store_6:
bst r16, 6
ret

store_7:
bst r16, 7
ret

exit:
rjmp exit
