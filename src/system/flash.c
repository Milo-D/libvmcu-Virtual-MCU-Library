/* AVR FLASH Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "system/flash.h"
#include "system/mcudef.h"
#include "table/table.h"
#include "disassembler/decoder.h"
#include "misc/ehandling.h"
#include "misc/stringmanip.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    int pc;
    int16_t *memory;
    unsigned int mem_usage;

    array_t *keys;
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

    flash->p->table = table_ctor(file);

    array_t *buffer = array_ctor(1024, NULL, NULL);
    decode_hex(file, buffer);

    flash->p->keys = array_ctor(buffer->top, tuple_dtor, tuple_cpy);
    flash->p->mem_usage = buffer->top;

    flash->p->memory = malloc(FLASH_SIZE * sizeof(int16_t));
    memset(flash->p->memory, 0x0000, FLASH_SIZE * sizeof(int16_t));

    for(int i = 0; i < buffer->top; i++) {

        plain_t *plain = (plain_t*) array_at(buffer, i);
        flash->p->memory[ plain->addr ] = plain->opcode;

        tuple_t *key = tuple_ctor(2, INT, INT);

        tuple_set(key, (void*) &plain->addr, sizeof(int), 0);
        tuple_set(key, (void*) &plain->key, sizeof(int), 1);

        array_push(flash->p->keys, (void*) key, sizeof(tuple_t));
        tuple_dtor(key);
    }

    flash->p->pc = 0x0000;
    array_dtor(buffer);

    return flash;
}

void flash_dtor(struct _flash *this) {

    table_dtor(this->p->table);
    array_dtor(this->p->keys);

    free(this->p->memory);
    free(this->p);
    free(this);
}

int flash_fetch(const struct _flash *this, tuple_t *buffer) {

    const int16_t value = this->p->memory[this->p->pc];
    tuple_set(buffer, (void*) &value, sizeof(int16_t), 0);

    for(int i = 0; i < this->p->mem_usage; i++) {

        tuple_t *t = (tuple_t*) array_at(this->p->keys, i);
        const int addr = *((int*) tuple_get(t, 0));

        if(this->p->pc == addr) {

            const int key = *((int*) tuple_get(t, 1));
            tuple_set(buffer, (void*) &key, sizeof(int), 1);

            return 0;
        }
    }

    const int err = -1;
    tuple_set(buffer, (void*) &err, sizeof(int), 1);

    return -1;
}

int flash_move_pc(struct _flash *this) {

    if(this->p->pc == FLASH_SIZE - 1)
        return -1;

    this->p->pc += 1;
    return 0;
}

int flash_set_pc(struct _flash *this, const int addr) {

    if(addr < 0 || addr >= FLASH_SIZE)
        return -1;

    this->p->pc = addr - 1;
    table_jmp(this->p->table, addr);

    return 0;
}

int flash_get_pc(const struct _flash *this) {

    return this->p->pc;
}

void flash_reboot(const struct _flash *this) {

    this->p->pc = 0x0000;
    table_set_tip(this->p->table, 0);
}

int flash_table_step(const struct _flash *this) {

    return table_step(this->p->table);
}

bool flash_is_sync(const struct _flash *this) {

    return table_is_sync(this->p->table, this->p->pc);
}

int flash_add_breakp(const struct _flash *this, const char *point) {

    return table_add_breakp(this->p->table, point);
}

int flash_del_breakp(const struct _flash *this, const char *point) {

    return table_del_breakp(this->p->table, point);
}

void flash_set_tip(const struct _flash *this, const int line) {

    table_set_tip(this->p->table, line);
}

int flash_get_tip(const struct _flash *this) {

    return table_get_tip(this->p->table);
}

bool flash_on_breakp(const struct _flash *this) {

    return table_on_breakp(this->p->table);
}

int flash_table_size(const struct _flash *this) {

    return this->p->table->size;
}

entry_t* flash_dump_table(const struct _flash *this) {

    return table_dump(this->p->table);
}