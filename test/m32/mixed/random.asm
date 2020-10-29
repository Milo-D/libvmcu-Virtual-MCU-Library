.INCLUDE "m32def.inc"

.def rmp = r18

Main:
; Init stack
ldi rmp, LOW(RAMEND) ; Init LSB stack
out SPL,rmp
; Init Port B
ldi rmp,(1<<DDB2)|(1<<DDB1)|(1<<DDB0) ; Direction of Port B
out DDRB,rmp
; [Add all other init routines here]
ldi rmp,1<<SE ; enable sleep
out MCUCR,rmp
sei
