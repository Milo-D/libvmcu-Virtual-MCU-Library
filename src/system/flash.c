/* AVR FLASH Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "system/flash.h"
#include "system/mcudef.h"
#include "table/table.h"
#include "disassembler/decoder.h"
#include "disassembler/plain.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    int pc;
    int16_t *memory;
    unsigned int mem_usage;

    array_t *plain;
    table_t *table;
};

/* --- Public --- */

struct _flash* flash_ctor(const char *file) {

    struct _flash *flash;

    if((flash = malloc(sizeof(struct _flash))) == NULL)
        return NULL;

    if((flash->p = malloc(sizeof(struct _private))) == NULL) {

        free(flash);
        return NULL;
    }

    flash->p->memory = malloc(FLASH_SIZE * sizeof(int16_t));
    memset(flash->p->memory, 0x0000, FLASH_SIZE * sizeof(int16_t));

    flash->p->plain = array_ctor(1, NULL, NULL);
    flash->p->table = table_ctor(file);

    decode_hex(file, flash->p->plain);
    flash->p->mem_usage = flash->p->plain->top;

    for(int i = 0; i < flash->p->mem_usage; i++) {

        plain_t *p = (plain_t*) array_at(flash->p->plain, i);

        if(p->dword == true) {

            const uint16_t opch = ((p->opcode & 0xffff0000) >> 16);
            const uint16_t opcl = ((p->opcode & 0x0000ffff));

            flash->p->memory[ p->addr ] = opch;
            flash->p->memory[ p->addr + 0x01 ] = opcl;

            continue;
        }

        flash->p->memory[ p->addr ] = p->opcode;
    }

    flash->p->pc = 0x0000;
    return flash;
}

void flash_dtor(struct _flash *this) {

    table_dtor(this->p->table);
    array_dtor(this->p->plain);

    free(this->p->memory);
    free(this->p);
    free(this);
}

plain_t* flash_fetch(const struct _flash *this) {

    return flash_read_instr(this, this->p->pc);
}

plain_t* flash_read_instr(const struct _flash *this, const int addr) {

    const int tar = (addr % FLASH_SIZE);

    for(int i = 0; i < this->p->mem_usage; i++) {

        plain_t *p = (plain_t*) array_at(this->p->plain, i);

        if(p->addr == tar)
            return p;
    }

    return NULL;
}

uint16_t flash_read(const struct _flash *this, const int addr) {

    return this->p->memory[addr % FLASH_SIZE];
}

void flash_move_pc(const struct _flash *this, const int inc) {

    this->p->pc = ((this->p->pc + inc) % FLASH_SIZE);
}

void flash_set_pc(struct _flash *this, const int addr) {

    this->p->pc = (addr % FLASH_SIZE);
}

int flash_get_pc(const struct _flash *this) {

    return this->p->pc;
}

void flash_reboot(const struct _flash *this) {

    this->p->pc = 0x0000;
}

int flash_add_breakp(const struct _flash *this, const char *point) {

    return table_add_breakp(this->p->table, point);
}

int flash_del_breakp(const struct _flash *this, const char *point) {

    return table_del_breakp(this->p->table, point);
}

bool flash_on_breakp(const struct _flash *this) {

    return table_on_breakp(this->p->table, this->p->pc);
}

int flash_table_size(const struct _flash *this) {

    return this->p->table->size;
}

entry_t* flash_dump_table(const struct _flash *this) {

    return table_dump(this->p->table);
}