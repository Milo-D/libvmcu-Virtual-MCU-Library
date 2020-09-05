.INCLUDE "tn45def.inc"

; This program was created by the SmallDragon Transpiler, which transpiled
; a .dg file into a .c file for the ATtiny45. Then we added our own small 
; (and naive) memory-management functions and finally assembled the transpiled
; file into an Intel Hex File.

; This small program demonstrates a simple memory allocation by creating
; two Nodes (structures, each 6 bytes in size) on the Heap. The stack starts
; at address 0x015f and grows towards low memory, while the heap is located
; at address 0x0064.

; In the end, n1.leftp (0x0066/0x0067) will point to n2 (0x0070).

;
;
;       ----------  0x0064
;      |   HEAP   |
;      |          |
;      |          |
;      |          |
;      |----------| 0x00e0
;      |          |
;      |          |
;      |          |
;      |  STACK   |
;       ----------  0x015f
;
;
;          NODE
;      --------------
;     | 16bit value  |
;      --------------
;     | 16bit leftp  |
;      --------------
;     | 16bit rightp |
;      --------------
;
;

; Following Code was disassembled by MDX.

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
rcall L2                  ; PC <- PC + 0xb0 + 1
rjmp L3                   ; PC <- PC + 0xd5 + 1

L1:
rjmp L4                   ; PC <- PC - 0x18 + 1

L16:
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

L17:
push r28                  ; DATA[SP] <- R28
push r29                  ; DATA[SP] <- R29
rcall L8                  ; PC <- PC + 0x0 + 1

L8:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
ldi r24, 0x06             ; R24 <- 0x06
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

L18:
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
ldi r24, 0x06             ; R24 <- 0x06
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
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
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
rcall L14                 ; PC <- PC + 0x0 + 1

L14:
rcall L15                 ; PC <- PC + 0x0 + 1

L15:
in r28, 0x3d              ; R28 <- IO[addr]
in r29, 0x3e              ; R29 <- IO[addr]
rcall L16                 ; PC <- PC - 0xb5 + 1
rcall L17                 ; PC <- PC - 0x5d + 1
std Y+2, r25              ; DATA[Y+2] <- R25
std Y+1, r24              ; DATA[Y+1] <- R24
rcall L17                 ; PC <- PC - 0x60 + 1
std Y+4, r25              ; DATA[Y+4] <- R25
std Y+3, r24              ; DATA[Y+3] <- R24
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
ldd r18, Y+3              ; R18 <- DATA[Y+3]
ldd r19, Y+4              ; R19 <- DATA[Y+4]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
std Z+3, r19              ; DATA[Z+3] <- R19
std Z+2, r18              ; DATA[Z+2] <- R18
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
movw r31:r30, r25:r24     ; R31:R30 <- R25:R24
ldd r24, Z+2              ; R24 <- DATA[Z+2]
ldd r25, Z+3              ; R25 <- DATA[Z+3]
rcall L18                 ; PC <- PC - 0x5e + 1
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
rcall L18                 ; PC <- PC - 0x61 + 1
ldi r24, 0x00             ; R24 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r0                    ; R0 <- DATA[--SP]
pop r29                   ; R29 <- DATA[--SP]
pop r28                   ; R28 <- DATA[--SP]
ret                       ; PC <- DATA[SP]

L3:
cli                       ; IF <- 0x00

L19:
rjmp L19                  ; PC <- PC - 0x1 + 1
