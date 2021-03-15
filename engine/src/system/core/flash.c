/* System FLASH Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/system/core/flash.h"
#include "engine/include/system/util/progmem.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"
#include "engine/include/misc/nummanip.h"

/* Forward Declaration of static Functions */

static void flash_erase_memory(vmcu_flash_t *this);

/* --- Extern --- */

vmcu_flash_t* vmcu_flash_ctor(const vmcu_report_t *report) {

    vmcu_flash_t *flash;

    if((flash = malloc(sizeof(vmcu_flash_t))) == NULL)
        return NULL;

    flash->memory = malloc(FLASH_SIZE * sizeof(vmcu_progmem_t));
    flash_erase_memory(flash);

    vmcu_instr_t *instr = report->disassembly;

    for(int i = 0; i < report->progsize; i++) {

        const int addr = instr[i].addr;

        if(addr < 0 || addr >= FLASH_SIZE)
            continue;

        flash->memory[ addr ].addr  = instr[i].addr;
        flash->memory[ addr ].key   = instr[i].key;
        
        flash->memory[ addr ].exec  = instr[i].exec;
        flash->memory[ addr ].dword = instr[i].dword;

        if(instr[i].dword == false) {

            flash->memory[ addr ].opcode = instr[i].opcode;
            continue;
        }
        
        const uint16_t opch = ((instr[i].opcode & 0xffff0000) >> 16);
        const uint16_t opcl = ((instr[i].opcode & 0x0000ffff));

        flash->memory[ addr ].opcode        = opch;
        flash->memory[ addr + 0x01 ].opcode = opcl;
    }

    flash->pc = 0x0000;
    return flash;
}

void vmcu_flash_dtor(vmcu_flash_t *this) {
    
    free(this->memory);
    free(this);
}

vmcu_progmem_t* vmcu_flash_fetch(const vmcu_flash_t *this) {

    return &this->memory[ mod(this->pc, FLASH_SIZE) ];
}

vmcu_progmem_t* vmcu_flash_read_progmem(const vmcu_flash_t *this, const int addr) {

    return &this->memory[ mod(addr, FLASH_SIZE) ];
}

uint16_t vmcu_flash_read(const vmcu_flash_t *this, const int addr) {

    return this->memory[ mod(addr, FLASH_SIZE) ].opcode;
}

void vmcu_flash_move_pc(vmcu_flash_t *this, const int inc) {

    this->pc = mod((this->pc + inc), FLASH_SIZE);
}

void vmcu_flash_set_pc(vmcu_flash_t *this, const int addr) {

    this->pc = mod(addr, FLASH_SIZE);
}

int vmcu_flash_get_pc(const vmcu_flash_t *this) {

    return this->pc;
}

void vmcu_flash_reboot(vmcu_flash_t *this) {

    this->pc = 0x0000;
}

/* --- Static --- */

static void flash_erase_memory(vmcu_flash_t *this) {

    for(int i = 0; i < FLASH_SIZE; i++) {

        this->memory[i].opcode = 0xffff;
        this->memory[i].addr   = i;
        this->memory[i].key    = -1;

        this->memory[i].exec   = false;
        this->memory[i].dword  = false;
    }
}




