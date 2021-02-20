.INCLUDE "m32def.inc"

.def temp = r18
.def incr = r19

init:                    ; SP Init
ldi r16, 0x05            ; R16 <- 0x0f
ldi incr, 0x01           ; R19 <- 0x01
rjmp useless             ; PC <- PC + useless + 1 

useless:
ldi temp, HIGH(RAMEND)   ; R18 <- H(RAMEND)
out SPH, temp            ; [SRAM + 0x20 + SPH] <- R18
ldi temp, LOW(RAMEND)    ; R18 <- L(RAMEND)
out SPL, temp            ; [SRAM + 0x20 + SPL] <- R18
rjmp loop                ; PC <- PC + loop + 1

loop:
cpi r16, 0x00            ; R17 - 0x1f
brne decrement           ; if (Z = 0): PC <- PC + decrement + 1
rjmp second              ; PC <- PC + exit + 1

decrement:
dec r16                  ; R16 <- R16 - 0x01
rjmp loop                ; PC <- PC + loop + 1

increment:
add r16, incr            ; R16 <- R16 + R19
rjmp second              ; PC <- PC + second + 1

second:
cpi r16, 0x05            ; R16 - 0x05
breq exit                ; if (Z = 1): PC <- PC + exit + 1
rjmp increment           ; PC <- PC + increment + 1

exit:
nop