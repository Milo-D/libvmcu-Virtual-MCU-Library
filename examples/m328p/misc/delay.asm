;
; This program is calling the _delay_ms()
; subroutine defined in util/delay.h
;
; Original C Code:
;
; #ifndef F_CPU
; #define F_CPU 16000000U
; #endif
;
; #include <avr/io.h>
; #include <avr/interrupt.h>
; #include <util/delay.h>
;
; int main(void) {
;
;     _delay_ms(1000);
;     return 0;
; }
;

.INCLUDE "m328Pdef.inc"

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
jmp L3                    ; PC <- 0x4c

L1:
jmp L4                    ; PC <- 0x0

L2:
ldi r18, 0xff             ; R18 <- 0xff
ldi r24, 0xd3             ; R24 <- 0xd3
ldi r25, 0x30             ; R25 <- 0x30

L5:
subi r18, 0x01            ; R18 <- R18 - 0x01
sbci r24, 0x00            ; R24 <- R24 - 0x00 - CF
sbci r25, 0x00            ; R25 <- R25 - 0x00 - CF
brne L5                   ; (Z = 0): PC <- PC - 0x4 + 1
rjmp L6                   ; PC <- PC + 0x0 + 1

L6:
nop                       ; no operation
ldi r24, 0x00             ; R24 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00
ret                       ; PC <- DATA[SP]

L3:
cli                       ; IF <- 0x00

L7:
rjmp L7                   ; PC <- PC - 0x1 + 1
