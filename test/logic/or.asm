.INCLUDE "m32def.inc"

; Simple Logical Instruction Test

init:
ldi r16, 0x00       ; R16 <- 0x00
rjmp loop           ; PC <- PC + loop + 1

loop:
ori r16, 0x00       ; R16 <- R16 | 0x00
ori r16, 0x01       ; R16 <- R16 | 0x01
ori r16, 0x02       ; R16 <- R16 | 0x02
ori r16, 0x04       ; R16 <- R16  
rjmp exit

exit:
nop      ; no instruction