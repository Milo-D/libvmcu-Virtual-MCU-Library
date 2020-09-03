/* Table Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "table/table.h"
#include "table/entry.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "disassembler/disassembler.h"
#include "collections/array.h"
#include "collections/tuple.h"

struct _private {
    
    int tip;
    entry_t *entry;
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
    
    table->source = malloc((len + 1) * sizeof(char));
    strncpy(table->source, hex_file, len);
    table->source[len] = '\0';

    array_t *buffer = array_ctor(1, tuple_dtor, tuple_cpy);
    disassemble(hex_file, buffer);
   
    table->p->tip = table->breakc = 0;
    table->size = buffer->size;

    table->p->entry = malloc(buffer->size * sizeof(entry_t));

    for(int i = 0; i < buffer->size; i++) {

        tuple_t *t = (tuple_t*) array_at(buffer, i);
        
        const char *content = (char*) tuple_get(t, 0);
        const size_t bytes = strlen(content) * sizeof(char);

        table->p->entry[i].ln = malloc(bytes + sizeof(char));
        strncpy(table->p->entry[i].ln, content, strlen(content) + 1);

        table->p->entry[i].addr = *((int*) tuple_get(t, 1));
        table->p->entry[i].breakp = false;
    }

    array_dtor(buffer);
    return table;
}

void table_dtor(struct _table *this) {

    for(int i = 0; i < this->size; i++)
        free(this->p->entry[i].ln);

    free(this->p->entry);
    free(this->source);
    free(this->p);
    free(this);
}

int table_step(struct _table *this) {

    if(this->p->tip >= this->size - 1)
        return -1;

    if(this->p->tip < 0)
        return -1;

    this->p->tip += 1;
    return 0;
}

int table_add_breakp(struct _table *this, const char *point) {

    const int line = get_int(point);

    if(line < 0 || line >= this->size)
        return -1;

    if(this->p->entry[line].breakp == true)
        return -1;

    this->p->entry[line].breakp = true;
    this->breakc += 1;

    return 0;
}

int table_del_breakp(struct _table *this, const char *point) {

    const int line = get_int(point);
	
    if(line < 0 || line >= this->size)
        return -1;

    if(this->p->entry[line].breakp == false)
        return -1;

    this->p->entry[line].breakp = false;
    this->breakc -= 1;

    return 0;
}

void table_define(struct _table *this, const char *alias, const char *seq) {

    for(int i = 0; i < this->size; i++) {

        int pos;
        char *line = this->p->entry[i].ln;

        if(this->p->entry[i].addr < 0) {

            char *repl = strrepl(line, alias, seq);
            const int len = strlen(repl) + 1;

            free(this->p->entry[i].ln);

            this->p->entry[i].ln = malloc(len * sizeof(char));
            strncpy(this->p->entry[i].ln, repl, len);
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

        free(this->p->entry[i].ln);
        this->p->entry[i].ln = malloc(len * sizeof(char));
        strncpy(this->p->entry[i].ln, result, len);

        nfree(4, code, comment, repl, result);
    }
}

void table_set_tip(struct _table *this, const int line) {

    if(line >= this->size)
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

    while(this->p->entry[i].addr != exec_addr) {

        i += 1;

        if(i >= this->size) {

            table_set_tip(this, -1);
            return;
        }
    }

    table_set_tip(this, i - 1);
}

bool table_is_breakp(const struct _table *this) {

    return this->p->entry[this->p->tip].breakp;
}

bool table_is_sync(const struct _table *this, const int hex_addr) {

    const int tip = this->p->tip;
    return (this->p->entry[tip].addr == hex_addr);
}

void table_content(const struct _table *this, array_t *buffer) {

    for(int i = 0; i < this->size; i++) {
    
        const char *ln = this->p->entry[i].ln;
        const size_t bytes = strlen(ln) * sizeof(char);

        array_push(buffer, (void*) ln, bytes + sizeof(char));
    }
}

void table_breakp(const struct _table *this, array_t *buffer) {

    for(int i = 0; i < this->size; i++) {

        const bool breakp = this->p->entry[i].breakp;
        array_push(buffer, (void*) &breakp, sizeof(bool));
    }
}


