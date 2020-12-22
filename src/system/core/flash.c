/* AVR FLASH Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "system/core/flash.h"
#include "system/mcudef.h"
#include "table/table.h"
#include "decoder/decoder.h"
#include "decoder/plain.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "collections/array.h"
#include "collections/tuple.h"

/* --- Public --- */

struct _flash* flash_ctor(const char *file) {

    struct _flash *flash;

    if((flash = malloc(sizeof(struct _flash))) == NULL)
        return NULL;

    flash->memory = malloc(FLASH_SIZE * sizeof(plain_t));

    array_t *buffer = array_ctor(1, NULL, NULL);
    flash->table = table_ctor(file);

    decode_hex(file, buffer);
    flash->mem_usage = buffer->top;

    for(int i = 0; i < FLASH_SIZE; i++) {

        flash->memory[i].opcode = 0xffff;
        flash->memory[i].addr = i;
        flash->memory[i].key = -1;

        flash->memory[i].mnem = NULL; 
        flash->memory[i].exec = false;
        flash->memory[i].dword = false;
    }

    for(int i = 0; i < flash->mem_usage; i++) {

        plain_t *p = (plain_t*) array_at(buffer, i);

        if(p->addr >= FLASH_SIZE)
            continue;

        flash->memory[ p->addr ].addr = p->addr;
        flash->memory[ p->addr ].key = p->key;
        
        flash->memory[ p->addr ].exec = p->exec;
        flash->memory[ p->addr ].dword = p->dword;

        if(p->dword == true) {

            const uint16_t opch = ((p->opcode & 0xffff0000) >> 16);
            const uint16_t opcl = ((p->opcode & 0x0000ffff));

            flash->memory[ p->addr ].opcode = opch;
            flash->memory[ p->addr + 0x01 ].opcode = opcl;

            continue;
        }

        flash->memory[ p->addr ].opcode = p->opcode;
    }

    flash->pc = 0x0000;
    array_dtor(buffer);

    return flash;
}

void flash_dtor(struct _flash *this) {

    table_dtor(this->table);
    
    free(this->memory);
    free(this);
}

plain_t* flash_fetch(const struct _flash *this) {

    return &this->memory[this->pc % FLASH_SIZE];
}

plain_t* flash_read_instr(const struct _flash *this, const int addr) {

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

int flash_write_table(struct _flash *this, const int lnno, const char *line) {
    
    return table_write(this->table, lnno, line);
}

char* flash_read_table(struct _flash *this, const int lnno) {
    
    return table_read(this->table, lnno);
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
