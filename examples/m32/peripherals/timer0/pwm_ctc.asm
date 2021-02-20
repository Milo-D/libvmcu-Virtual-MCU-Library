.INCLUDE "m32def.inc"

;
; Waveform Generation using the 8-bit Timer0 
; in CTC Mode.
;
; This small example toggles the OC Pin (PINB3)
; on compare match with OCR.
;
; System: ATmega32
; Prescaler: 1 (CS00)
; Interrupts: disabled
; CPU Clock: 1.000.000 Hz
;

cli								; clear global interrupt flag
rjmp configure_output

configure_output:
ldi r16, 0b00001000						; PINB3 for output
out DDRB, r16							; configuring DDRB
rjmp set_comp

set_comp:
ldi r16, 0x21							; 0x21 as compare value
out OCR0, r16							; setting OCR0 to 0x21

enable_timer:
ldi r16, (0x01 << CS00) | (0x01 << COM00) | (0x01 << WGM01)	; p = 1, CTC, toggle on match
out TCCR0, r16							; enabling timer0
rjmp loop

loop:
rjmp loop							; endless loop
