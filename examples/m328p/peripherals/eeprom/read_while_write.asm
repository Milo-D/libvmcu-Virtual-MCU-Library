.INCLUDE "m328Pdef.inc"

;
; EEPROM - Read-While-Write Test
;
; This test covers the case when
; the user tries to read the EEPROM
; without polling the EEPE bit.
;
; This test is successfully passed, if
; the desired read access fails.
;

.def addrl = r16
.def addrh = r17
.def data = r18

.def temp = r19

.equ BASE = 0x0000

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp main

main:
ldi addrl, LOW(BASE)
ldi addrh, HIGH(BASE)
ldi data, 0x80
rcall write_eeprom
rcall read_eeprom
push r0
rjmp exit

read_eeprom:
sbi EECR, EERE
in r0, EEDR
ret

write_eeprom:
sbic EECR, EEPE
rjmp write_eeprom
out EEARL, addrl
out EEARH, addrh
out EEDR, data
sbi EECR, EEMPE
sbi EECR, EEPE
ret

exit:
rjmp exit
