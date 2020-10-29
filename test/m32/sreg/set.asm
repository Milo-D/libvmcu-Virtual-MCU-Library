.INCLUDE "m32def.inc"

start:              ; entry
ses                 ; [S-Flag] <- 0x01
set                 ; [T-Flag] <- 0x01
sev                 ; [V-Flag] <- 0x01
sez                 ; [Z-Flag] <- 0x01
seh                 ; [H-Flag] <- 0x01
sec                 ; [C-Flag] <- 0x01
sei                 ; [I-Flag] <- 0x01
sen                 ; [N-Flag] <- 0x01
rjmp clr            ; PC <- [PC + clr + 1] 

clr:
bclr 0x04           ; [S-Flag] <- 0x00
bclr 0x06           ; [T-Flag] <- 0x00
bclr 0x03           ; [V-Flag] <- 0x00
bclr 0x01           ; [Z-Flag] <- 0x00
bclr 0x05           ; [H-Flag] <- 0x00
bclr 0x00           ; [C-Flag] <- 0x00
bclr 0x07           ; [I-Flag] <- 0x00
bclr 0x02           ; [N-Flag] <- 0x00
rjmp end            ; PC <- [PC + end + 1]
 
end:                ; end
nop                 ; no instruction