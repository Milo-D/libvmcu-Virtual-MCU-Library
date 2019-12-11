.INCLUDE "m32def.inc"

; Iterative Fibonacci Sequence

; Calculating the Fibonacci Sequence
; and pushing the last two calculated values
; onto the Stack.

; This program does not terminate due to the Overflow in the 8-bit registers.

init:
ldi r16, 0x00              ; first Fibonacci number (k = 0)
ldi r17, 0x01              ; second Fibonacci number (k = 1)
rjmp stack_init            ; jump to stack_init

stack_init:                
ldi r18, LOW(RAMEND)       ; load lower 8-bit of RAMEND address into R18
out SPL, r18               ; init SPL with R18
ldi r18, HIGH(RAMEND)      ; load higher 8-bit of RAMEND address into R18
out SPH, r18               ; init SPH with R18
rjmp loop                  ; jump to main loop

loop:
push r16                   ; saving (k - 2) fibonacci number 
push r17                   ; saving (k - 1) fibonacci number
rjmp fib                   ; jump to fibonacci calculation of k

fib:
pop r17                    ; pop (k - 1) from the stack and into R17
pop r16                    ; pop (k - 2) from the stack and into R16
add r16, r17               ; overwriting R16 with the sum of (k - 1) and (k - 2)
push r17                   ; pushing R17 onto the Stack
push r16                   ; pushing R16 onto the Stack
rjmp fib                   ; repeat