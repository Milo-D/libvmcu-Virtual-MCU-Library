/* System Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/system/system.h"
#include "engine/include/system/core/gpr.h"
#include "engine/include/system/core/sreg.h"
#include "engine/include/system/core/data.h"
#include "engine/include/system/core/flash.h"
#include "engine/include/analyzer/report/report.h"
#include "engine/include/instructions/instructions.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"

/* Forward Declaration of static Functions */

static void system_update_io(vmcu_system_t *this, const uint64_t dc);
static void system_exec_isr(vmcu_system_t *this, const int isr);

/* --- Extern --- */

vmcu_system_t* vmcu_system_ctor(const vmcu_report_t *report) {

    vmcu_system_t *sys;
    
    if((sys = malloc(sizeof(vmcu_system_t))) == NULL)
        return NULL;

    if((sys->flash = vmcu_flash_ctor(report)) == NULL) {

        vmcu_system_dtor(sys);
        return NULL;
    }

    if((sys->gpr = vmcu_gpr_ctor()) == NULL) {

        vmcu_system_dtor(sys);
        return NULL;
    }

    if((sys->sreg = vmcu_sreg_ctor()) == NULL) {

        vmcu_system_dtor(sys);
        return NULL;
    }
    
    if((sys->data = vmcu_data_ctor()) == NULL) {
    
        vmcu_system_dtor(sys);
        return NULL;
    }

    sys->cycles = 0;
    sys->clock = CLOCK;
    sys->steps = 0;
    
    return sys;
}

void vmcu_system_dtor(vmcu_system_t *this) {

    if(this->flash != NULL)
        vmcu_flash_dtor(this->flash);
        
    if(this->gpr != NULL)
        vmcu_gpr_dtor(this->gpr);
        
    if(this->sreg != NULL)
        vmcu_sreg_dtor(this->sreg);
        
    if(this->data != NULL)
        vmcu_data_dtor(this->data);

    free(this);
}

int vmcu_system_step(vmcu_system_t *this) {

    int err = 0;

    const uint64_t old_cycles = this->cycles;
    vmcu_progmem_t *p = vmcu_flash_fetch(this->flash);

    if(p->exec == false) {

        vmcu_flash_move_pc(this->flash, 1);
        this->cycles += 1;

        err = -1;

    } else if(p->dword == true) {

        uint16_t opcl = vmcu_flash_read(this->flash, this->flash->pc + 1);
        (*vmcu_execute[p->key])(this, (p->opcode << 16) + opcl);

    } else {

        (*vmcu_execute[p->key])(this, p->opcode);
    }

    system_update_io(this, (this->cycles - old_cycles));
    this->steps += 1;
    
    return err;
}

void vmcu_system_backstep(vmcu_system_t *this) {

    int counter = this->steps - 1;
    vmcu_system_reboot(this);

    while(counter-- > 0)
        vmcu_system_step(this);
}

void vmcu_system_reboot(vmcu_system_t *this) {

    this->cycles = 0;
    this->clock = CLOCK;
    
    this->steps = 0;

    vmcu_flash_reboot(this->flash);
    vmcu_sreg_reboot(this->sreg);
    vmcu_gpr_reboot(this->gpr);
    vmcu_data_reboot(this->data);
}

void vmcu_system_write_gpr(vmcu_system_t *this, const int rx, const int8_t data) {

    vmcu_gpr_write(this->gpr, rx, data);
}

int8_t vmcu_system_read_gpr(const vmcu_system_t *this, const int rx) {

    return vmcu_gpr_read(this->gpr, rx);
}

int8_t* vmcu_system_dump_gpr(const vmcu_system_t *this) {

    return vmcu_gpr_dump(this->gpr);
}

void vmcu_system_write_sreg(vmcu_system_t *this, const int flag, const bool bit) {

    vmcu_sreg_write(this->sreg, flag, bit);
}

bool vmcu_system_read_sreg(const vmcu_system_t *this, const int flag) {

    return vmcu_sreg_read(this->sreg, flag);
}

uint8_t vmcu_system_dump_sreg(const vmcu_system_t *this) {

    return vmcu_sreg_dump(this->sreg);
}

vmcu_progmem_t* vmcu_system_read_progmem(const vmcu_system_t *this, const int addr) {

    return vmcu_flash_read_progmem(this->flash, addr);
}

uint16_t vmcu_system_read_flash(const vmcu_system_t *this, const int addr) {

    return vmcu_flash_read(this->flash, addr);
}

void vmcu_system_move_pc(const vmcu_system_t *this, const int inc) {

    vmcu_flash_move_pc(this->flash, inc);
}

void vmcu_system_set_pc(vmcu_system_t *this, const int addr) {

    vmcu_flash_set_pc(this->flash, addr);
}

int vmcu_system_get_pc(const vmcu_system_t *this) {

    return vmcu_flash_get_pc(this->flash);
}

void vmcu_system_push_stack(vmcu_system_t *this, const int8_t value) {

    vmcu_data_push(this->data, value);
}

int8_t vmcu_system_pop_stack(const vmcu_system_t *this) {

    return vmcu_data_pop(this->data);
}

void vmcu_system_write_data(vmcu_system_t *this, const uint16_t addr, const int8_t value) {

    if(addr < GPR_SIZE) {

        vmcu_gpr_write(this->gpr, addr, value);
        return;
    }
    
    if(addr == (GPR_SIZE + SREG)) {
        
        vmcu_sreg_write_byte(this->sreg, value);
        return;
    }

    vmcu_data_write(this->data, addr, value);
}

int8_t vmcu_system_read_data(const vmcu_system_t *this, const uint16_t addr) {

    if(addr < GPR_SIZE)
        return vmcu_gpr_read(this->gpr, addr);

    if(addr == (GPR_SIZE + SREG))
        return vmcu_sreg_read_byte(this->sreg);

    return vmcu_data_read(this->data, addr);
}

int8_t* vmcu_system_dump_data(const vmcu_system_t *this) {

    return vmcu_data_dump(this->data);
}

void vmcu_system_set_sfr(vmcu_system_t *this, const uint16_t addr, const int bit) {
    
    vmcu_data_set_sfr(this->data, addr, bit);
}

void vmcu_system_clear_sfr(vmcu_system_t *this, const uint16_t addr, const int bit) {

    vmcu_data_clear_sfr(this->data, addr, bit);
}

int8_t* vmcu_system_dump_eeprom(const vmcu_system_t *this) {

    return vmcu_data_dump_eeprom(this->data);
}

/* --- Static --- */

static void system_update_io(vmcu_system_t *this, const uint64_t dc) {

    const uint8_t sreg = vmcu_sreg_dump(this->sreg);
    const bool iflag = ((sreg & (0x01 << IF)) >> IF);

    vmcu_data_update_io(this->data, this->clock, dc);

    if(iflag == true) {

        const int isr = vmcu_data_check_irq(this->data);
        
        if(isr >= 0)
            system_exec_isr(this, isr);
    }
}

static void system_exec_isr(vmcu_system_t *this, const int isr) {

    const int pc = vmcu_flash_get_pc(this->flash);

    vmcu_data_push(this->data, pc & 0x00ff);
    vmcu_data_push(this->data, (pc & 0xff00) >> 8);

    vmcu_sreg_write(this->sreg, IF, 0x00);
    this->cycles += 4;

    system_update_io(this, 4);
    vmcu_flash_set_pc(this->flash, isr);
}
