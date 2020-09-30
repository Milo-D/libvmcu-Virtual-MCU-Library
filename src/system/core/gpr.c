/* GPR Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/core/gpr.h"
#include "system/mcudef.h"
#include "printer/memprop.h"
#include "collections/array.h"

struct _private {

    int8_t *regfile;
    int *coi;
};

/* Forward Declaration of static GPR Functions */

static void gpr_clear_coi(const struct _gpr *this);

/* --- Public --- */

struct _gpr* gpr_ctor(void) {

    struct _gpr *gpr;
    
    if((gpr = malloc(sizeof(struct _gpr))) == NULL)
        return NULL;
    
    if((gpr->p = malloc(sizeof(struct _private))) == NULL) {
     
        free(gpr);
        return NULL;
    }
    
    gpr->p->regfile = malloc(GPR_SIZE * sizeof(int8_t));
    memset(gpr->p->regfile, 0x00, GPR_SIZE * sizeof(int8_t));
    
    gpr->p->coi = malloc(GPR_SIZE * sizeof(int));
    memset(gpr->p->coi, NONE, GPR_SIZE * sizeof(int));
	
    return gpr;
}

void gpr_dtor(struct _gpr *this) {

    free(this->p->regfile);
    free(this->p->coi);
    free(this->p);
    free(this);
}

void gpr_write(struct _gpr *this, const int rx, const int8_t data) {

    this->p->coi[rx] = DEST;
    this->p->regfile[rx] = data;
}

int8_t gpr_read(const struct _gpr *this, const int rx) {

    this->p->coi[rx] = SRC;
    return this->p->regfile[rx];
}

void gpr_coi(const struct _gpr *this, array_t *buffer) {

    for(int i = 0; i < GPR_SIZE; i++) {
	
        const int prop = this->p->coi[i];
        array_push(buffer, (void*) &prop, sizeof(int));
    }

    gpr_clear_coi(this);
}

int8_t* gpr_dump(const struct _gpr *this) {

    return this->p->regfile;
}

void gpr_reboot(const struct _gpr *this) {

    memset(this->p->regfile, 0x00, GPR_SIZE * sizeof(int8_t));
    memset(this->p->coi, NONE, GPR_SIZE * sizeof(int));
}

/* --- Private --- */

static void gpr_clear_coi(const struct _gpr *this) {

    for(int i = 0; i < GPR_SIZE; i++)
        this->p->coi[i] = NONE;
}

