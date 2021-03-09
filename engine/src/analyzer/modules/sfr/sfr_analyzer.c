/* SFR-Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/sfr/sfr_analyzer.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"

#define IO_OFFSET GPR_SIZE

/* Forward Declaration of static Members */

static const VMCU_SFREGISTER sfregister_lookup[SFR_SIZE];

/* Forward Declaration of static Functions */

static vmcu_operand_t* instr_is_sfr_related(vmcu_plain_t *p);
static inline bool operand_is_sfr_related(vmcu_operand_t *op);

static VMCU_SFREGISTER get_id_by_instr(vmcu_plain_t *p);

static vmcu_sfr_t* get_sfrs(const int32_t *sfr_map, int32_t size);
static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_sfr_t *sfr);

/* --- Extern --- */

int vmcu_analyze_sfr(vmcu_report_t *report) {

    int32_t sfr_map[SFR_SIZE];

    size_t bytes = SFR_SIZE * sizeof(int32_t);
    memset(sfr_map, 0, bytes);

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_plain_t *p = &report->disassembly[i];
        VMCU_SFREGISTER id = get_id_by_instr(p);

        if(id == VMCU_SFREGISTER_NONE)
            continue;

        if(sfr_map[id] == 0)
            report->n_sfr += 1;

        sfr_map[id] += 1;
    }

    if(report->n_sfr <= 0)
        return -1;

    report->sfr = get_sfrs(sfr_map, report->n_sfr);

    for(int32_t i = 0; i < report->n_sfr; i++) {

        vmcu_sfr_t *sfr = &report->sfr[i];

        sfr->n_xref = sfr_map[sfr->id];
        sfr->xref   = get_xrefs(report, sfr);
    }

    return 0;
}

/* --- Static --- */

static vmcu_operand_t* instr_is_sfr_related(vmcu_plain_t *p) {

    switch(p->key) {

        case VMCU_SBIS:  return &p->dest;
        case VMCU_SBIC:  return &p->dest;
        case VMCU_CBI:   return &p->dest;
        case VMCU_SBI:   return &p->dest;
        case VMCU_OUT:   return &p->dest;
        case VMCU_IN:    return &p->src;

        case VMCU_LDS:   return &p->src;
        case VMCU_LDS32: return &p->src;

        case VMCU_STS:   return &p->dest;
        case VMCU_STS32: return &p->dest;

        default:         return NULL;
    }
}

static inline bool operand_is_sfr_related(vmcu_operand_t *op) {

    const int offs = ((op->type == VMCU_IODIRECT) * IO_OFFSET);
    return ((op->value + offs) >= SFR_START && (op->value + offs) <= SFR_END);
}

static VMCU_SFREGISTER get_id_by_instr(vmcu_plain_t *p) {

    vmcu_operand_t *op;

    if((op = instr_is_sfr_related(p)) == NULL)
        return VMCU_SFREGISTER_NONE;

    if(operand_is_sfr_related(op) == false)
        return VMCU_SFREGISTER_NONE;

    int offs = ((op->type != VMCU_IODIRECT) * IO_OFFSET);
    return sfregister_lookup[op->value - offs];
}

static vmcu_sfr_t* get_sfrs(const int32_t *sfr_map, int32_t size) {

    vmcu_sfr_t *sfrs = malloc(size * sizeof(vmcu_sfr_t));

    for(int32_t i = 0, j = 0; i < SFR_SIZE; i++) {

        if(sfr_map[i] == 0)
            continue;

        sfrs[j++] = (vmcu_sfr_t) {

            .id     = i,
            .n_xref = 0,
            .xref   = NULL
        };
    }

    return sfrs;
}

static vmcu_xref_t* get_xrefs(vmcu_report_t *report, vmcu_sfr_t *sfr) {

    vmcu_xref_t *xrefs = malloc(sfr->n_xref * sizeof(vmcu_xref_t));

    for(int32_t i = 0, j = 0; i < report->progsize; i++) {

        vmcu_plain_t *p = &report->disassembly[i];

        if(get_id_by_instr(p) != sfr->id)
            continue;

        xrefs[j++].p = p;
    }

    return xrefs;
}

static const VMCU_SFREGISTER sfregister_lookup[SFR_SIZE] = {

    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_PINB,
    VMCU_DDRB,
    VMCU_PORTB,
    VMCU_PINC,
    VMCU_DDRC,
    VMCU_PORTC,
    VMCU_PIND,
    VMCU_DDRD,
    VMCU_PORTD,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_TIFR0,
    VMCU_TIFR1,
    VMCU_TIFR2,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_PCIFR,
    VMCU_EIFR,
    VMCU_EIMSK,
    VMCU_GPIOR0,
    VMCU_EECR,
    VMCU_EEDR,
    VMCU_EEARL,
    VMCU_EEARH,
    VMCU_GTCCR,
    VMCU_TCCR0A,
    VMCU_TCCR0B,
    VMCU_TCNT0,
    VMCU_OCR0A,
    VMCU_OCR0B,
    VMCU_RESERVED,
    VMCU_GPIOR1,
    VMCU_GPIOR2,
    VMCU_SPCR0,
    VMCU_SPSR0,
    VMCU_SPDR0,
    VMCU_RESERVED,
    VMCU_ACSR,
    VMCU_DWDR,
    VMCU_RESERVED,
    VMCU_SMCR,
    VMCU_MCUSR,
    VMCU_MCUCR,
    VMCU_RESERVED,
    VMCU_SPMCSR,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_SPL,
    VMCU_SPH,
    VMCU_SREG,
    VMCU_WDTCSR,
    VMCU_CLKPR,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_PRR,
    VMCU_RESERVED,
    VMCU_OSCCAL,
    VMCU_RESERVED,
    VMCU_PCICR,
    VMCU_EICRA,
    VMCU_RESERVED,
    VMCU_PCMSK0,
    VMCU_PCMSK1,
    VMCU_PCMSK2,
    VMCU_TIMSK0,
    VMCU_TIMSK1,
    VMCU_TIMSK2,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_ADCL,
    VMCU_ADCH,
    VMCU_ADCSRA,
    VMCU_ADCSRB,
    VMCU_ADMUX,
    VMCU_RESERVED,
    VMCU_DIDR0,
    VMCU_DIDR1,
    VMCU_TCCR1A,
    VMCU_TCCR1B,
    VMCU_TCCR1C,
    VMCU_RESERVED,
    VMCU_TCNT1L,
    VMCU_TCNT1H,
    VMCU_ICR1L,
    VMCU_ICR1H,
    VMCU_OCR1AL,
    VMCU_OCR1AH,
    VMCU_OCR1BL,
    VMCU_OCR1BH,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_TCCR2A,
    VMCU_TCCR2B,
    VMCU_TCNT2,
    VMCU_OCR2A,
    VMCU_OCR2B,
    VMCU_RESERVED,
    VMCU_ASSR,
    VMCU_RESERVED,
    VMCU_TWBR,
    VMCU_TWSR,
    VMCU_TWAR,
    VMCU_TWDR,
    VMCU_TWCR,
    VMCU_TWAMR,
    VMCU_RESERVED,
    VMCU_RESERVED,
    VMCU_UCSR0A,
    VMCU_UCSR0B,
    VMCU_UCSR0C,
    VMCU_RESERVED,
    VMCU_UBRR0L,
    VMCU_UBRR0H,
    VMCU_UDR0
};
