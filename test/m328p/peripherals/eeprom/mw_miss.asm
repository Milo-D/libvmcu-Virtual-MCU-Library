.INCLUDE "m328Pdef.inc"

; EEPROM Master-Write-Miss Test

.def data = r16
.def addrl = r17
.def addrh = r18

.equ ADDR = 0x0000

stack_init:
ldi r20, LOW(RAMEND)
out SPL, r20
ldi r20, HIGH(RAMEND)
out SPH, r20
rjmp main

main:
ldi data, 0x21
ldi addrl, LOW(ADDR)
ldi addrh, HIGH(ADDR)
rcall write_eeprom
rjmp exit

write_eeprom:
sbic EECR, EEPE
rjmp write_eeprom
out EEDR, data
out EEARL, addrl
out EEARH, addrh
sbi EECR, EEMPE
inc XL
inc XL
inc XL
inc XL
sbi EECR, EEPE
ret

exit:
sbic EECR, EEPE
rjmp exit
rjmp erdy

erdy:
rjmp erdy
