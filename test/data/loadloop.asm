.INCLUDE "m32def.inc"

gpr_init:
ldi XL, 0x60             ; GPR[XL] <- 0x5e
ldi XH, 0x08             ; GPR[XH] <- 0x08
rjmp stack_init          ; PC <- PC + stack_init + 1

stack_init:          
ldi r18, HIGH(RAMEND)    ; R18 <- H(RAMEND)
out SPH, r18             ; [SRAM + 0x20 + SPH] <- R18
ldi r18, LOW(RAMEND)     ; R18 <- L(RAMEND)
out SPL, r18             ; [SRAM + 0x20 + SPL] <- R18
rjmp put                 ; PC <- [PC + put + 1]

put:
cpi r16, 0x21            ; R16 - 0x21
breq get                 ; PC <- PC + get + 1
push r16                 ; DATA[SP] <- R16
inc r16                  ; R16 <- R16 + 0x01
rjmp put                 ; PC <- PC + put + 1

get:
cpi r16, 0x00            ; R16 - 0x00
breq end                 ; PC <- PC + end + 1
rcall load               ; PC <- PC + load + 1
rjmp get                 ; PC <- [PC + get + 1]

load:
dec r16                  ; R16 <- R16 - 0x01
ld r20, -X               ; R20 <- DATA[-X]
ret                      ; PC <- DATA[SP]

end:
nop                      ; no instruction
