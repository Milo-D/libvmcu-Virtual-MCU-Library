L0:
jmp L1                    ; PC <- 0x34
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L3                    ; PC <- 0x40
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e
jmp L2                    ; PC <- 0x3e

L1:
eor r1, r1                ; R1 xor R1
out 0x3f, r1              ; SREG
ldi r28, 0xff             ; R28 <- 0xff
ldi r29, 0x08             ; R29 <- 0x08
out 0x3e, r29             ; SPH
out 0x3d, r28             ; SPL
call L4                   ; PC <- 0x4a
jmp L6                    ; PC <- 0x58

L2:
jmp L0                    ; PC <- 0x0

L3:
push r1                   ; DATA[SP--] <- R1
push r0                   ; DATA[SP--] <- R0
in r0, 0x3f               ; SREG
push r0                   ; DATA[SP--] <- R0
eor r1, r1                ; R1 xor R1
pop r0                    ; R0 <- DATA[++SP]
out 0x3f, r0              ; SREG
pop r0                    ; R0 <- DATA[++SP]
pop r1                    ; R1 <- DATA[++SP]
reti                      ; PC <- DATA[SP]

L4:
out 0x24, r1              ; TCCR0A
out 0x25, r1              ; TCCR0B
sts 0x006e, r1            ; TIMSK0
in r24, 0x25              ; TCCR0B
ori r24, 0x03             ; R24 <- R24 | 0x03
out 0x25, r24             ; TCCR0B
lds r24, 0x006e           ; TIMSK0
ori r24, 0x01             ; R24 <- R24 | 0x01
sts 0x006e, r24           ; TIMSK0
sei                       ; IF <- 0x01

L5:
rjmp L5                   ; PC <- PC - 0x1 + 1

L6:
cli                       ; IF <- 0x00

L7:
rjmp L7                   ; PC <- PC - 0x1 + 1
