.INCLUDE "tn45def.inc"

; This small example program was transpiled by the SmallDragon Transpiler
; (https://github.com/pointbazaar/smalldragon/)
;
;      SmallDragon        AVR-GCC
; .dg --------------> .c ----------> .hex
;
; Also we had to include our own (naive) memory allocation (found in dfs.c),
; before using avr-gcc.
;
;
; This example uses elements of functional programming in order to find a value 
; in a binarytree. This is done by a recursive DFS (left-first traversal).
;
; The checker-function (check7) is called on each node and returns true if the
; node's value is 7. If the binarytree contains a 7 (it does), the value 0x0b
; will be pushed onto the stack at address 0x015a.
;
; You can load this program into MDX by compiling MDX against ATtiny45 architecture
; and then typing: ./mdx dfs.hex
;

; This assembly file was disassembled by MDX.

L4:
rjmp L0                   ; PC <- PC + 0xe + 1
rjmp L1                   ; PC <- PC + 0x15 + 1
rjmp L1                   ; PC <- PC + 0x14 + 1
rjmp L1                   ; PC <- PC + 0x13 + 1
rjmp L1                   ; PC <- PC + 0x12 + 1
rjmp L1                   ; PC <- PC + 0x11 + 1
rjmp L1                   ; PC <- PC + 0x10 + 1
rjmp L1                   ; PC <- PC + 0xf + 1
rjmp L1                   ; PC <- PC + 0xe + 1
rjmp L1                   ; PC <- PC + 0xd + 1
rjmp L1                   ; PC <- PC + 0xc + 1
rjmp L1                   ; PC <- PC + 0xb + 1
rjmp L1                   ; PC <- PC + 0xa + 1
rjmp L1                   ; PC <- PC + 0x9 + 1
rjmp L1                   ; PC <- PC + 0x8 + 1

L0:
clr r1                    ; R1 xor R1
out 0x3f, r1              ; IO[addr] <- R1
ldi r28, 0x5f             ; R28 <- 0x5f
ldi r29, 0x01             ; R29 <- 0x01
out 0x3e, r29             ; IO[addr] <- R29
out 0x3d, r28             ; IO[addr] <- R28
rcall L2                  ; PC <- PC + 0x186 + 1
rjmp L3                   ; PC <- PC + 0x1a8 + 1

L1:
rjmp L4                   ; PC <- PC - 0x18 + 1
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
ldi r24, 0x60             ; R24 <- 0x60
ldi r25, 0x00             ; R25 <- 0x00
ldi r18, 0x64             ; R18 <- 0x64
ldi r19, 0x00             ; R19 <- 0x00
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+1, r19              ; DATA[Z+1] <- R19
std Z+0, r18              ; DATA[Z+0] <- R18
nop                       ; no operation
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L9:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
sbiw r29:r28, 0x08        ; R29:R28 <- R29:R28 - 0x08
in r0, 0x3f               ; R0 <- IO[addr]
cli                       ; IF <- 0x00
out 0x3e, r29             ; IO[addr] <- R29
out 0x3f, r0              ; IO[addr] <- R0
out 0x3d, r28             ; IO[addr] <- R28
std Y+8, r25              ; DATA[Y+8] <- R25
std Y+7, r24              ; DATA[Y+7] <- R24
ldi r24, 0x60             ; R24 <- 0x60
ldi r25, 0x00             ; R25 <- 0x00
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+0              ; R24 <- DATA[Z+0]
ldd r25, Z+1              ; R25 <- DATA[Z+1]
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+7              ; R24 <- DATA[Y+7]
ldd r25, Y+8              ; R25 <- DATA[Y+8]
ldd r18, Y+1              ; R18 <- DATA[Y+1]
ldd r19, Y+2              ; R19 <- DATA[Y+2]
add r24, r18              ; R24 <- R24 + R18
adc r25, r19              ; R25 <- R25 + R19 + CF
std Y+4, r25              ; DATA[Y+4] <- R25
std Y+3, r24              ; DATA[Y+3] <- R24
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
cpi r24, 0xe1             ; R24 - 0xe1
cpc r25, r1               ; R25 - R1 - CF
brcc L5                   ; (C = 0): PC <- PC + 0x12 + 1
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
std Y+6, r25              ; DATA[Y+6] <- R25
std Y+5, r24              ; DATA[Y+5] <- R24
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldi r24, 0x60             ; R24 <- 0x60
ldi r25, 0x00             ; R25 <- 0x00
ldd r18, Y+1              ; R18 <- DATA[Y+1]
ldd r19, Y+2              ; R19 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+1, r19              ; DATA[Z+1] <- R19
std Z+0, r18              ; DATA[Z+0] <- R18
ldd r24, Y+5              ; R24 <- DATA[Y+5]
ldd r25, Y+6              ; R25 <- DATA[Y+6]
rjmp L6                   ; PC <- PC + 0x2 + 1

L5:
ldi r24, 0x00             ; R24 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00

L6:
adiw r29:r28, 0x08        ; R29:R28 <- R29:R28 + 0x08
in r0, 0x3f               ; R0 <- IO[addr]
cli                       ; IF <- 0x00
out 0x3e, r29             ; IO[addr] <- R29
out 0x3f, r0              ; IO[addr] <- R0
out 0x3d, r28             ; IO[addr] <- R28
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L11:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L7                  ; PC <- PC + 0x0 + 1

L7:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
nop                       ; no operation
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L18:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L8                  ; PC <- PC + 0x0 + 1

L8:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
ldi r24, 0x08             ; R24 <- 0x08
ldi r25, 0x00             ; R25 <- 0x00
rcall L9                  ; PC <- PC - 0x52 + 1
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L10                 ; PC <- PC + 0x0 + 1

L10:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
rcall L11                 ; PC <- PC - 0x28 + 1
nop                       ; no operation
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L12                 ; PC <- PC + 0x0 + 1

L12:
rcall L13                 ; PC <- PC + 0x0 + 1

L13:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
std Y+4, r25              ; DATA[Y+4] <- R25
std Y+3, r24              ; DATA[Y+3] <- R24
ldi r24, 0x08             ; R24 <- 0x08
ldi r25, 0x00             ; R25 <- 0x00
rcall L9                  ; PC <- PC - 0x76 + 1
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r18, Z+0              ; R18 <- DATA[Z+0]
ldd r19, Z+1              ; R19 <- DATA[Z+1]
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+1, r19              ; DATA[Z+1] <- R19
std Z+0, r18              ; DATA[Z+0] <- R18
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r18, Z+2              ; R18 <- DATA[Z+2]
ldd r19, Z+3              ; R19 <- DATA[Z+3]
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+3, r19              ; DATA[Z+3] <- R19
std Z+2, r18              ; DATA[Z+2] <- R18
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r18, Z+4              ; R18 <- DATA[Z+4]
ldd r19, Z+5              ; R19 <- DATA[Z+5]
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+5, r19              ; DATA[Z+5] <- R19
std Z+4, r18              ; DATA[Z+4] <- R18
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r18, Z+6              ; R18 <- DATA[Z+6]
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+6, r18              ; DATA[Z+6] <- R18
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r18, Z+7              ; R18 <- DATA[Z+7]
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+7, r18              ; DATA[Z+7] <- R18
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L14                 ; PC <- PC + 0x0 + 1

L14:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+0              ; R24 <- DATA[Z+0]
ldd r25, Z+1              ; R25 <- DATA[Z+1]
ldi r18, 0x01             ; R18 <- 0x01
sbiw r25:r24, 0x07        ; R25:R24 <- R25:R24 - 0x07
breq L15                  ; (Z = 1): PC <- PC + 0x1 + 1
ldi r18, 0x00             ; R18 <- 0x00

L15:
mov r24, r18              ; R24 <- R18
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L21:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L16                 ; PC <- PC + 0x0 + 1

L16:
rcall L17                 ; PC <- PC + 0x0 + 1

L17:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
std Y+4, r25              ; DATA[Y+4] <- R25
std Y+3, r24              ; DATA[Y+3] <- R24
rcall L18                 ; PC <- PC - 0x84 + 1
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
ldd r18, Y+3              ; R18 <- DATA[Y+3]
ldd r19, Y+4              ; R19 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+1, r19              ; DATA[Z+1] <- R19
std Z+0, r18              ; DATA[Z+0] <- R18
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+6, r1               ; DATA[Z+6] <- R1
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+7, r1               ; DATA[Z+7] <- R1
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L34:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L19                 ; PC <- PC + 0x0 + 1

L19:
rcall L20                 ; PC <- PC + 0x0 + 1

L20:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
ldi r24, 0x06             ; R24 <- 0x06
ldi r25, 0x00             ; R25 <- 0x00
rcall L21                 ; PC <- PC - 0x2c + 1
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldi r24, 0x07             ; R24 <- 0x07
ldi r25, 0x00             ; R25 <- 0x00
rcall L21                 ; PC <- PC - 0x31 + 1
std Y+4, r25              ; DATA[Y+4] <- R25
std Y+3, r24              ; DATA[Y+3] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
ldd r18, Y+3              ; R18 <- DATA[Y+3]
ldd r19, Y+4              ; R19 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+5, r19              ; DATA[Z+5] <- R19
std Z+4, r18              ; DATA[Z+4] <- R18
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
ldi r18, 0x01             ; R18 <- 0x01
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+7, r18              ; DATA[Z+7] <- R18
ldi r24, 0x08             ; R24 <- 0x08
ldi r25, 0x00             ; R25 <- 0x00
rcall L21                 ; PC <- PC - 0x42 + 1
movw r19:r18, r25:r24     ; R19:R18 <- R25:R24
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+3, r19              ; DATA[Z+3] <- R19
std Z+2, r18              ; DATA[Z+2] <- R18
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
ldi r18, 0x01             ; R18 <- 0x01
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+6, r18              ; DATA[Z+6] <- R18
ldi r24, 0x09             ; R24 <- 0x09
ldi r25, 0x00             ; R25 <- 0x00
rcall L21                 ; PC <- PC - 0x50 + 1
movw r19:r18, r25:r24     ; R19:R18 <- R25:R24
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+5, r19              ; DATA[Z+5] <- R19
std Z+4, r18              ; DATA[Z+4] <- R18
ldd r24, Y+3              ; R24 <- DATA[Y+3]
ldd r25, Y+4              ; R25 <- DATA[Y+4]
ldi r18, 0x01             ; R18 <- 0x01
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+7, r18              ; DATA[Z+7] <- R18
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L27:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L22                 ; PC <- PC + 0x0 + 1

L22:
rcall L23                 ; PC <- PC + 0x0 + 1

L23:
push r1                   ; DATA[SP] <- R1
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
std Y+3, r25              ; DATA[Y+3] <- R25
std Y+2, r24              ; DATA[Y+2] <- R24
std Y+5, r23              ; DATA[Y+5] <- R23
std Y+4, r22              ; DATA[Y+4] <- R22
ldd r24, Y+2              ; R24 <- DATA[Y+2]
ldd r25, Y+3              ; R25 <- DATA[Y+3]
ldd r18, Y+4              ; R18 <- DATA[Y+4]
ldd r19, Y+5              ; R19 <- DATA[Y+5]
movw r31:r30, r19:r18     ; R31:R30 <- R19:R18
icall                     ; PC <- ZH:ZL
and r24, r24              ; R24 <- R24 & R24
breq L24                  ; (Z = 1): PC <- PC + 0x2 + 1
ldi r24, 0x01             ; R24 <- 0x01
rjmp L25                  ; PC <- PC + 0x2f + 1

L24:
std Y+1, r1               ; DATA[Y+1] <- R1
ldd r24, Y+2              ; R24 <- DATA[Y+2]
ldd r25, Y+3              ; R25 <- DATA[Y+3]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+6              ; R24 <- DATA[Z+6]
and r24, r24              ; R24 <- R24 & R24
breq L26                  ; (Z = 1): PC <- PC + 0xa + 1
ldd r24, Y+2              ; R24 <- DATA[Y+2]
ldd r25, Y+3              ; R25 <- DATA[Y+3]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+2              ; R24 <- DATA[Z+2]
ldd r25, Z+3              ; R25 <- DATA[Z+3]
ldd r18, Y+4              ; R18 <- DATA[Y+4]
ldd r19, Y+5              ; R19 <- DATA[Y+5]
movw r23:r22, r19:r18     ; R23:R22 <- R19:R18
rcall L27                 ; PC <- PC - 0x25 + 1
std Y+1, r24              ; DATA[Y+1] <- R24

L26:
ldd r24, Y+2              ; R24 <- DATA[Y+2]
ldd r25, Y+3              ; R25 <- DATA[Y+3]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+7              ; R24 <- DATA[Z+7]
and r24, r24              ; R24 <- R24 & R24
breq L28                  ; (Z = 1): PC <- PC + 0x17 + 1
ldd r24, Y+1              ; R24 <- DATA[Y+1]
and r24, r24              ; R24 <- R24 & R24
brne L29                  ; (Z = 0): PC <- PC + 0xb + 1
ldd r24, Y+2              ; R24 <- DATA[Y+2]
ldd r25, Y+3              ; R25 <- DATA[Y+3]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+4              ; R24 <- DATA[Z+4]
ldd r25, Z+5              ; R25 <- DATA[Z+5]
ldd r18, Y+4              ; R18 <- DATA[Y+4]
ldd r19, Y+5              ; R19 <- DATA[Y+5]
movw r23:r22, r19:r18     ; R23:R22 <- R19:R18
rcall L27                 ; PC <- PC - 0x38 + 1
and r24, r24              ; R24 <- R24 & R24
breq L30                  ; (Z = 1): PC <- PC + 0x3 + 1

L29:
ldi r24, 0x01             ; R24 <- 0x01
ldi r25, 0x00             ; R25 <- 0x00
rjmp L31                  ; PC <- PC + 0x2 + 1

L30:
ldi r24, 0x00             ; R24 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00

L31:
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
andi r24, 0x01            ; R24 <- R24 & 0x01
std Y+1, r24              ; DATA[Y+1] <- R24

L28:
ldd r24, Y+1              ; R24 <- DATA[Y+1]

L25:
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L2:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L32                 ; PC <- PC + 0x0 + 1

L32:
rcall L33                 ; PC <- PC + 0x0 + 1

L33:
push r1                   ; DATA[SP] <- R1
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
rcall L34                 ; PC <- PC - 0x95 + 1
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
ldi r22, 0xd6             ; R22 <- 0xd6
ldi r23, 0x00             ; R23 <- 0x00
rcall L27                 ; PC <- PC - 0x5b + 1
std Y+3, r24              ; DATA[Y+3] <- R24
std Y+5, r1               ; DATA[Y+5] <- R1
std Y+4, r1               ; DATA[Y+4] <- R1
ldd r24, Y+3              ; R24 <- DATA[Y+3]
and r24, r24              ; R24 <- R24 & R24
breq L35                  ; (Z = 1): PC <- PC + 0x4 + 1
ldi r24, 0x0b             ; R24 <- 0x0b
ldi r25, 0x00             ; R25 <- 0x00
std Y+5, r25              ; DATA[Y+5] <- R25
std Y+4, r24              ; DATA[Y+4] <- R24

L35:
ldi r24, 0x00             ; R24 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L3:
cli                       ; IF <- 0x00

L36:
rjmp L36                  ; PC <- PC - 0x1 + 1
