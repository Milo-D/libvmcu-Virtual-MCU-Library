.INCLUDE "m32def.inc"

; Testing new Hex Converter

.def acc = r16
.def temp = r18

init:                    ; SP Init
ldi temp, HIGH(RAMEND)   ; R18 <- H(RAMEND)
out SPH, temp            ; [SRAM + 0x20 + SPH] <- R18
ldi temp, LOW(RAMEND)    ; R18 <- L(RAMEND)
out SPL, temp            ; [SRAM + 0x20 + SPL] <- R18
ldi acc, 0xff            ; R16 <- 0xff
rjmp push_loop           ; PC <- [PC + push_loop + 1]

push_loop:
push acc                 ; DATA[SP] <- R18
brne rep                 ; Z = 0 : PC <- [PC + rep + 1]
rjmp pop_loop            ; PC <- [PC + pop_loop + 1]

pop_loop:
pop acc                  ; R16 <- DATA[SP]
cpi acc, 0xff            ; R16 - 0xff 
breq exit                ; (Z = 1): PC <- PC + exit + 1
rjmp pop_loop            ; PC <- PC + pop_loop + 1

rep:
dec acc                  ; R16 <- R16 - 0x01
rjmp push_loop           ; PC <- [PC + push_loop + 1]

exit:
nop                      ; no instruction