.INCLUDE "m32def.inc"

.def temp = r18

init:                    ; SP Init
ldi temp, HIGH(RAMEND)   ; R18 <- H(RAMEND)
out SPH, temp            ; [SRAM + 0x20 + SPH] <- R18
ldi temp, LOW(RAMEND)    ; R18 <- L(RAMEND)
out SPL, temp            ; [SRAM + 0x20 + SPL] <- R18

start: 
ldi r16, 0x2c            ; R16 <- 0x2c
ldi r17, 0x1f            ; R17 <- 0x1f
brne init                ; if (Z = 0): PC <- PC + init + 1
rjmp exit                ; PC <- PC + exit + 1

exit:
clr r16                  ; R16 <- R16 xor R16
clr r17                  ; R17 <- R17 xor R17
clr r18                  ; R18 <- R18 xor R18