/* AVR FLASH Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "system/core/flash.h"
#include "system/mcudef.h"
#include "table/table.h"
#include "disassembler/decoder.h"
#include "disassembler/plain.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "collections/array.h"
#include "collections/tuple.h"

/* --- Public --- */

struct _flash* flash_ctor(const char *file) {

    struct _flash *flash;

    if((flash = malloc(sizeof(struct _flash))) == NULL)
        return NULL;

    flash->memory = malloc(FLASH_SIZE * sizeof(int16_t));
    memset(flash->memory, 0x0000, FLASH_SIZE * sizeof(int16_t));

    flash->plain = array_ctor(1, NULL, NULL);
    flash->table = table_ctor(file);

    decode_hex(file, flash->plain);
    flash->mem_usage = flash->plain->top;

    for(int i = 0; i < flash->mem_usage; i++) {

        plain_t *p = (plain_t*) array_at(flash->plain, i);

        if(p->dword == true) {

            const uint16_t opch = ((p->opcode & 0xffff0000) >> 16);
            const uint16_t opcl = ((p->opcode & 0x0000ffff));

            flash->memory[ p->addr ] = opch;
            flash->memory[ p->addr + 0x01 ] = opcl;

            continue;
        }

        flash->memory[ p->addr ] = p->opcode;
    }

    flash->pc = 0x0000;
    return flash;
}

void flash_dtor(struct _flash *this) {

    table_dtor(this->table);
    array_dtor(this->plain);

    free(this->memory);
    free(this);
}

plain_t* flash_fetch(const struct _flash *this) {

    return flash_read_instr(this, this->pc);
}

plain_t* flash_read_instr(const struct _flash *this, const int addr) {

    const int tar = (addr % FLASH_SIZE);

    for(int i = 0; i < this->mem_usage; i++) {

        plain_t *p = (plain_t*) array_at(this->plain, i);

        if(p->addr == tar)
            return p;
    }

    return NULL;
}

uint16_t flash_read(const struct _flash *this, const int addr) {

    return this->memory[addr % FLASH_SIZE];
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

int flash_add_breakp(const struct _flash *this, const char *point) {

    return table_add_breakp(this->table, point);
}

int flash_del_breakp(const struct _flash *this, const char *point) {

    return table_del_breakp(this->table, point);
}

bool flash_on_breakp(const struct _flash *this) {

    return table_on_breakp(this->table, this->pc);
}

int flash_table_size(const struct _flash *this) {

    return this->table->size;
}

entry_t* flash_dump_table(const struct _flash *this) {

    return table_dump(this->table);
}
