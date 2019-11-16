.INCLUDE "m32def.inc"

; Simple Logical Instruction Test

init:
ldi r16, 0x11       ; R16 <- 0x11
ldi r17, 21         ; R17 <- 0d21
mov r16, r17        ; R16 <- R17
mov r0, r16         ; R0 <- R16
ldi r20, 0x01       ; R20 <- 0x01
rjmp clear          ; PC <- PC + [clear] + 1

clear:
clr r16             ; R16 <- R16 xor R16
clr r17             ; R17 <- R17 xor R17
clr r0              ; R0 <- R0 xor R0
clr r20             ; R20 <- R20 xor R20
rjmp exit_loop      ; PC <- PC + [exit_loop] + 1

exit_loop:
rjmp exit_loop      ; PC <- PC + [exit_loop] + 1