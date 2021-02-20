.INCLUDE "m32def.inc"

; Simple Logical Instruction Test

init:
ldi r16, 0x00       ; R16 <- 0x00
ldi r17, 0x10       ; R16 <- 0x01
rjmp loop           ; PC <- PC + loop + 1

loop:
ori r16, 0x00       ; R16 <- R16 | 0x00
ori r16, 0x01       ; R16 <- R16 | 0x01
ori r16, 0x02       ; R16 <- R16 | 0x02
ori r16, 0x04       ; R16 <- R16  
rjmp next           ; PC <- PC + next + 1

next:
or r16, r17         ; R16 <- R16 | R17
rjmp exit           ; PC <- PC + exit + 1

exit:
nop      ; no instruction