.INCLUDE "m32def.inc"

.def head = r16
.def value = r17
.def pointer = r18
.def cval = r19
.def cpoi = r20
.def it = r21
.def find = r22
.def rem = r23
.def const = r24
.def temp = r25

.equ ROOT = 0xc4
.equ NULL = 0x00

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
clr temp
rjmp head_init

head_init:
ldi XL, ROOT
ldi value, 0x00
st X, value
dec XL
ldi pointer, NULL
st X, pointer
ldi head, ROOT
rjmp main

main:
ldi value, 0xf5
rcall fill_list
ldi find, 0xff
ldi it, ROOT
rcall find_element
rjmp exit

fill_list:
inc value
rcall push_back
cpi value, 0xff
brne fill_list
ret

exit:
rjmp exit

push_back:
cpi head, LOW(RAMEND) - 0x01
breq exit
rcall move_val
add XL, const
st X, value
rcall move_poi
add XL, const
mov pointer, head
sub pointer, const
st X, pointer
sub head, const 
ret

move_val:
mov XL, head
ld temp, X
ldi const, 0x02
sub XL, const
st X, temp
ret

move_poi:
mov XL, head
dec XL
ld temp, X
ldi const, 0x02
sub XL, const
st X, temp
ret

find_element:
mov XL, it
ld cval, X
dec XL
ld cpoi, X
cp cval, find
brne next
ret

next:
cpi it, LOW(RAMEND) - 0x01
breq not_found
cpi cpoi, NULL
breq exit
mov it, cpoi
rjmp find_element

not_found:
ldi cval, 0x00
ldi cpoi, NULL
ret

remove:
mov find, rem
rcall find_element
mov it, cpoi
ldi const, 0x02
sub it, const
rjmp slide_r

slide_r:
mov XL, it
ld temp, X
ldi const, 0x02
add XL, const
st X, temp
ldi const, 0x03
sub XL, const
ld temp, X
ldi const, 0x02
add XL, const
st X, temp
cp it, head
breq rem_return
brlo rem_return
ldi const, 0x02
sub XL, const
rjmp slide_r

rem_return:
mov XL, head
ldi value, 0x00
st X, value
dec XL
ldi pointer, NULL
st X, pointer   
dec XL
dec XL
ret

del_list:
mov XL, it
ldi value, 0x00
st X, value
dec XL
ldi pointer, NULL
st X, pointer
cp it, head
breq del_return
ldi const, 0x02
sub it, const
rjmp del_list

del_return:
ret

