/* System Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/system/system.h"
#include "engine/include/system/mcudef.h"
#include "engine/include/system/core/gpr.h"
#include "engine/include/system/core/sreg.h"
#include "engine/include/system/core/data.h"
#include "engine/include/system/core/flash.h"
#include "engine/include/analyzer/report/report.h"
#include "engine/include/instructions/instructions.h"

// Project Headers (shared)
#include "shared/include/collections/array.h"
#include "shared/include/collections/tuple.h"

/* Forward Declaration of static Functions */

static void sys_update_io(struct _system *this, const uint64_t dc);
static void sys_exec_isr(struct _system *this, const int isr);

/* --- Extern --- */

struct _system* sys_ctor(const report_t *report) {

    struct _system *sys;
    
    if((sys = malloc(sizeof(struct _system))) == NULL)
        return NULL;

    if((sys->flash = flash_ctor(report)) == NULL) {

        sys_dtor(sys);
        return NULL;
    }

    if((sys->gpr = gpr_ctor()) == NULL) {

        sys_dtor(sys);
        return NULL;
    }

    if((sys->sreg = sreg_ctor()) == NULL) {

        sys_dtor(sys);
        return NULL;
    }
    
    if((sys->data = data_ctor()) == NULL) {
    
        sys_dtor(sys);
        return NULL;
    }

    sys->cycles = 0;
    sys->clock = CLOCK;
    sys->steps = 0;
    
    return sys;
}

void sys_dtor(struct _system *this) {

    if(this->flash != NULL)
        flash_dtor(this->flash);
        
    if(this->gpr != NULL)
        gpr_dtor(this->gpr);
        
    if(this->sreg != NULL)
        sreg_dtor(this->sreg);
        
    if(this->data != NULL)
        data_dtor(this->data);

    free(this);
}

int sys_step(struct _system *this) {

    int err = 0;
    const uint64_t old_cycles = this->cycles;
    
    progmem_t *p = flash_fetch(this->flash);

    if(p->exec == false) {

        flash_move_pc(this->flash, 1);
        this->cycles += 1;

        err = -1;

    } else if(p->dword == true) {

        uint16_t opcl = flash_read(this->flash, this->flash->pc + 1);
        (*instructions[p->key])(this, (p->opcode << 16) + opcl);

    } else {

        (*instructions[p->key])(this, p->opcode);
    }

    sys_update_io(this, (this->cycles - old_cycles));
    this->steps += 1;
    
    return err;
}

void sys_backstep(struct _system *this) {

    int counter = this->steps - 1;
    sys_reboot(this);

    while(counter-- > 0)
        sys_step(this);
}

void sys_reboot(struct _system *this) {

    this->cycles = 0;
    this->clock = CLOCK;
    
    this->steps = 0;

    flash_reboot(this->flash);
    sreg_reboot(this->sreg);
    gpr_reboot(this->gpr);
    data_reboot(this->data);
}

void sys_write_gpr(struct _system *this, const int rx, const int8_t data) {

    gpr_write(this->gpr, rx, data);
}

int8_t sys_read_gpr(const struct _system *this, const int rx) {

    return gpr_read(this->gpr, rx);
}

void sys_gpr_coi(const struct _system *this, array_t *buffer) {

    gpr_coi(this->gpr, buffer);
}

int8_t* sys_dump_gpr(const struct _system *this) {

    return gpr_dump(this->gpr);
}

void sys_write_sreg(struct _system *this, const int flag, const bool bit) {

    sreg_write(this->sreg, flag, bit);
}

bool sys_read_sreg(const struct _system *this, const int flag) {

    return sreg_read(this->sreg, flag);
}

void sys_sreg_coi(const struct _system *this, array_t *buffer) {

    sreg_coi(this->sreg, buffer);
}

uint8_t sys_dump_sreg(const struct _system *this) {

    return sreg_dump(this->sreg);
}

progmem_t* sys_read_progmem(const struct _system *this, const int addr) {

    return flash_read_progmem(this->flash, addr);
}

uint16_t sys_read_flash(const struct _system *this, const int addr) {

    return flash_read(this->flash, addr);
}

void sys_move_pc(const struct _system *this, const int inc) {

    flash_move_pc(this->flash, inc);
}

void sys_set_pc(struct _system *this, const int addr) {

    flash_set_pc(this->flash, addr);
}

int sys_get_pc(const struct _system *this) {

    return flash_get_pc(this->flash);
}

void sys_push_stack(struct _system *this, const int8_t value) {

    data_push(this->data, value);
}

int8_t sys_pop_stack(const struct _system *this) {

    return data_pop(this->data);
}

void sys_write_data(struct _system *this, const uint16_t addr, const int8_t value) {

    if(addr < GPR_SIZE) {

        gpr_write(this->gpr, addr, value);
        return;
    }
    
    if(addr == (GPR_SIZE + SREG)) {
        
        sreg_write_byte(this->sreg, value);
        return;
    }

    data_write(this->data, addr, value);
}

int8_t sys_read_data(const struct _system *this, const uint16_t addr) {

    if(addr < GPR_SIZE)
        return gpr_read(this->gpr, addr);

    if(addr == (GPR_SIZE + SREG))
        return sreg_read_byte(this->sreg);

    return data_read(this->data, addr);
}

void sys_data_coi(const struct _system *this, tuple_t *buffer) {

    data_coi(this->data, buffer);
}

int8_t* sys_dump_data(const struct _system *this) {

    return data_dump(this->data);
}

void sys_set_sfr(struct _system *this, const uint16_t addr, const int bit) {
    
    data_set_sfr(this->data, addr, bit);
}

void sys_clear_sfr(struct _system *this, const uint16_t addr, const int bit) {

    data_clear_sfr(this->data, addr, bit);
}

int8_t* sys_dump_eeprom(const struct _system *this) {

    return data_dump_eeprom(this->data);
}

/* --- Static --- */

static void sys_update_io(struct _system *this, const uint64_t dc) {

    const uint8_t sreg = sreg_dump(this->sreg);
    const bool iflag = ((sreg & (0x01 << IF)) >> IF);

    data_update_io(this->data, this->clock, dc);

    if(iflag == true) {

        const int isr = data_check_irq(this->data);
        
        if(isr >= 0)
            sys_exec_isr(this, isr);
    }
}

static void sys_exec_isr(struct _system *this, const int isr) {

    const int pc = flash_get_pc(this->flash);

    data_push(this->data, pc & 0x00ff);
    data_push(this->data, (pc & 0xff00) >> 8);

    sreg_write(this->sreg, IF, 0x00);
    this->cycles += 4;

    sys_update_io(this, 4);
    flash_set_pc(this->flash, isr);
}
