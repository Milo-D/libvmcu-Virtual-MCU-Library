.INCLUDE "m32def.inc"

start:              ; entry
ldi r16, 0x2c       ; R16 <- 0x2c
ldi r17, 0x1f       ; R17 <- 0x1f
push r17            ; [SRAM + SP] <- R17
push r16            ; [SRAM + SP] <- R16
push r17            ; [SRAM + SP] <- R17
push r16            ; [SRAM + SP] <- R16
push r17            ; [SRAM + SP] <- R17
push r16            ; [SRAM + SP] <- R16
push r17            ; [SRAM + SP] <- R17
push r16            ; [SRAM + SP] <- R16
ldi r16, 0x00       ; R16 <- 0x00
ldi r17, 0x00       ; R17 <- 0x00
