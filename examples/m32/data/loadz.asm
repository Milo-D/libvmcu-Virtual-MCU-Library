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
ldi ZL, 0x5f             ; R30 <- 0x5f
ldi ZH, 0x08             ; R31 <- 0x08
ld r16, Z                ; R16 <- [DATA + ZL + ZH]
rjmp end                 ; PC <- [PC + end + 1]

end:
rjmp end                 ; PC <- [PC + end + 1]