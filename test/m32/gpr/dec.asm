.INCLUDE "m32def.inc"

; Testing new Hex Converter

init:
ldi r16, 0xff          ; R16 <- 0xff
rjmp loop              ; PC <- PC + loop + 1

loop:
cpi r16, 0x00          ; R16 - 0x00
brne rep               ; Z = 0: PC <- PC + rep + 1
rjmp exit              ; PC <- PC + exit + 1

rep:
dec r16                ; R16 <- R16 - 0x01
rjmp loop              ; PC <- PC + loop + 1

exit:
nop                    ; no instruction