.INCLUDE "m32def.inc"

start:              ; entry
ldi r16, 0x2c       ; R16 <- 0x2c
ldi r17, 0x1f       ; R17 <- 0x1f
push r17            ; [SRAM + SP] <- R17
mov r17, r16        ; R17 <- R16
nop                 ; no instruction
muls r16, r16       ; R16 <- R16 * R16
rjmp 0x01           ; PC <- PC + 1 (+ 1)
ldi r16, 0x11       ; R16 <- 0x11
mulsu r17, r18      ; R17 <- R17 * R18
nop                 ; no instruction
rjmp start          ; PC <- PC - 7 (+ 1) 
