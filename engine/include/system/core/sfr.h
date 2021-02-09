/* SFR Header */

#ifndef SFR_H
#define SFR_H

// C Headers
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/system/mcudef.h"

/*
 * This file belongs to the IO Module but is
 * quite long so I decided to create a seperate
 * file for these functions.
 * 
 * This file will be automatically included by
 * including the IO Module 'io.h'.
 * 
 **/

typedef struct _io io_t;

extern void (*sfr_set[SFRL_SIZE]) (io_t *this, const int bit);
extern void (*sfr_clear[SFRL_SIZE]) (io_t *this, const int bit);

extern void (*sfr_write[SFR_SIZE]) (io_t *this, const int8_t value);
extern int8_t (*sfr_read[SFR_SIZE]) (io_t *this);

#endif
