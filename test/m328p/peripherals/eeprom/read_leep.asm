.INCLUDE "m328Pdef.inc"

;
; EEPROM - Read-leep Test
;
; In this test, a .eep.hex file
; is loaded (via 'leep' command)
; into EEPROM and then read out
; by software.
;

.def strlen = r17
.def temp = r18

.def addrl = r24
.def addrh = r25

.equ MSGLEN = 16

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp gpr_init

gpr_init:
ldi strlen, MSGLEN
ldi addrl, LOW(eep_data)
ldi addrh, HIGH(eep_data)
ldi XL, 0x00
ldi XH, 0x01
rjmp main

main:
rcall read_eeprom
st X+, r0
adiw addrh:addrl, 0x01
dec strlen
brne main
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

.ESEG

.ORG 0x0000
eep_data:
.db "EEPROM Read Test"
