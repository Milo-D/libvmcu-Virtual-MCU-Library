.INCLUDE "m32def.inc"

; Approximating Squareroot of a number stored
; in R16 via binary search. 

; Due to the limiting 8-bit register size and other limitations
; only 0x04, 0x10, 0x24 will work for R16.

.def left = r21          ; left border
.def right = r22         ; right border

init:
ldi r16, 0x10            ; number, we want the squareroot from
ldi left, 0x00           ; init left border with 0x00
mov right, r16           ; init right border with R16
rjmp stack_init          ; jump to stack_init

stack_init:              
ldi r18, LOW(RAMEND)     ; load R18 with lower 8-bit of RAMEND address
out SPL, r18             ; init SPL with R18
ldi r18, HIGH(RAMEND)    ; load R18 with higher 8-bit of RAMEND address
out SPH, r18             ; init SPH with R18
rjmp approx              ; jump to actual algorithm

approx:
mov r19, right           ; move right border into R19
rcall get_mid            ; call get_mid subroutine to calculate mid of L/R Border
rcall pow                ; call pow to calculate pow(x, 2)
cpi r19, 0x10            ; cp with Rd aren't available yet, replace const with val of R16
breq exit                ; done. jumping to exit
brge set_lower           ; branch to set_lower
rjmp set_higher          ; branch to set_higher

get_mid:
sub r19, left            ; subtract left border from R19
lsr r19                  ; logical right shift at R19 to divide by two
add r19, left            ; add left border to R19
mov r20, r19             ; move R19 (now mid point) into R20
ret                      ; return from subroutine

pow:
muls r19, r19            ; multiply signed R19 with R19
mov r19, r0              ; get lower 8-bit from result and store it into R19
ret                      ; return from subroutine

set_lower:
mov right, r20           ; setting right border to R20
rjmp approx              ; jump back to approx

set_higher:
mov left, r20            ; setting left border to R20
rjmp approx              ; jump back to approx

exit:
push r20                 ; pushing squareroot of R16 onto the Stack
nop                      ; no instruction