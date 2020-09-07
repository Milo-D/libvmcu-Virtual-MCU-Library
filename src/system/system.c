/* System Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/system.h"
#include "system/mcudef.h"
#include "system/alu.h"
#include "system/gpr.h"
#include "system/data.h"
#include "system/flash.h"
#include "system/eeprom.h"
#include "table/table.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    alu_t *alu;
    data_t *data;
    eeprom_t *eeprom;
    
    int steps;
    bool terminated;
};

struct _system* sys_ctor(const char *file) {

    struct _system *sys;
    
    if((sys = malloc(sizeof(struct _system))) == NULL)
        return NULL;
        
    if((sys->p = malloc(sizeof(struct _private))) == NULL) {
    
        free(sys);
        return NULL;
    }
    
    if((sys->p->alu = alu_ctor(file)) == NULL) {
    
        sys_dtor(sys);
        return NULL;
    }
    
    if((sys->p->data = data_ctor()) == NULL) {
    
        sys_dtor(sys);
        return NULL;
    }
    
    if((sys->p->eeprom = eeprom_ctor()) == NULL) {
    
        sys_dtor(sys);
        return NULL;
    }

    sys->cycles = 0;
    sys->clock = CLOCK;

    sys->p->steps = 0;
    sys->p->terminated = false;

    return sys;
}

void sys_dtor(struct _system *this) {

    if(this->p->alu != NULL)
        alu_dtor(this->p->alu);
        
    if(this->p->data != NULL)
        data_dtor(this->p->data);
        
    if(this->p->eeprom != NULL)
        eeprom_dtor(this->p->eeprom);

    free(this->p);
    free(this);
}

int sys_step(struct _system *this) {

    if(this->p->terminated == true)
        return 0;

    this->p->steps += 1;
    return alu_fetch(this->p->alu, this);
}

void sys_backstep(struct _system *this) {

    if(this->p->terminated == true)
        return;

    int counter = this->p->steps - 1;
    sys_reboot(this);

    while(counter-- > 0)
        sys_step(this);
}

void sys_reboot(struct _system *this) {

    this->cycles = 0;
    this->clock = CLOCK;

    this->p->steps = 0;
    this->p->terminated = false;

    alu_reboot(this->p->alu);
    data_reboot(this->p->data);
    eeprom_reboot(this->p->eeprom);
}

void sys_kill(const struct _system *this) {

    this->p->terminated = true;
}

bool sys_is_term(const struct _system *this) {

    return this->p->terminated;	
}

void sys_write_gpr(struct _system *this, const int rx, const int8_t data) {

    alu_write_gpr(this->p->alu, rx, data);
}

int8_t sys_read_gpr(const struct _system *this, const int rx) {

    return alu_read_gpr(this->p->alu, rx);
}

void sys_gpr_coi(const struct _system *this, array_t *buffer) {

    alu_gpr_coi(this->p->alu, buffer);
}

void sys_dump_gpr(const struct _system *this, array_t *buffer) {

    alu_dump_gpr(this->p->alu, buffer);
}

void sys_write_sreg(struct _system *this, const int flag, const bool bit) {

    alu_write_sreg(this->p->alu, flag, bit);
}

bool sys_read_sreg(const struct _system *this, const int flag) {

    return alu_read_sreg(this->p->alu, flag);
}

void sys_sreg_coi(const struct _system *this, array_t *buffer) {

    alu_sreg_coi(this->p->alu, buffer);
}

uint8_t sys_dump_sreg(const struct _system *this) {

    return alu_dump_sreg(this->p->alu);
}

int sys_get_pc(const struct _system *this) {

    return alu_get_pc(this->p->alu);
}

void sys_set_pc(struct _system *this, const int addr) {

    alu_set_pc(this->p->alu, addr);
}

void sys_push_stack(struct _system *this, const int8_t value) {

    data_push(this->p->data, value);
}

int8_t sys_pop_stack(const struct _system *this) {

    return data_pop(this->p->data);
}

void sys_write_data(struct _system *this, const uint16_t addr, const int8_t value) {

    if(addr < GPR_SIZE) {

        alu_write_gpr(this->p->alu, addr, value);
        return;
    }

    data_write(this->p->data, addr, value);
}

int8_t sys_read_data(const struct _system *this, const uint16_t addr) {

    if(addr < GPR_SIZE)
        return alu_read_gpr(this->p->alu, addr);

    return data_read(this->p->data, addr);
}

void sys_data_coi(const struct _system *this, tuple_t *buffer) {

    data_coi(this->p->data, buffer);
}

void sys_dump_data(const struct _system *this, array_t *buffer) {

    data_dump(this->p->data, buffer);
}

void sys_write_eeprom(struct _system *this, const uint16_t addr, const int8_t value) {

    eeprom_write(this->p->eeprom, addr, value);
}

int8_t sys_read_eeprom(const struct _system *this, const uint16_t addr) {

    return eeprom_read(this->p->eeprom, addr);
}

void sys_eeprom_coi(const struct _system *this, tuple_t *buffer) {

    eeprom_coi(this->p->eeprom, buffer);
}

void sys_dump_eeprom(const struct _system *this, array_t *buffer) {

    eeprom_dump(this->p->eeprom, buffer);
}

int sys_add_breakp(const struct _system *this, const char *point) {

    return alu_add_breakp(this->p->alu, point);
}

int sys_del_breakp(const struct _system *this, const char *point) {

    return alu_del_breakp(this->p->alu, point);
}

void sys_set_tip(const struct _system *this, const int line) {

    alu_set_tip(this->p->alu, line);
}

int sys_get_tip(const struct _system *this) {

    return alu_get_tip(this->p->alu);
}

bool sys_on_breakp(const struct _system *this) {

    return alu_on_breakp(this->p->alu);
}

int sys_table_size(const struct _system *this) {

    return alu_table_size(this->p->alu);
}

entry_t* sys_dump_table(const struct _system *this) {

    return alu_dump_table(this->p->alu);
}
