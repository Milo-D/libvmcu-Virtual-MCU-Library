.INCLUDE "m32def.inc"

; OR Loop Test

.def acc = r17

gpr_init:
ldi r16, 0x00             ; R16 <- 0x00
ldi acc, 0x01             ; R17 <- 0x00
rjmp stack_init           ; PC <- PC + stack_init + 1

stack_init:
ldi r18, LOW(RAMEND)      ; R18 <- LOW(RAMEND)
out SPL, r18              ; DATA[SPL] <- R18
ldi r18, HIGH(RAMEND)     ; R18 <- HIGH(RAMEND)
out SPH, r18              ; DATA[SPH] <- R18
rjmp loop                 ; PC <- PC + loop + 1

loop:
or r16, acc               ; R16 <- R16 | R17
cpi r16, 0xff             ; R16 - 0xff
breq exit                 ; (Z = 1): PC <- PC + exit + 1
rcall increment           ; PC <- PC + increment + 1
rjmp loop                 ; PC <- PC + loop + 1

increment:
add acc, acc              ; R16 <- R16 + R17
ret                       ; PC <- DATA[--SP]

exit:
nop                       ; no instruction