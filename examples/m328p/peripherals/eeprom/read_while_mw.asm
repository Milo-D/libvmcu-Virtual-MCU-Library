.INCLUDE "m328Pdef.inc"

;
; EEPROM - Read-While-Master-Write Test
;

.def addrl = r16
.def addrh = r17

.def temp = r18

.equ EEP_BASE = 0x0000

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp main

main:
ldi addrl, LOW(EEP_BASE)
ldi addrh, HIGH(EEP_BASE)
out EEARL, addrl
out EEARH, addrh
sbi EECR, EEMPE
sbi EECR, EERE
in r0, EEDR
push r0
rjmp exit

exit:
rjmp exit
