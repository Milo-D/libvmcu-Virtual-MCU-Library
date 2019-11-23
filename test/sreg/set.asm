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
rjmp end            ; PC <- [PC + end + 1] 

end:                ; end
nop                 ; no instruction