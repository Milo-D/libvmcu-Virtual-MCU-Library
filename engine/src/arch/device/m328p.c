/* ATmega328P Device Loader Implementation */

// Project Headers (engine utilites)
#include "engine/include/arch/device/m328p.h"
#include "engine/include/arch/enum/device.h"
#include "engine/include/arch/enum/core.h"
#include "engine/include/arch/enum/sfr_enum.h"
#include "engine/include/arch/model.h"

/* Forward Declaration of ATmega328P specific fields */

VMCU_SFR m328p_sfr_layout[167];
VMCU_VECT m328p_vtable_layout[26];

/* --- Extern --- */

void load_device_m328p(vmcu_model_t *model) {

    /* <----------------- GENERAL -----------------> */

    model->core                    = VMCU_CORE_AVRep;
    model->device                  = VMCU_DEVICE_M328P;

    /* <------------------ FLASH ------------------> */

    model->flash.size              = 0x4000;
    model->flash.start             = 0x0000;
    model->flash.end               = 0x3fff;

    /* <----------------- EEPROM ------------------> */

    model->eeprom.size             = 0x0400;
    model->eeprom.start            = 0x0000;
    model->eeprom.end              = 0x03ff;

    /* <------------------- DS --------------------> */

    model->ds.size                 = 0x0900;

    model->ds.registers.size       = 0x20;
    model->ds.registers.start      = 0x0000;
    model->ds.registers.end        = 0x001f;

    model->ds.internal_io.size     = 0x40;
    model->ds.internal_io.start    = 0x0020;
    model->ds.internal_io.end      = 0x5f;

    model->ds.external_io.size     = 0xa0;
    model->ds.external_io.start    = 0x0060;
    model->ds.external_io.end      = 0x00ff;

    model->ds.sram.size            = 0x0800;
    model->ds.sram.start           = 0x0100;
    model->ds.sram.end             = 0x08ff;

    /* <------------------ SFR --------------------> */

    model->sfr.section.size        = 0xa7;
    model->sfr.section.start       = 0x0020;
    model->sfr.section.end         = 0x00c6;

    model->sfr.layout              = m328p_sfr_layout;

    /* <-------------- Vectortable ----------------> */

    model->vtable.n_vect           = 26;

    model->vtable.dfl.size         = 0x33;
    model->vtable.dfl.start        = 0x0000;
    model->vtable.dfl.end          = 0x0032;

    model->vtable.layout           = m328p_vtable_layout;
    model->vtable.vector_32bit     = true;
}

/* --- ATmega328P specific fields --- */

VMCU_SFR m328p_sfr_layout[167] = {

    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_PINB,
    VMCU_SFR_DDRB,
    VMCU_SFR_PORTB,
    VMCU_SFR_PINC,
    VMCU_SFR_DDRC,
    VMCU_SFR_PORTC,
    VMCU_SFR_PIND,
    VMCU_SFR_DDRD,
    VMCU_SFR_PORTD,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_TIFR0,
    VMCU_SFR_TIFR1,
    VMCU_SFR_TIFR2,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_PCIFR,
    VMCU_SFR_EIFR,
    VMCU_SFR_EIMSK,
    VMCU_SFR_GPIOR0,
    VMCU_SFR_EECR,
    VMCU_SFR_EEDR,
    VMCU_SFR_EEARL,
    VMCU_SFR_EEARH,
    VMCU_SFR_GTCCR,
    VMCU_SFR_TCCR0A,
    VMCU_SFR_TCCR0B,
    VMCU_SFR_TCNT0,
    VMCU_SFR_OCR0A,
    VMCU_SFR_OCR0B,
    VMCU_SFR_RESERVED,
    VMCU_SFR_GPIOR1,
    VMCU_SFR_GPIOR2,
    VMCU_SFR_SPCR0,
    VMCU_SFR_SPSR0,
    VMCU_SFR_SPDR0,
    VMCU_SFR_RESERVED,
    VMCU_SFR_ACSR,
    VMCU_SFR_DWDR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_SMCR,
    VMCU_SFR_MCUSR,
    VMCU_SFR_MCUCR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_SPMCSR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_SPL,
    VMCU_SFR_SPH,
    VMCU_SFR_SREG,
    VMCU_SFR_WDTCSR,
    VMCU_SFR_CLKPR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_PRR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_OSCCAL,
    VMCU_SFR_RESERVED,
    VMCU_SFR_PCICR,
    VMCU_SFR_EICRA,
    VMCU_SFR_RESERVED,
    VMCU_SFR_PCMSK0,
    VMCU_SFR_PCMSK1,
    VMCU_SFR_PCMSK2,
    VMCU_SFR_TIMSK0,
    VMCU_SFR_TIMSK1,
    VMCU_SFR_TIMSK2,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_ADCL,
    VMCU_SFR_ADCH,
    VMCU_SFR_ADCSRA,
    VMCU_SFR_ADCSRB,
    VMCU_SFR_ADMUX,
    VMCU_SFR_RESERVED,
    VMCU_SFR_DIDR0,
    VMCU_SFR_DIDR1,
    VMCU_SFR_TCCR1A,
    VMCU_SFR_TCCR1B,
    VMCU_SFR_TCCR1C,
    VMCU_SFR_RESERVED,
    VMCU_SFR_TCNT1L,
    VMCU_SFR_TCNT1H,
    VMCU_SFR_ICR1L,
    VMCU_SFR_ICR1H,
    VMCU_SFR_OCR1AL,
    VMCU_SFR_OCR1AH,
    VMCU_SFR_OCR1BL,
    VMCU_SFR_OCR1BH,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_TCCR2A,
    VMCU_SFR_TCCR2B,
    VMCU_SFR_TCNT2,
    VMCU_SFR_OCR2A,
    VMCU_SFR_OCR2B,
    VMCU_SFR_RESERVED,
    VMCU_SFR_ASSR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_TWBR,
    VMCU_SFR_TWSR,
    VMCU_SFR_TWAR,
    VMCU_SFR_TWDR,
    VMCU_SFR_TWCR,
    VMCU_SFR_TWAMR,
    VMCU_SFR_RESERVED,
    VMCU_SFR_RESERVED,
    VMCU_SFR_UCSR0A,
    VMCU_SFR_UCSR0B,
    VMCU_SFR_UCSR0C,
    VMCU_SFR_RESERVED,
    VMCU_SFR_UBRR0L,
    VMCU_SFR_UBRR0H,
    VMCU_SFR_UDR0
};

VMCU_VECT m328p_vtable_layout[26] = {

    VMCU_VECT_RESET,
    VMCU_VECT_INT0,
    VMCU_VECT_INT1,
    VMCU_VECT_PCINT0,
    VMCU_VECT_PCINT1,
    VMCU_VECT_PCINT2,
    VMCU_VECT_WDT,
    VMCU_VECT_TIMER2_COMPA,
    VMCU_VECT_TIMER2_COMPB,
    VMCU_VECT_TIMER2_OVF,
    VMCU_VECT_TIMER1_CAPT,
    VMCU_VECT_TIMER1_COMPA,
    VMCU_VECT_TIMER1_COMPB,
    VMCU_VECT_TIMER1_OVF,
    VMCU_VECT_TIMER0_COMPA,
    VMCU_VECT_TIMER0_COMPB,
    VMCU_VECT_TIMER0_OVF,
    VMCU_VECT_SPI_STC,
    VMCU_VECT_USART_RX,
    VMCU_VECT_USART_UDRE,
    VMCU_VECT_USART_TX,
    VMCU_VECT_ADC,
    VMCU_VECT_EE_READY,
    VMCU_VECT_ANALOG_COMP,
    VMCU_VECT_TWI,
    VMCU_VECT_SPM_READY
};