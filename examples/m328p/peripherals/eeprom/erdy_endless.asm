.INCLUDE "m328Pdef.inc"

;
; EEPROM - ERDY Interrupt Test (endless)
;
; EERIE generates a constant interrupt request
; when set to 'one' and EEPE to 'zero'.
;
; The ISR is not resetting EERIE nor EEPE,
; resulting in an endless queue of ERDY interrupts.
;

.def data = r20
.def addrl = r21
.def addrh = r22

.ORG 0x0000
jmp stack_init

.ORG ERDYaddr
jmp erdy_isr

stack_init:
ldi r18, LOW(RAMEND)
out SPL, r18
ldi r18, HIGH(RAMEND)
out SPH, r18
rjmp main

main:
ldi data, 0x21
ldi addrl, 0xff
ldi addrh, 0x03
sei
rcall eeprom_write
rjmp exit

eeprom_write:
out EEDR, data
out EEARL, addrl
out EEARH, addrh
sbi EECR, EEMPE
sbi EECR, EEPE
sbi EECR, EERIE
ret

exit:
rjmp exit

erdy_isr:
reti
