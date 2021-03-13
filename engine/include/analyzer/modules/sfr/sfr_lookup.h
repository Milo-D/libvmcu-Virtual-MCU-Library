/* SFR Lookup Table Header */

#ifndef VMCU_SFR_LOOKUP_H
#define VMCU_SFR_LOOKUP_H

// Project Headers (engine)
#include "engine/include/analyzer/report/sfr.h"

#define SFR_LOOKUP_SIZE 167

/*
 * This file contains a lookup table for
 * special function registers. It maps I/O addresses
 * to VMCU_SFREGISTER.
 *
 * todo: move VMCU_SFREGISTER and vmcu_sfr_lookup[] to
 *       engine/include/arch/ and engine/src/arch/ in order
 *       to avoid architecture specific definitions in the
 *       analyzer pipeline.
 * */

extern const VMCU_SFREGISTER vmcu_sfr_lookup[SFR_LOOKUP_SIZE];

#endif
