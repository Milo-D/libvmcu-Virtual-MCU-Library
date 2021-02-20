L6:
jmp L0                    ; PC <- 0x34
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46
jmp L1                    ; PC <- 0x46

L0:
eor r1, r1                ; R1 xor R1
out 0x3f, r1              ; IO[addr] <- R1
ldi r28, 0xff             ; R28 <- 0xff
ldi r29, 0x08             ; R29 <- 0x08
out 0x3e, r29             ; IO[addr] <- R29
out 0x3d, r28             ; IO[addr] <- R28
ldi r18, 0x01             ; R18 <- 0x01
ldi r26, 0x00             ; R26 <- 0x00
ldi r27, 0x01             ; R27 <- 0x01
rjmp L2                   ; PC <- PC + 0x1 + 1

L3:
st X+, r1                 ; DATA[X+] <- R1

L2:
cpi r26, 0x01             ; R26 - 0x01
cpc r27, r18              ; R27 - R18 - CF
brne L3                   ; (Z = 0): PC <- PC - 0x4 + 1
call L4                   ; PC <- 0x48
jmp L5                    ; PC <- 0x5c

L1:
jmp L6                    ; PC <- 0x0

L4:
ldi r24, 0x80             ; R24 <- 0x80
out 0x27, r24             ; IO[addr] <- R24
in r24, 0x24              ; R24 <- IO[addr]
ori r24, 0x40             ; R24 <- R24 | 0x40
out 0x24, r24             ; IO[addr] <- R24
in r24, 0x25              ; R24 <- IO[addr]
ori r24, 0x01             ; R24 <- R24 | 0x01
out 0x25, r24             ; IO[addr] <- R24
sei                       ; IF <- 0x01
ldi r24, 0xff             ; R24 <- 0xff
out 0x04, r24             ; IO[addr] <- R24
sbi 0x0a, 6               ; IO[0a, 6] <- 0x01

L7:
in r0, 0x26               ; R0 <- IO[addr]
sbrs r0, 7                ; (R0[7] = 1): PC <- skip
rjmp L7                   ; PC <- PC - 0x3 + 1
in r24, 0x25              ; R24 <- IO[addr]
ori r24, 0x80             ; R24 <- R24 | 0x80
out 0x25, r24             ; IO[addr] <- R24
out 0x26, r1              ; IO[addr] <- R1
rjmp L7                   ; PC <- PC - 0x8 + 1

L5:
cli                       ; IF <- 0x00

L8:
rjmp L8                   ; PC <- PC - 0x1 + 1
