L7:
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
jmp L2                    ; PC <- 0x48
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
rjmp L3                   ; PC <- PC + 0x1 + 1

L4:
st X+, r1                 ; DATA[X+] <- R1

L3:
cpi r26, 0x01             ; R26 - 0x01
cpc r27, r18              ; R27 - R18 - CF
brne L4                   ; (Z = 0): PC <- PC - 0x4 + 1
call L5                   ; PC <- 0x58
jmp L6                    ; PC <- 0x69

L1:
jmp L7                    ; PC <- 0x0

L2:
push r24                  ; DATA[SP] <- R24
in r24, 0x3f              ; R24 <- IO[addr]
push r24                  ; DATA[SP] <- R24
lds r24, 0x0100           ; R24 <- DATA[0x100]
com r24                   ; R24 <- 0xff - R24
out 0x05, r24             ; IO[addr] <- R24
lds r24, 0x0100           ; R24 <- DATA[0x100]
com r24                   ; R24 <- 0xff - R24
sts 0x0100, r24           ; DATA[0x100] <- R24
pop r24                   ; R24 <- DATA[--SP]
out 0x3f, r24             ; IO[addr] <- R24
pop r24                   ; R24 <- DATA[--SP]
reti                      ; PC <- DATA[SP]

L5:
ldi r24, 0x80             ; R24 <- 0x80
out 0x27, r24             ; IO[addr] <- R24
lds r24, 0x006e           ; R24 <- DATA[0x6e]
ori r24, 0x02             ; R24 <- R24 | 0x02
sts 0x006e, r24           ; DATA[0x6e] <- R24
in r24, 0x24              ; R24 <- IO[addr]
ori r24, 0x02             ; R24 <- R24 | 0x02
out 0x24, r24             ; IO[addr] <- R24
in r24, 0x25              ; R24 <- IO[addr]
ori r24, 0x01             ; R24 <- R24 | 0x01
out 0x25, r24             ; IO[addr] <- R24
sei                       ; IF <- 0x01
ldi r24, 0xff             ; R24 <- 0xff
out 0x04, r24             ; IO[addr] <- R24

L8:
rjmp L8                   ; PC <- PC - 0x1 + 1

L6:
cli                       ; IF <- 0x00

L9:
rjmp L9                   ; PC <- PC - 0x1 + 1
