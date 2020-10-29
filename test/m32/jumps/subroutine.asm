.INCLUDE "m32def.inc"

.def temp = r18

init:                    ; SP Init
ldi r16, 0x05            ; R16 <- 0x0f
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
rjmp exit                ; PC <- PC + exit + 1

clear:
clr r16                  ; R16 <- R16 xor R16
clr r17                  ; R17 <- R17 xor R17
clr r18                  ; R18 <- R18 xor R18
ret                      ; PC <- DATA[SP]

decrement:
dec r16                  ; R16 <- R16 - 0x01
rjmp loop                ; PC <- PC + loop + 1

exit:
rcall clear              ; PC <- PC + clear
nop                      ; no instruction
nop                      ; no instruction