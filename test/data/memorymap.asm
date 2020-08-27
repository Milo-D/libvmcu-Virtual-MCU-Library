ldi r26, 0x1f             ; R26 <- 0x1f
clr r27                   ; R27 xor R27
ldi r31, 0x21             ; R31 <- 0x21
rjmp L0                   ; PC <- PC + 0x0 + 1

L0:
ld r16, X                 ; R16 <- DATA[X]
rjmp L1                   ; PC <- PC + 0x0 + 1

L1:
rjmp L1                   ; PC <- PC - 0x1 + 1
