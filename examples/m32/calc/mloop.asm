.INCLUDE "m32def.inc"

; Simple loop that calculates
; (x * y), going from x = y = 1 to x = y = 10

init:
ldi r16, 0x01              ; R16 <- 0x01
ldi r17, 0x01              ; R17 <- 0x01
rjmp loop                  ; PC <- PC + loop + 1

loop:
muls r16, r17              ; R1:R0 <- R16 * R17
cpi r17, 0x0a              ; R17 - 0x0a
breq increment             ; (Z = 1): PC <- PC + increment + 1
inc r17                    ; R17 <- R17 + 0x01
rjmp loop                  ; PC <- PC + loop + 1

increment:
cpi r16, 0x0a              ; R16 - 0x0a
breq exit                  ; (Z = 1): PC <- PC + exit + 1
ldi r17, 0x01              ; R17 <- 0x01
inc r16                    ; R16 <- R16 + 0x01
rjmp loop                  ; PC <- PC + loop + 1

exit:
nop                        ; no instruction