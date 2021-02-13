/* System FLASH Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/system/core/flash.h"
#include "engine/include/system/util/progmem.h"
#include "engine/include/system/mcudef.h"
#include "engine/include/analyzer/report/report.h"

/* Forward Declaration of static Functions */

static void flash_erase_memory(struct _flash *this);

/* --- Extern --- */

struct _flash* flash_ctor(const report_t *report) {

    struct _flash *flash;

    if((flash = malloc(sizeof(struct _flash))) == NULL)
        return NULL;

    flash->memory = malloc(FLASH_SIZE * sizeof(progmem_t));
    flash_erase_memory(flash);

    plain_t *p = report->disassembly;

    for(int i = 0; i < report->progsize; i++) {

        const int addr = p[i].addr;

        if(addr < 0 || addr >= FLASH_SIZE)
            continue;

        flash->memory[ addr ].addr  = p[i].addr;
        flash->memory[ addr ].key   = p[i].key;
        
        flash->memory[ addr ].exec  = p[i].exec;
        flash->memory[ addr ].dword = p[i].dword;

        if(p[i].dword == false) {

            flash->memory[ addr ].opcode = p[i].opcode;
            continue;
        }
        
        const uint16_t opch = ((p[i].opcode & 0xffff0000) >> 16);
        const uint16_t opcl = ((p[i].opcode & 0x0000ffff));

        flash->memory[ addr ].opcode        = opch;
        flash->memory[ addr + 0x01 ].opcode = opcl;
    }

    flash->pc = 0x0000;
    return flash;
}

void flash_dtor(struct _flash *this) {
    
    free(this->memory);
    free(this);
}

progmem_t* flash_fetch(const struct _flash *this) {

    return &this->memory[this->pc % FLASH_SIZE];
}

progmem_t* flash_read_progmem(const struct _flash *this, const int addr) {

    return &this->memory[addr % FLASH_SIZE];
}

uint16_t flash_read(const struct _flash *this, const int addr) {

    return this->memory[addr % FLASH_SIZE].opcode;
}

void flash_move_pc(struct _flash *this, const int inc) {

    this->pc = ((this->pc + inc) % FLASH_SIZE);
}

void flash_set_pc(struct _flash *this, const int addr) {

    this->pc = (addr % FLASH_SIZE);
}

int flash_get_pc(const struct _flash *this) {

    return this->pc;
}

void flash_reboot(struct _flash *this) {

    this->pc = 0x0000;
}

/* --- Static --- */

static void flash_erase_memory(struct _flash *this) {

    for(int i = 0; i < FLASH_SIZE; i++) {

        this->memory[i].opcode = 0xffff;
        this->memory[i].addr   = i;
        this->memory[i].key    = -1;

        this->memory[i].exec   = false;
        this->memory[i].dword  = false;
    }
}




