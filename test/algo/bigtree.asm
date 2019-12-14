.INCLUDE "m32def.inc"

; Recursive Depth First Search through a Graph.

.def addr = r16
.def found = r17
.def gadl = r18
.def leaf = r19
.def temp = r20

.equ ROOT = 0x0090
.equ TARGET = 0x2a

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
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x8d
st X, temp
dec XL
ldi temp, 0x8a
st X, temp
dec XL
ldi temp, 0x0f
st X, temp
dec XL
ldi temp, 0x87
st X, temp
dec XL
ldi temp, 0x84
st X, temp
dec XL
ldi temp, 0xff
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x81
st X, temp
dec XL
ldi temp, 0xd0
st X, temp
dec XL
ldi temp, 0x7e
st X, temp
dec XL
ldi temp, 0x7b
st X, temp
dec XL
ldi temp, 0x02
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x2a
st X, temp
dec XL
ldi temp, 0x78
st X, temp
dec XL
ldi temp, 0x75
st X, temp
dec XL
ldi temp, 0x67
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x03
st X, temp
dec XL
ldi temp, 0x72
st X, temp
dec XL
ldi temp, 0x6f
st X, temp
dec XL
ldi temp, 0x09
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x6c
st X, temp
dec XL
ldi temp, 0x3e
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x10
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x21
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x3e
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0xcf
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
ldi temp, 0x00
st X, temp
dec XL
rjmp main

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