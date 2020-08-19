/* AVR ALU Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "system/alu.h"
#include "system/flash.h"
#include "system/gpr.h"
#include "system/sreg.h"
#include "system/system.h"
#include "instructions/instructions.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    flash_t *flash;
    gpr_t *gpr;
    sreg_t *sreg;
};

/* --- Public --- */

struct _alu* alu_ctor(table_t *table) {

    struct _alu *alu;

    if((alu = malloc(sizeof(struct _alu))) == NULL)
        return NULL;

    if((alu->p = malloc(sizeof(struct _private))) == NULL) {

        free(alu);
        return NULL;
    }

    if((alu->p->flash = flash_ctor(table)) == NULL) {

        alu_dtor(alu);
        return NULL;
    }

    if((alu->p->gpr = gpr_ctor()) == NULL) {

        alu_dtor(alu);
        return NULL;
    }

    if((alu->p->sreg = sreg_ctor()) == NULL) {

        alu_dtor(alu);
        return NULL;
    }

    return alu;
}

void alu_dtor(struct _alu *this) {

    if(this->p->flash != NULL)
        flash_dtor(this->p->flash);

    if(this->p->gpr != NULL)
        gpr_dtor(this->p->gpr);

    if(this->p->sreg != NULL)
        sreg_dtor(this->p->sreg);

    free(this->p);
    free(this);
}

int alu_fetch(struct _alu *this, system_t *sys) {

    tuple_t *buffer = tuple_ctor(2, INT16, INT);
    flash_fetch(this->p->flash, buffer);

    const int opcode = *((int*) tuple_get(buffer, 0));
    const int key = *((int*) tuple_get(buffer, 1));

    if(flash_table_is_sync(this->p->flash) == true) {

        if(key < 0) {

            tuple_dtor(buffer);
            return -1;
        }

        (*instructions[key])(sys, opcode);
        flash_pc_next(this->p->flash);
    }

    if(flash_table_step(this->p->flash) < 0)
        sys_kill(sys);

    tuple_dtor(buffer);
    return 0;
}

int alu_get_pc(const struct _alu *this) {

    return flash_pc(this->p->flash);
}

void alu_set_pc(struct _alu *this, const int addr) {

    flash_pc_set(this->p->flash, addr);
}

void alu_write_gpr(struct _alu *this, const int rx, const int8_t data) {
    
    gpr_write(this->p->gpr, rx, data);
}

int8_t alu_read_gpr(const struct _alu *this, const int rx) {

    return gpr_read(this->p->gpr, rx);
}

void alu_gpr_coi(const struct _alu *this, array_t *buffer) {

    gpr_coi(this->p->gpr, buffer);
}

void alu_dump_gpr(const struct _alu *this, array_t *buffer) {

    gpr_dump(this->p->gpr, buffer);
}

void alu_write_sreg(struct _alu *this, const int flag, const bool bit) {

    sreg_write(this->p->sreg, flag, bit);
}

bool alu_read_sreg(const struct _alu *this, const int flag) {

    return sreg_read(this->p->sreg, flag);
}

void alu_sreg_coi(const struct _alu *this, array_t *buffer) {

    sreg_coi(this->p->sreg, buffer);
}

uint8_t alu_dump_sreg(const struct _alu *this) {

    return sreg_dump(this->p->sreg);
}


