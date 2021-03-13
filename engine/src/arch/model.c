/* Device Model Implementation */

// C Headers
#include <stdlib.h>
#include <stddef.h>

// Project Headers (engine utilites)
#include "engine/include/arch/model.h"

// Project Headers (engine utilities) (devices)
#include "engine/include/arch/device/m328p.h"

vmcu_model_t* vmcu_model_ctor(const VMCU_DEVICE device) {

    vmcu_model_t *model;

    if((model = malloc(sizeof(vmcu_model_t))) == NULL)
        return NULL;

    switch(device) {

        case VMCU_M328P: load_device_m328p(model); break;

        default: /* device does not exist */ return NULL;
    }

    return model;
}

void vmcu_model_dtor(vmcu_model_t *this) {

    free(this);
}
