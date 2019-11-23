.INCLUDE "m32def.inc"

init:                      ; init
ldi r16, 0x11              ; R16 <- 0x11
ldi r17, 21                ; R17 <- 0d21
rjmp exec                  ; PC <- [PC + exec + 1]

exec:                      ; execution
movw r19:r18, r17:r16      ; R19:R18 <- R17:R16
rjmp clean                 ; PC <- [PC + clean + 1]

clean:                     ; cleanup
clr r16                    ; R16 xor R16
clr r17                    ; R!7 xor R17
clr r18                    ; R18 xor R18
clr r19                    ; R19 xor R19
rjmp end                   ; PC <- [PC + end + 1]

end:                       ; end loop
rjmp end