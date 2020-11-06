.INCLUDE "m328Pdef.inc"

;
; EEPROM - Read Test
;
; This test reads a single byte from
; the EEPROM.
;
; This test is passed if 0xff is read
; from the EEPROM and then, pushed
; onto the stack.
;

.def addrl = r16
.def addrh = r17
.def temp = r18

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
rcall read_eeprom
push r0
rjmp exit

read_eeprom:
sbic EECR, EEPE
rjmp read_eeprom
out EEARL, addrl
out EEARH, addrh
sbi EECR, EERE
in r0, EEDR
ret

exit:
rjmp exit
