.INCLUDE "m32def.inc"

init:                    ; SP Init
ldi r16, 0x05            ; R16 <- 0x05
rjmp stack_init          ; PC <- PC + stack_init + 1

stack_init:
ldi r18, LOW(RAMEND)     ; R18 <- LOW(RAMEND)
out SPL, r18             ; DATA[SPL] <- R18
ldi r18, HIGH(RAMEND)    ; R18 <- HIGH(RAMEND)
out SPH, r18             ; DATA[SPH] <- R18
rjmp loop                ; PC <- PC + loop + 1

loop:
cpi r16, 0x01            ; R16 - 0x01
brge subroutine          ; (N ^ V = 0): PC <- PC + subroutine + 1
rjmp exit                ; PC <- PC + exit + 1

decrement:
dec r16                  ; R16 <- R16 - 0x01
ret                      ; PC <- DATA[SP]

subroutine:
rcall decrement          ; PC <- PC + decrement + 1
rjmp loop                ; PC <- PC + loop + 1

exit:
nop                      ; no instruction