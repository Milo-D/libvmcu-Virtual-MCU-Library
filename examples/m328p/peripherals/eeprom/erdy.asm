.INCLUDE "m328Pdef.inc"

;
; EEPROM - ERDY IRQ Test
;
; This test covers the ERDY Interrupt.
; Whenever the EEPROM Module is ready (EEPE=0) and
; ERDY interrupts are enabled (EERIE=1), a call to
; the corresponding ISR occurs.
;
; This test is writing the string "ERDY_Test", char by char,
; to the EEPROM. If a write access is finished (3.3ms), the hardware
; clears the EEPE bit in EECR, and an ERDY interrupt occurs,
; so the next letter can be written to the EEPROM Module.
;
; Once all chars are written, the EERIE bit is cleared, so that no
; further write access can be initiated.
;
; This program was written in C, compiled by avr-gcc, and finally
; disassembled by MDX.
;

L9:
jmp L0                    ; PC <- 0x34
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L2                    ; PC <- 0x53
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51
jmp L1                    ; PC <- 0x51

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
ldi r30, 0x10             ; R30 <- 0x10
ldi r31, 0x01             ; R31 <- 0x01
rjmp L3                   ; PC <- PC + 0x2 + 1

L4:
lpm r0, Z+                ; R0 <- FLASH[Z+]
st X+, r0                 ; DATA[X+] <- R0

L3:
cpi r26, 0x0c             ; R26 - 0x0c
cpc r27, r17              ; R27 - R17 - CF
brne L4                   ; (Z = 0): PC <- PC - 0x5 + 1
ldi r18, 0x01             ; R18 <- 0x01
ldi r26, 0x0c             ; R26 <- 0x0c
ldi r27, 0x01             ; R27 <- 0x01
rjmp L5                   ; PC <- PC + 0x1 + 1

L6:
st X+, r1                 ; DATA[X+] <- R1

L5:
cpi r26, 0x0d             ; R26 - 0x0d
cpc r27, r18              ; R27 - R18 - CF
brne L6                   ; (Z = 0): PC <- PC - 0x4 + 1
call L7                   ; PC <- 0x82
jmp L8                    ; PC <- 0x86

L1:
jmp L9                    ; PC <- 0x0

L2:
push r1                   ; DATA[SP] <- R1
in r1, 0x3f               ; R1 <- IO[addr]
push r1                   ; DATA[SP] <- R1
eor r1, r1                ; R1 xor R1
push r24                  ; DATA[SP] <- R24
push r25                  ; DATA[SP] <- R25
push r30                  ; DATA[SP] <- R30
push r31                  ; DATA[SP] <- R31
lds r24, 0x010c           ; R24 <- DATA[0x10c]
ldi r25, 0x01             ; R25 <- 0x01
add r25, r24              ; R25 <- R25 + R24
sts 0x010c, r25           ; DATA[0x10c] <- R25
ldi r25, 0x00             ; R25 <- 0x00
out 0x22, r25             ; IO[addr] <- R25
out 0x21, r24             ; IO[addr] <- R24
lds r30, 0x0100           ; R30 <- DATA[0x100]
lds r31, 0x0101           ; R31 <- DATA[0x101]
ldd r24, Z+0              ; R24 <- DATA[Z+0]
out 0x20, r24             ; IO[addr] <- R24
sbi 0x1f, 2               ; IO[1f, 2] <- 0x01
sbi 0x1f, 1               ; IO[1f, 1] <- 0x01
lds r30, 0x0100           ; R30 <- DATA[0x100]
lds r31, 0x0101           ; R31 <- DATA[0x101]
ldd r24, Z+0              ; R24 <- DATA[Z+0]
cpse r24, r1              ; (R24 = R1): PC <- skip
rjmp L10                  ; PC <- PC + 0x9 + 1
cbi 0x1f, 3               ; IO[1f, 3] <- 0x00

L11:
pop r31                   ; R31 <- DATA[--SP]
pop r30                   ; R30 <- DATA[--SP]
pop r25                   ; R25 <- DATA[--SP]
pop r24                   ; R24 <- DATA[--SP]
pop r1                    ; R1 <- DATA[--SP]
out 0x3f, r1              ; IO[addr] <- R1
pop r1                    ; R1 <- DATA[--SP]
reti                      ; PC <- DATA[SP]

L10:
adiw r31:r30, 0x01        ; R31:R30 <- R31:R30 + 0x01
sts 0x0101, r31           ; DATA[0x101] <- R31
sts 0x0100, r30           ; DATA[0x100] <- R30
rjmp L11                  ; PC <- PC - 0xe + 1

L7:
out 0x1f, r1              ; IO[addr] <- R1
sbi 0x1f, 3               ; IO[1f, 3] <- 0x01
sei                       ; IF <- 0x01

L12:
rjmp L12                  ; PC <- PC - 0x1 + 1

L8:
cli                       ; IF <- 0x00

L13:
rjmp L13                  ; PC <- PC - 0x1 + 1
movw r1:r0, r5:r4         ; R1:R0 <- R5:R4
subi r20, 0x25            ; R20 <- R20 - 0x25
subi r20, 0x94            ; R20 <- R20 - 0x94
subi r21, 0x4f            ; R21 <- R21 - 0x4f
andi r22, 0x35            ; R22 <- R22 & 0x35
.dw 0x0074                ; 0x74
