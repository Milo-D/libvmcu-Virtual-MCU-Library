.INCLUDE "m328Pdef.inc"

;
; Small test for the EEPROM Write-Only Mode
; (EEPM1 = 1). This test writes a value
; (EEDR = 0x21) at a specific address (EEAR = 0x0000).
;
; Address 0x0000 contains non erased data (!= 0xff).
; For this test, it is important to load the non erased
; data into the EEPROM with the 'leep <file.eep.hex>' command.
;


.CSEG
.ORG 0x0000

rjmp main			; jump to main


main:
ldi r16, 'J'			; data argument for EEDR
clr r17				; address argument for EEARL
clr r18				; address argument for EEARH
rcall eeprom_wonly		; call to write-only subroutine
rjmp end			; end loop, time to wait for the write access

eeprom_wonly:
sbic EECR, EEPE			; check if previous write access has finished
rjmp eeprom_wonly		; wait if not
out EEDR, r16			; store value argument in EEDR
out EEARL, r17			; store address argument in EEARL
out EEARH, r18			; store address argument in EEARH
sbi EECR, EEPM1			; (EEPM = 1) ==> write-only mode 
sbi EECR, EEMPE			; Master-Write Enabled
sbi EECR, EEPE			; EEPROM Write Strobe
ret				; return to main

end:
rjmp end			; wait here

.ESEG
.ORG 0x0000

.db "D"				; non erased data, load it with 'leep' into MDX
