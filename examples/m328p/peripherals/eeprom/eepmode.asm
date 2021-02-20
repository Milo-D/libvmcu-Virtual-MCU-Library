.INCLUDE "m328Pdef.inc"

; EEPROM EEPM1:0 block Test
;
; Once the EEPE bit is set in EECR,
; further writes to the EEPM1:0 bits
; are blocked by hardware.
;
; see Atmel-42735A-ATmega328/P_Datasheet_Complete-06/2016
; p.42

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
sbi EECR, EEPE
ret

write_eeprom_mode:
sbi EECR, EEPM0
sbi EECR, EEPM1
cbi EECR, EEPM0
cbi EECR, EEPM1
ret

exit:
rcall write_eeprom_mode
sbic EECR, EEPE
rjmp exit
rjmp erdy

erdy:
rcall write_eeprom_mode
rjmp erdy

