/* AVR ALU Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "system/core/alu.h"
#include "system/core/flash.h"
#include "system/core/gpr.h"
#include "system/core/sreg.h"
#include "system/system.h"
#include "disassembler/plain.h"
#include "instructions/instructions.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    flash_t *flash;
    gpr_t *gpr;
    sreg_t *sreg;
};

/* --- Public --- */

struct _alu* alu_ctor(const char *file) {

    struct _alu *alu;

    if((alu = malloc(sizeof(struct _alu))) == NULL)
        return NULL;

    if((alu->p = malloc(sizeof(struct _private))) == NULL) {

        free(alu);
        return NULL;
    }

    if((alu->p->flash = flash_ctor(file)) == NULL) {

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

    plain_t *p = flash_fetch(this->p->flash);

    if(p == NULL) {

        flash_move_pc(this->p->flash, 1); 
        return -1;
    }

    (*instructions[p->key])(sys, p->opcode);
    return 0;
}

void alu_reboot(const struct _alu *this) {

    flash_reboot(this->p->flash);
    sreg_reboot(this->p->sreg);
    gpr_reboot(this->p->gpr);
}

plain_t* alu_read_instr(const struct _alu *this, const int addr) {

    return flash_read_instr(this->p->flash, addr);
}

uint16_t alu_read_flash(const struct _alu *this, const int addr) {

    return flash_read(this->p->flash, addr);
}

void alu_move_pc(const struct _alu *this, const int inc) {

    flash_move_pc(this->p->flash, inc);
}

void alu_set_pc(struct _alu *this, const int addr) {

    flash_set_pc(this->p->flash, addr);
}

int alu_get_pc(const struct _alu *this) {

    return flash_get_pc(this->p->flash);
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

int alu_add_breakp(const struct _alu *this, const char *point) {

    return flash_add_breakp(this->p->flash, point);
}

int alu_del_breakp(const struct _alu *this, const char *point) {

    return flash_del_breakp(this->p->flash, point);
}

bool alu_on_breakp(const struct _alu *this) {

    return flash_on_breakp(this->p->flash);
}

int alu_table_size(const struct _alu *this) {

    return flash_table_size(this->p->flash);
}

entry_t* alu_dump_table(const struct _alu *this) {

    return flash_dump_table(this->p->flash);
}


