.INCLUDE "m328Pdef.inc"

;
; KMP Algorithm Test for MDX
;
; This file contains the KMP Pattern Matching
; Algorithm. If the pattern is found, the index
; will be printed to the EEPROM at address 0x0000.
;
; Text: Knuth Morris Pratt
; Patten: is Pra
;
; Expected Result: 10 (0x0a)
;

L6:
jmp L0                    ; PC <- 0x34
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49
jmp L1                    ; PC <- 0x49

L0:
eor r1, r1                ; R1 xor R1
out 0x3f, r1              ; IO[addr] <- R1
ldi r28, 0xff             ; R28 <- 0xff
ldi r29, 0x08             ; R29 <- 0x08
out 0x3e, r29             ; IO[addr] <- R29
out 0x3d, r28             ; IO[addr] <- R28
ldi r17, 0x01             ; R17 <- 0x01
ldi r26, 0x00             ; R26 <- 0x00
ldi r27, 0x01             ; R27 <- 0x01
ldi r30, 0x84             ; R30 <- 0x84
ldi r31, 0x02             ; R31 <- 0x02
rjmp L2                   ; PC <- PC + 0x2 + 1

L3:
lpm r0, Z+                ; R0 <- FLASH[Z+]
st X+, r0                 ; DATA[X+] <- R0

L2:
cpi r26, 0x1a             ; R26 - 0x1a
cpc r27, r17              ; R27 - R17 - CF
brne L3                   ; (Z = 0): PC <- PC - 0x5 + 1
call L4                   ; PC <- 0x116
jmp L5                    ; PC <- 0x140

L1:
jmp L6                    ; PC <- 0x0

L15:
push r16                  ; DATA[SP] <- R16
push r17                  ; DATA[SP] <- R17
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
movw r29:r28, r25:r24     ; R29:R28 <- R25:R24
movw r17:r16, r23:r22     ; R17:R16 <- R23:R22
movw r27:r26, r21:r20     ; R27:R26 <- R21:R20
adiw r27:r26, 0x01        ; R27:R26 <- R27:R26 + 0x01
st X, r1                  ; DATA[X] <- R1
st -X, r1                 ; DATA[-X] <- R1
ldi r18, 0x01             ; R18 <- 0x01
ldi r19, 0x00             ; R19 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00
ldi r24, 0x00             ; R24 <- 0x00

L9:
cp r18, r16               ; R18 - R16
cpc r19, r17              ; R19 - R17 - CF
brlt L7                   ; (N ^ V = 1): PC <- PC + 0x5 + 1
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
pop r17                   ; R17 <- DATA[--SP]
pop r16                   ; R16 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L7:
movw r31:r30, r29:r28     ; R31:R30 <- R29:R28
add r30, r18              ; R30 <- R30 + R18
adc r31, r19              ; R31 <- R31 + R19 + CF
ldd r21, Z+0              ; R21 <- DATA[Z+0]
movw r31:r30, r29:r28     ; R31:R30 <- R29:R28
add r30, r24              ; R30 <- R30 + R24
adc r31, r25              ; R31 <- R31 + R25 + CF
ldd r20, Z+0              ; R20 <- DATA[Z+0]
cpse r21, r20             ; (R21 = R20): PC <- skip
rjmp L8                   ; PC <- PC + 0xb + 1
adiw r25:r24, 0x01        ; R25:R24 <- R25:R24 + 0x01
movw r31:r30, r19:r18     ; R31:R30 <- R19:R18
add r30, r30              ; R30 <- R30 + R30
adc r31, r31              ; R31 <- R31 + R31 + CF
add r30, r26              ; R30 <- R30 + R26
adc r31, r27              ; R31 <- R31 + R27 + CF
std Z+1, r25              ; DATA[Z+1] <- R25
st Z, r24                 ; DATA[Z] <- R24

L11:
subi r18, 0xff            ; R18 <- R18 - 0xff
sbci r19, 0xff            ; R19 <- R19 - 0xff - CF
rjmp L9                   ; PC <- PC - 0x1d + 1

L8:
sbiw r25:r24, 0x00        ; R25:R24 <- R25:R24 - 0x00
breq L10                  ; (Z = 1): PC <- PC + 0xa + 1
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
subi r30, 0x01            ; R30 <- R30 - 0x01
sbci r31, 0x80            ; R31 <- R31 - 0x80 - CF
add r30, r30              ; R30 <- R30 + R30
adc r31, r31              ; R31 <- R31 + R31 + CF
add r30, r26              ; R30 <- R30 + R26
adc r31, r27              ; R31 <- R31 + R27 + CF
ldd r24, Z+0              ; R24 <- DATA[Z+0]
ldd r25, Z+1              ; R25 <- DATA[Z+1]
rjmp L9                   ; PC <- PC - 0x29 + 1

L10:
movw r31:r30, r19:r18     ; R31:R30 <- R19:R18
add r30, r30              ; R30 <- R30 + R30
adc r31, r31              ; R31 <- R31 + R31 + CF
add r30, r26              ; R30 <- R30 + R26
adc r31, r27              ; R31 <- R31 + R27 + CF
std Z+1, r1               ; DATA[Z+1] <- R1
st Z, r1                  ; DATA[Z] <- R1
rjmp L11                  ; PC <- PC - 0x17 + 1

L25:
push r8                   ; DATA[SP] <- R8
push r9                   ; DATA[SP] <- R9
push r10                  ; DATA[SP] <- R10
push r11                  ; DATA[SP] <- R11
push r12                  ; DATA[SP] <- R12
push r13                  ; DATA[SP] <- R13
push r14                  ; DATA[SP] <- R14
push r15                  ; DATA[SP] <- R15
push r16                  ; DATA[SP] <- R16
push r17                  ; DATA[SP] <- R17
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L12                 ; PC <- PC + 0x0 + 1

L12:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
movw r15:r14, r25:r24     ; R15:R14 <- R25:R24
movw r11:r10, r23:r22     ; R11:R10 <- R23:R22
in r18, 0x3d              ; R18 <- IO[addr]
in r19, 0x3e              ; R19 <- IO[addr]
std Y+2, r19              ; DATA[Y+2] <- R19
std Y+1, r18              ; DATA[Y+1] <- R18
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24

L13:
ld r0, Z+                 ; R0 <- DATA[Z+]
and r0, r0                ; R0 <- R0 & R0
brne L13                  ; (Z = 0): PC <- PC - 0x3 + 1
sbiw r31:r30, 0x01        ; R31:R30 <- R31:R30 - 0x01
movw r17:r16, r31:r30     ; R17:R16 <- R31:R30
sub r16, r24              ; R16 <- R16 - R24
sbc r17, r25              ; R17 <- R17 - R25 - CF
movw r31:r30, r23:r22     ; R31:R30 <- R23:R22

L14:
ld r0, Z+                 ; R0 <- DATA[Z+]
and r0, r0                ; R0 <- R0 & R0
brne L14                  ; (Z = 0): PC <- PC - 0x3 + 1
sbiw r31:r30, 0x01        ; R31:R30 <- R31:R30 - 0x01
movw r13:r12, r31:r30     ; R13:R12 <- R31:R30
sub r12, r22              ; R12 <- R12 - R22
sbc r13, r23              ; R13 <- R13 - R23 - CF
movw r25:r24, r17:r16     ; R25:R24 <- R17:R16
add r24, r24              ; R24 <- R24 + R24
adc r25, r25              ; R25 <- R25 + R25 + CF
sub r18, r24              ; R18 <- R18 - R24
sbc r19, r25              ; R19 <- R19 - R25 - CF
in r0, 0x3f               ; R0 <- IO[addr]
cli                       ; IF <- 0x00
out 0x3e, r19             ; IO[addr] <- R19
out 0x3f, r0              ; IO[addr] <- R0
out 0x3d, r18             ; IO[addr] <- R18
in r24, 0x3d              ; R24 <- IO[addr]
in r25, 0x3e              ; R25 <- IO[addr]
adiw r25:r24, 0x01        ; R25:R24 <- R25:R24 + 0x01
movw r9:r8, r25:r24       ; R9:R8 <- R25:R24
movw r21:r20, r25:r24     ; R21:R20 <- R25:R24
movw r23:r22, r17:r16     ; R23:R22 <- R17:R16
movw r25:r24, r15:r14     ; R25:R24 <- R15:R14
call L15                  ; PC <- 0x4b
ldi r25, 0x00             ; R25 <- 0x00
ldi r24, 0x00             ; R24 <- 0x00
ldi r19, 0x00             ; R19 <- 0x00
ldi r18, 0x00             ; R18 <- 0x00

L19:
cp r18, r12               ; R18 - R12
cpc r19, r13              ; R19 - R13 - CF
brlt L16                  ; (N ^ V = 1): PC <- PC + 0x16 + 1
ldd r18, Y+1              ; R18 <- DATA[Y+1]
ldd r19, Y+2              ; R19 <- DATA[Y+2]
in r0, 0x3f               ; R0 <- IO[addr]
cli                       ; IF <- 0x00
out 0x3e, r19             ; IO[addr] <- R19
out 0x3f, r0              ; IO[addr] <- R0
out 0x3d, r18             ; IO[addr] <- R18
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
pop r17                   ; R17 <- DATA[--SP]
pop r16                   ; R16 <- DATA[--SP]
pop r15                   ; R15 <- DATA[--SP]
pop r14                   ; R14 <- DATA[--SP]
pop r13                   ; R13 <- DATA[--SP]
pop r12                   ; R12 <- DATA[--SP]
pop r11                   ; R11 <- DATA[--SP]
pop r10                   ; R10 <- DATA[--SP]
pop r9                    ; R9 <- DATA[--SP]
pop r8                    ; R8 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L16:
movw r31:r30, r15:r14     ; R31:R30 <- R15:R14
add r30, r24              ; R30 <- R30 + R24
adc r31, r25              ; R31 <- R31 + R25 + CF
ldd r21, Z+0              ; R21 <- DATA[Z+0]
movw r31:r30, r11:r10     ; R31:R30 <- R11:R10
add r30, r18              ; R30 <- R30 + R18
adc r31, r19              ; R31 <- R31 + R19 + CF
ldd r20, Z+0              ; R20 <- DATA[Z+0]
cpse r21, r20             ; (R21 = R20): PC <- skip
rjmp L17                  ; PC <- PC + 0x29 + 1
adiw r25:r24, 0x01        ; R25:R24 <- R25:R24 + 0x01
subi r18, 0xff            ; R18 <- R18 - 0xff
sbci r19, 0xff            ; R19 <- R19 - 0xff - CF
cp r16, r24               ; R16 - R24
cpc r17, r25              ; R17 - R25 - CF
brne L18                  ; (Z = 0): PC <- PC + 0x10 + 1

L22:
mov r20, r18              ; R20 <- R18
sub r20, r24              ; R20 <- R20 - R24
out 0x20, r20             ; IO[addr] <- R20
out 0x22, r1              ; IO[addr] <- R1
out 0x21, r1              ; IO[addr] <- R1
sbi 0x1f, 2               ; IO[1f, 2] <- 0x01
sbi 0x1f, 1               ; IO[1f, 1] <- 0x01

L21:
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
sbiw r31:r30, 0x01        ; R31:R30 <- R31:R30 - 0x01
add r30, r30              ; R30 <- R30 + R30
adc r31, r31              ; R31 <- R31 + R31 + CF
add r30, r8               ; R30 <- R30 + R8
adc r31, r9               ; R31 <- R31 + R9 + CF
ldd r24, Z+0              ; R24 <- DATA[Z+0]
ldd r25, Z+1              ; R25 <- DATA[Z+1]
rjmp L19                  ; PC <- PC - 0x39 + 1

L18:
cp r18, r12               ; R18 - R12
cpc r19, r13              ; R19 - R13 - CF
brge L19                  ; (N ^ V = 0): PC <- PC - 0x3c + 1
movw r31:r30, r15:r14     ; R31:R30 <- R15:R14
add r30, r24              ; R30 <- R30 + R24
adc r31, r25              ; R31 <- R31 + R25 + CF
ldd r21, Z+0              ; R21 <- DATA[Z+0]
movw r31:r30, r11:r10     ; R31:R30 <- R11:R10
add r30, r18              ; R30 <- R30 + R18
adc r31, r19              ; R31 <- R31 + R19 + CF
ldd r20, Z+0              ; R20 <- DATA[Z+0]
cp r21, r20               ; R21 - R20
brne L20                  ; (Z = 0): PC <- PC + 0x1 + 1
rjmp L19                  ; PC <- PC - 0x47 + 1

L20:
sbiw r25:r24, 0x00        ; R25:R24 <- R25:R24 - 0x00
brne L21                  ; (Z = 0): PC <- PC - 0x19 + 1
subi r18, 0xff            ; R18 <- R18 - 0xff
sbci r19, 0xff            ; R19 <- R19 - 0xff - CF
rjmp L19                  ; PC <- PC - 0x4c + 1

L17:
cp r24, r16               ; R24 - R16
cpc r25, r17              ; R25 - R17 - CF
breq L22                  ; (Z = 1): PC <- PC - 0x26 + 1
rjmp L20                  ; PC <- PC - 0x9 + 1

L4:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
sbiw r29:r28, 0x1a        ; R29:R28 <- R29:R28 - 0x1a
in r0, 0x3f               ; R0 <- IO[addr]
cli                       ; IF <- 0x00
out 0x3e, r29             ; IO[addr] <- R29
out 0x3f, r0              ; IO[addr] <- R0
out 0x3d, r28             ; IO[addr] <- R28
ldi r24, 0x13             ; R24 <- 0x13
ldi r30, 0x00             ; R30 <- 0x00
ldi r31, 0x01             ; R31 <- 0x01
movw r27:r26, r29:r28     ; R27:R26 <- R29:R28
adiw r27:r26, 0x01        ; R27:R26 <- R27:R26 + 0x01

L23:
ld r0, Z+                 ; R0 <- DATA[Z+]
st X+, r0                 ; DATA[X+] <- R0
dec r24                   ; R24 <- R24 - 0x01
brne L23                  ; (Z = 0): PC <- PC - 0x4 + 1
ldi r24, 0x07             ; R24 <- 0x07
ldi r30, 0x13             ; R30 <- 0x13
ldi r31, 0x01             ; R31 <- 0x01
movw r27:r26, r29:r28     ; R27:R26 <- R29:R28
adiw r27:r26, 0x14        ; R27:R26 <- R27:R26 + 0x14

L24:
ld r0, Z+                 ; R0 <- DATA[Z+]
st X+, r0                 ; DATA[X+] <- R0
dec r24                   ; R24 <- R24 - 0x01
brne L24                  ; (Z = 0): PC <- PC - 0x4 + 1
movw r23:r22, r29:r28     ; R23:R22 <- R29:R28
subi r22, 0xff            ; R22 <- R22 - 0xff
sbci r23, 0xff            ; R23 <- R23 - 0xff - CF
movw r25:r24, r29:r28     ; R25:R24 <- R29:R28
adiw r25:r24, 0x14        ; R25:R24 <- R25:R24 + 0x14
call L25                  ; PC <- 0x8a
ldi r25, 0x00             ; R25 <- 0x00
ldi r24, 0x00             ; R24 <- 0x00
adiw r29:r28, 0x1a        ; R29:R28 <- R29:R28 + 0x1a
in r0, 0x3f               ; R0 <- IO[addr]
cli                       ; IF <- 0x00
out 0x3e, r29             ; IO[addr] <- R29
out 0x3f, r0              ; IO[addr] <- R0
out 0x3d, r28             ; IO[addr] <- R28
ret                       ; PC <- DATA[SP]

L5:
cli                       ; IF <- 0x00

L26:
rjmp L26                  ; PC <- PC - 0x1 + 1
ori r20, 0xeb             ; R20 <- R20 | 0xeb
andi r23, 0x45            ; R23 <- R23 & 0x45
and r6, r8                ; R6 <- R6 & R8
ori r20, 0xfd             ; R20 <- R20 | 0xfd
andi r23, 0x22            ; R23 <- R23 & 0x22
andi r22, 0x39            ; R22 <- R22 & 0x39
subi r18, 0x00            ; R18 <- R18 - 0x00
ori r23, 0x12             ; R23 <- R23 | 0x12
andi r23, 0x44            ; R23 <- R23 & 0x44
ori r16, 0x90             ; R16 <- R16 | 0x90
and r7, r3                ; R7 <- R7 & R3
andi r21, 0x20            ; R21 <- R21 & 0x20
.dw 0x0061                ; 0x61
