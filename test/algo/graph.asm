.INCLUDE "m32def.inc"

; Recursive Depth First Search through a Graph.

.def addr = r16
.def found = r17
.def gadl = r18
.def leaf = r19
.def temp = r20

.equ ROOT = 0x0081
.equ TARGET = 0x02

gpr_init:
ldi XL, LOW(ROOT)
ldi XH, HIGH(ROOT)
rjmp stack_init

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp graph_init

graph_init:
rcall root_init
ldi leaf, 0x01
rcall leaf_init
ldi leaf, 0x02
rcall leaf_init
rjmp main

root_init:
ldi temp, 0x00
st X, temp
dec XL
ldi temp, (ROOT - 0x03)
st X, temp
dec XL
ldi temp, (ROOT - 0x06)
st X, temp
ret

leaf_init:
dec XL
st X, leaf
dec XL
ldi temp, 0x00
st X, temp
dec XL
st X, temp
ret

main:
ldi XL, ROOT
rcall search
rjmp exit

search:
ld temp, X
cpi temp, TARGET
brne test_left
mov addr, XL
ret

test_left:
ld temp, -X
cpi temp, 0x00
breq test_right
rjmp go_left

test_right:
ld temp, -X
cpi temp, 0x00
brne go_right
ret

go_left:
push XL
ld gadl, X
mov XL, gadl
rcall search
pop XL
rjmp test_right

go_right:
push XL
ld gadl, X
mov XL, gadl
rcall search
pop XL
ret

exit:
mov XL, addr
ldi XH, 0x00
ld found, X