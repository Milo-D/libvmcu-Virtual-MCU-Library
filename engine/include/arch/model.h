/* Device Model Header */

#ifndef VMCU_MODEL_H
#define VMCU_MODEL_H

// C Headers
#include <stdbool.h>
#include <inttypes.h>

// Project Headers (engine utilities)
#include "engine/include/arch/enum/device.h"
#include "engine/include/arch/enum/core.h"
#include "engine/include/arch/enum/sfr_enum.h"
#include "engine/include/arch/enum/vect.h"

typedef struct vmcu_section {

    uint32_t size;

    uint32_t start;
    uint32_t end;

} vmcu_section_t;

typedef struct vmcu_data_model {

    uint32_t size;

    vmcu_section_t registers;
    vmcu_section_t internal_io;
    vmcu_section_t external_io;
    vmcu_section_t sram;

} vmcu_data_model_t;

typedef struct vmcu_sfr_model {

    vmcu_section_t section;
    VMCU_SFR *layout;

} vmcu_sfr_model_t;

typedef struct vmcu_vtable_model {

    uint32_t n_vect;
    bool vector_32bit;

    vmcu_section_t dfl;
    VMCU_VECT *layout;

} vmcu_vtable_model_t;

typedef struct vmcu_model {

    VMCU_CORE core;
    VMCU_DEVICE device;

    vmcu_section_t flash;
    vmcu_section_t eeprom;

    vmcu_data_model_t ds;
    vmcu_sfr_model_t sfr;

    vmcu_vtable_model_t vtable;

} vmcu_model_t;

extern vmcu_model_t* vmcu_model_ctor(const VMCU_DEVICE device);
extern void vmcu_model_dtor(vmcu_model_t *this);

#endif
