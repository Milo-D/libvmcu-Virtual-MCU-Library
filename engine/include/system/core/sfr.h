/* SFR Header */

#ifndef VMCU_SFR_H
#define VMCU_SFR_H

// C Headers
#include <inttypes.h>

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"

/*
 * This file belongs to the IO Module but is
 * quite long so I decided to create a seperate
 * file for these functions.
 * 
 * This file will be automatically included by
 * including the IO Module 'io.h'.
 * 
 **/

typedef struct vmcu_io vmcu_io_t;

extern void (*vmcu_sfr_set[SFRL_SIZE]) (vmcu_io_t *this, const int bit);
extern void (*vmcu_sfr_clear[SFRL_SIZE]) (vmcu_io_t *this, const int bit);

extern void (*vmcu_sfr_write[SFR_SIZE]) (vmcu_io_t *this, const int8_t value);
extern int8_t (*vmcu_sfr_read[SFR_SIZE]) (vmcu_io_t *this);

#endif
