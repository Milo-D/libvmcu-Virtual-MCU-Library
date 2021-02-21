/* Testfile for the Arduino Uno (ATmega328P) */

#ifndef F_CPU
#define F_CPU 16000000U
#endif

// C AVR Headers
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define pin_toggle(port, p) port ^= (1 << p)

/*
 * This AVR program toggles a LED on and off,
 * based on an overflow counter for timer0.
 *
 * Every time the overflow counter reaches
 * 0xf424, the LED will be toggled. Timer0
 * operates in normal mode (WGM0[2:0] = 0x00)
 * with no prescaler (CS0[2:0] = 0x01). The
 * ATmega328P is using a cpu clock of 16 MHz.
 *
 * This setup leads to a delay of approx.
 * 1 second between LED toggling.
 *
 * Atleast in theory. ledtest.c will test this
 * functionality. It will initialize an instance
 * of vmcu_system_t in order test the delay between
 * a toggle.
 *
 * */

volatile uint16_t counter = 0x0000;

int main(void) {

    /* pin configuration */

    DDRB |= (1 << PB5);

    /* timer0 setup */

    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TIMSK0 = 0x00;

    TCCR0B |= (1 << CS00);

    TIMSK0 |= (1 << TOIE0);
    sei();

    /* wait loop */

    while(1) {

        // wait here
    }

    return 0;
}

ISR(TIMER0_OVF_vect) {

    if(counter == 0xf424) {

        pin_toggle(PORTB, PB5);
        counter = 0x0000;

        return;
    }

    counter += 1;
}