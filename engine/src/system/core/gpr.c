/* GPR Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/system/core/gpr.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"

/* --- Extern --- */

vmcu_gpr_t* vmcu_gpr_ctor(void) {

    vmcu_gpr_t *gpr;
    
    if((gpr = malloc(sizeof(vmcu_gpr_t))) == NULL)
        return NULL;
    
    gpr->regfile = malloc(GPR_SIZE * sizeof(int8_t));
    memset(gpr->regfile, 0x00, GPR_SIZE * sizeof(int8_t));
	
    return gpr;
}

void vmcu_gpr_dtor(vmcu_gpr_t *this) {

    free(this->regfile);
    free(this);
}

void vmcu_gpr_write(vmcu_gpr_t *this, const int rx, const int8_t data) {

    this->regfile[rx] = data;
}

int8_t vmcu_gpr_read(const vmcu_gpr_t *this, const int rx) {

    return this->regfile[rx];
}

int8_t* vmcu_gpr_dump(const vmcu_gpr_t *this) {

    return this->regfile;
}

void vmcu_gpr_reboot(const vmcu_gpr_t *this) {

    memset(this->regfile, 0x00, GPR_SIZE * sizeof(int8_t));
}

