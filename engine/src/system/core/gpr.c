/* GPR Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/core/gpr.h"
#include "system/mcudef.h"
#include "collections/array.h"

/* Forward Declaration of static GPR Functions */

static void gpr_clear_coi(const struct _gpr *this);

/* --- Public --- */

struct _gpr* gpr_ctor(void) {

    struct _gpr *gpr;
    
    if((gpr = malloc(sizeof(struct _gpr))) == NULL)
        return NULL;
    
    gpr->regfile = malloc(GPR_SIZE * sizeof(int8_t));
    memset(gpr->regfile, 0x00, GPR_SIZE * sizeof(int8_t));
    
    gpr->coi = malloc(GPR_SIZE * sizeof(MEMPROP));
    memset(gpr->coi, NONE, GPR_SIZE * sizeof(MEMPROP));
	
    return gpr;
}

void gpr_dtor(struct _gpr *this) {

    free(this->regfile);
    free(this->coi);
    free(this);
}

void gpr_write(struct _gpr *this, const int rx, const int8_t data) {

    this->coi[rx] = DEST;
    this->regfile[rx] = data;
}

int8_t gpr_read(const struct _gpr *this, const int rx) {

    this->coi[rx] = SRC;
    return this->regfile[rx];
}

void gpr_coi(const struct _gpr *this, array_t *buffer) {

    for(int i = 0; i < GPR_SIZE; i++) {
	
        const MEMPROP prop = this->coi[i];
        array_push(buffer, (void*) &prop, sizeof(MEMPROP));
    }

    gpr_clear_coi(this);
}

int8_t* gpr_dump(const struct _gpr *this) {

    return this->regfile;
}

void gpr_reboot(const struct _gpr *this) {

    memset(this->regfile, 0x00, GPR_SIZE * sizeof(int8_t));
    memset(this->coi, NONE, GPR_SIZE * sizeof(MEMPROP));
}

/* --- Private --- */

static void gpr_clear_coi(const struct _gpr *this) {

    for(int i = 0; i < GPR_SIZE; i++)
        this->coi[i] = NONE;
}

