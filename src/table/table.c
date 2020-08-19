/* Table Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "table/table.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "disassembler/disassembler.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {

    int tip;
    int size;
    int breakc;
    char *source;
    
    array_t *file;
    array_t *breakp;
};

/* --- Public --- */

struct _table* table_ctor(const char *hex_file) {

    struct _table *table;
    
    if((table = malloc(sizeof(struct _table))) == NULL)
        return NULL;
        
    if((table->p = malloc(sizeof(struct _private))) == NULL) {
    
        free(table);
        return NULL;
    }

    const int len = strlen(hex_file);
    
    table->p->source = malloc((len + 1) * sizeof(char));
    strncpy(table->p->source, hex_file, len);
    table->p->source[len] = '\0';

    table->p->file = array_ctor(1, tuple_dtor, tuple_cpy);
    disassemble(hex_file, table->p->file);
   
    table->p->tip = table->p->breakc = 0;
    table->p->size = table->p->file->size;

    table->p->breakp = array_ctor(table->p->size, NULL, NULL);
  
    for(int i = 0; i < table->p->size; i++) {
    
        const bool init = false;
        array_push(table->p->breakp, (void*) &init, sizeof(bool));
    }
    
    return table;
}

void table_dtor(struct _table *this) {

    array_dtor(this->p->file);
    array_dtor(this->p->breakp);

    free(this->p->source);
    free(this->p);
    free(this);
}

int table_step(struct _table *this) {

    if(this->p->tip >= this->p->size - 1)
        return -1;

    if(this->p->tip < 0)
        return -1;

    this->p->tip += 1;
    return 0;
}

int table_add_breakp(struct _table *this, const char *point) {

    const int line = get_int(point);

    if(line < 0 || line >= this->p->size)
        return -1;

    if(*((bool*) array_at(this->p->breakp, line)) == true)
        return -1;

    const bool set = true;
    array_set(this->p->breakp, (void*) &set, sizeof(bool), line);
    this->p->breakc += 1;

    return 0;
}

int table_del_breakp(struct _table *this, const char *point) {

    const int line = get_int(point);
	
    if(line < 0 || line >= this->p->size)
        return -1;

    if(*((bool*) array_at(this->p->breakp, line)) == false)
        return -1;

    const bool unset = false;
    array_set(this->p->breakp, (void*) &unset, sizeof(bool), line);
    this->p->breakc -= 1;

    return 0;
}

void table_define(struct _table *this, const char *alias, const char *seq) {

    for(int i = 0; i < this->p->size; i++) {

        int pos;

        tuple_t *entry = (tuple_t*) array_at(this->p->file, i);
        char *line = (char*) tuple_get(entry, 0);

        if(*((int*) tuple_get(entry, 1)) < 0) {

            char *repl = strrepl(line, alias, seq);

            const int len = strlen(repl) + 1;
            tuple_set(entry, (void*) repl, len * sizeof(char), 0);
            free(repl);

            continue;
        }

        if((pos = strpos(line, ";")) < 0)
            continue;

        char *code = substr(line, 0, pos - 1);
        char *comment = substr(line, pos, strlen(line));

        char *repl = strrepl(code, alias, seq);
        char *result = strxcat(repl, comment);

        const int len = strlen(result) + 1;
        tuple_set(entry, (void*) result, len * sizeof(char), 0);

        nfree(4, code, comment, repl, result);
    }
}

void table_set_tip(struct _table *this, const int line) {

    if(line >= this->p->size)
        return;

    if(line < 0)
        return;

    this->p->tip = line;
}

int table_get_tip(const struct _table *this) {

    return this->p->tip;
}

void table_jmp(struct _table *this, const int exec_addr) {

    int i = 0;

    while(true) {

        tuple_t *entry = (tuple_t*) array_at(this->p->file, i);

        if(*((int*) tuple_get(entry, 1)) == exec_addr)
            break;

        i += 1;

        if(i >= this->p->size) {

            table_set_tip(this, -1);
            return;
        }
    }

    tuple_t *pre = array_at(this->p->file, i - 2);

    if(*((int*) tuple_get(pre , 1)) < 0) {

        table_set_tip(this, i - 2);
        return;
    }

    table_set_tip(this, i - 1);
}

bool table_is_breakp(const struct _table *this) {

    return *((bool*) array_at(this->p->breakp, this->p->tip));
}

bool table_has_breakp(const struct _table *this) {

    return (this->p->breakc > 0);
}

bool table_is_sync(const struct _table *this, const int hex_addr) {

    const int tip = this->p->tip;

    tuple_t *entry = (tuple_t*) array_at(this->p->file, tip);
    const int match = *((int*) tuple_get(entry, 1));

    return (match == hex_addr);
}

int table_size(const struct _table *this) {

    return this->p->size;
}

char* table_source(const struct _table *this) {

    return this->p->source;
}

void table_content(const struct _table *this, array_t *buffer) {

    for(int i = 0; i < this->p->size; i++) {
    
        tuple_t *entry = (tuple_t*) array_at(this->p->file, i);

        const char *fcon = (char*) tuple_get(entry, 0);
        const size_t len = strlen(fcon);
        
        array_push(buffer, (void*) fcon, (len + 1) * sizeof(char));
    }
}

void table_breakp(const struct _table *this, array_t *buffer) {

    for(int i = 0; i < this->p->size; i++) {

        const bool brk = *((bool*) array_at(this->p->breakp, i));
        array_push(buffer, (void*) &brk, sizeof(bool));
    }
}


