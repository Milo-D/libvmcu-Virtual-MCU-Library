.INCLUDE "m32def.inc"

.def counter = r17

gpr_init:
ldi r16, 0x00             ; R16 <- 0x00
ldi counter, 0x00         ; R17 <- 0x00
rjmp stack_init           ; PC <- PC + stack_init + 1

stack_init:
ldi r18, LOW(RAMEND)      ; R18 <- LOW(RAMEND)
out SPL, r18              ; DATA[SPL] <- R18
ldi r18, HIGH(RAMEND)     ; R18 <- HIGH(RAMEND)
out SPH, r18              ; DATA[SPH] <- R18
rjmp loop                 ; PC <- PC + loop + 1

loop:
com r16                   ; R16 <- 0xff - R16
cpi counter, 0xff         ; R17 - 0xff
breq exit                 ; (Z = 1): PC <- PC + exit + 1
rcall increment           ; PC <- PC + increment + 1
rjmp loop                 ; PC <- PC + loop + 1

increment:
inc counter               ; R17 <- R17 + 0x01
mov r16, counter          ; R16 <- R17
ret                       ; PC <- DATA[--SP]

exit:
nop                       ; no instruction