.INCLUDE "m32def.inc"

; Testing new Hex Converter

.def acc = r16
.def temp = r18

init:                    ; SP Init
ldi temp, HIGH(RAMEND)   ; R18 <- H(RAMEND)
out SPH, temp            ; [SRAM + 0x20 + SPH] <- R18
ldi temp, LOW(RAMEND)    ; R18 <- L(RAMEND)
out SPL, temp            ; [SRAM + 0x20 + SPL] <- R18
ldi acc, 0xff            ; R16 <- 0xff
rjmp loop                ; PC <- [PC + loop + 1]

loop:
push acc                 ; DATA[SP] <- R18
brne rep                 ; Z = 0 : PC <- [PC + rep + 1]
rjmp exit                ; PC <- [PC + exit + 1]

rep:
dec acc                  ; R16 <- R16 - 0x01
rjmp loop                ; PC <- [PC + loop + 1]

exit:
nop                      ; no instruction