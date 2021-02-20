L4:
jmp L0                    ; PC <- 0x34
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e
jmp L1                    ; PC <- 0x3e

L0:
eor r1, r1                ; R1 xor R1
out 0x3f, r1              ; IO[addr] <- R1
ldi r28, 0xff             ; R28 <- 0xff
ldi r29, 0x08             ; R29 <- 0x08
out 0x3e, r29             ; IO[addr] <- R29
out 0x3d, r28             ; IO[addr] <- R28
call L2                   ; PC <- 0x40
jmp L3                    ; PC <- 0x49

L1:
jmp L4                    ; PC <- 0x0

L2:
ldi r30, 0x20             ; R30 <- 0x20
ldi r31, 0x00             ; R31 <- 0x00

L5:
ld r24, Z+                ; R24 <- DATA[Z+]
cpi r30, 0xc7             ; R30 - 0xc7
cpc r31, r1               ; R31 - R1 - CF
brne L5                   ; (Z = 0): PC <- PC - 0x4 + 1
ldi r25, 0x00             ; R25 <- 0x00
ldi r24, 0x00             ; R24 <- 0x00
ret                       ; PC <- DATA[SP]

L3:
cli                       ; IF <- 0x00

L6:
rjmp L6                   ; PC <- PC - 0x1 + 1
