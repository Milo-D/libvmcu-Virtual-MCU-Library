.INCLUDE "m32def.inc"

; Testing new Hex Converter

init:
ldi r16, 0xf0          ; R16 <- 0xf0
ldi r18, LOW(RAMEND)   ; R18 <- LOW(RAMEND)
out SPL, r18           ; DATA[SPL] <- R18
ldi r18, HIGH(RAMEND)  ; R18 <- HIGH(RAMEND)
out SPH, r18           ; DATA[SPH] <- R18
rjmp loop              ; PC <- PC + loop + 1

loop:
cpi r16, 0xff          ; R16 - 0x00
brne rep               ; Z = 0: PC <- PC + rep + 1
rjmp exit              ; PC <- PC + exit + 1

rep:
rcall sub_inc          ; PC <- PC + sub_inc + 1
rjmp loop              ; PC <- PC + loop + 1

sub_inc:
inc r16                ; R16 <- R16 - 0x01
ret                    ; PC <- DATA[SP]

exit:
nop                    ; no instruction