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
ldi r19, 0xf0            ; R19 <- 0xf0
push temp                ; DATA[SP] <- R18
push r19                 ; DATA[SP] <- R19
rjmp load                ; PC <- [PC + load + 1]

load:
ldi XL, 0x5e             ; R26 <- 0x5e
ldi XH, 0x08             ; R27 <- 0x08
ld r16, X+               ; R16 <- DATA[X+]
ld r17, X                ; R17 <- DATA[X]
rjmp end                 ; PC <- [PC + end + 1]

end:
rjmp end                 ; PC <- [PC + end + 1]
