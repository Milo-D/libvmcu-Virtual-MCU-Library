.INCLUDE "m32def.inc"

.def temp = r18

init:                         ; SP Init
ldi r20, 0x3e                 ; R20 <- 0x3e
ldi temp, HIGH(RAMEND - 4)    ; R18 <- H(RAMEND)
out SPH, temp                 ; [SRAM + 0x20 + SPH] <- R18
ldi temp, LOW(RAMEND - 4)     ; R18 <- L(RAMEND)
out SPL, temp                 ; [SRAM + 0x20 + SPL] <- R18

start:
push r20                      ; [SRAM + SP] <- R20 
ldi r16, (0x01 << TXEN)       ; R16 <- (0x01 << TXEN)
push r16                      ; [SRAM + SP] <- R17
push r18                      ; [SRAM + SP] <- R16
clr r16                       ; R16 <- 0x00
clr temp                      ; R18 <- 0x00
rjmp delete                   ; PC <- PC + [delete] + 1 

delete:
pop r17                       ; R17 <- [SRAM + SP]
pop r16                       ; R16 <- [SRAM + SP]
rjmp loop                     ; PC <- PC + [loop] + 1

loop:
rjmp start                    ; PC <- PC + [start] + 1