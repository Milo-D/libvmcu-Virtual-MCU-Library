.INCLUDE "m32def.inc"

.def temp = r18

init:                    ; SP Init
ldi temp, HIGH(RAMEND)   ; R18 <- H(RAMEND)
out SPH, temp            ; [SRAM + 0x20 + SPH] <- R18
ldi temp, LOW(RAMEND)    ; R18 <- L(RAMEND)
out SPL, temp            ; [SRAM + 0x20 + SPL] <- R18
rjmp build               ; PC <- [PC + build + 1]

build:
ldi temp, 0x21           ; R18 <- 0x21
push temp                ; DATA[SP] <- R18
rjmp load                ; PC <- [PC + load + 1]

load:
ldi YL, 0x5f             ; R28 <- 0x5f
ldi YH, 0x08             ; R29 <- 0x08
ld r16, Y                ; R16 <- [DATA + YL + YH]
rjmp end                 ; PC <- [PC + end + 1]

end:
rjmp end                 ; PC <- [PC + end + 1]