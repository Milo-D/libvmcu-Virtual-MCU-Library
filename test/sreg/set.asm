.INCLUDE "m32def.inc"

start:              ; entry
ses                 ; [S-Flag] <- 0x01
set                 ; [T-Flag] <- 0x01
rjmp end            ; PC <- [PC + end + 1] 

end:                ; end
nop                 ; no instruction