.INCLUDE "m328Pdef.inc"

; EEPROM EEPM1:0 Erase Only Test
;
; This test covers two different EEPROM Modes:
;
; 1.) Atomic Erase-and-Write (EEPM1:0 = 0x00)
; 2.) Erase Only (EEPM1:0 = 0x01)
;

.def data = r16
.def mode = r17
.def addrl = r24
.def addrh = r25
.def strlen = r20
.def temp = r21

.equ EW_MODE = 0x00
.equ E_MODE = (0x01 << EEPM0)
.equ EEP_BASE = 0x0000

.equ BASE = 0x0100
.equ MSGLEN = 6

.ORG 0x0000
rjmp stack_init

.ORG 0x0002

flash_data:
.db "EEPROM"

stack_init:
ldi temp, LOW(RAMEND)
out SPL, temp
ldi temp, HIGH(RAMEND)
out SPH, temp
rjmp gpr_init

gpr_init:
ldi ZL, LOW(2 * flash_data)
ldi ZH, HIGH(2 * flash_data)
ldi XL, LOW(BASE)
ldi XH, HIGH(BASE)
ldi strlen, MSGLEN
rjmp data_init

data_init:
rcall read_flash
dec strlen
breq main_write
rjmp data_init

read_flash:
lpm r0, Z+
st X+, r0
ret

main_write:
ldi addrl, LOW(EEP_BASE)
ldi addrh, HIGH(EEP_BASE)
ldi mode, EW_MODE
ldi XL, LOW(BASE)
ldi XH, HIGH(BASE)
ldi strlen, MSGLEN
rcall write_eeprom
rjmp main_erase

main_erase:
ldi addrl, LOW(EEP_BASE)
ldi addrh, HIGH(EEP_BASE)
ldi mode, E_MODE
ldi XL, LOW(BASE)
ldi XH, HIGH(BASE)
ldi strlen, MSGLEN
rcall write_eeprom
rjmp exit

write_eeprom:
sbic EECR, EEPE
rjmp write_eeprom

sbrs mode, 4
ld data, X+
sbrs mode, 4
out EEDR, data
out EEARL, addrl
out EEARH, addrh
adiw addrh:addrl, 0x01

out EECR, mode
sbi EECR, EEMPE
sbi EECR, EEPE

dec strlen
brne write_eeprom
ret

exit:
rjmp exit








