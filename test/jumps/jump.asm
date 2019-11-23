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
push r17                 ; [SRAM + SP] <- R17
push r16                 ; [SRAM + SP] <- R16
push r17                 ; [SRAM + SP] <- R17
push r16                 ; [SRAM + SP] <- R16
push r17                 ; [SRAM + SP] <- R17
push r16                 ; [SRAM + SP] <- R16
push r17                 ; [SRAM + SP] <- R17
push r16                 ; [SRAM + SP] <- R16
ldi r16, 0x00            ; R16 <- 0x00
ldi r17, 0x00            ; R17 <- 0x00
rjmp clear               ; PC <- PC + [clear] + 1 

clear:
pop r17                  ; R17 <- [SRAM + SP]
pop r16                  ; R16 <- [SRAM + SP]
pop r17                  ; R17 <- [SRAM + SP]
pop r16                  ; R16 <- [SRAM + SP]
pop r17                  ; R17 <- [SRAM + SP]
pop r16                  ; R16 <- [SRAM + SP]
pop r17                  ; R17 <- [SRAM + SP]
pop r16                  ; R16 <- [SRAM + SP]
rjmp start               ; PC <- PC + [start] + 1