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
#include "disassembler/plain.h"
#include "collections/array.h"

/* --- Public --- */

struct _table* table_ctor(const char *hex_file) {

    struct _table *table;
    
    if((table = malloc(sizeof(struct _table))) == NULL)
        return NULL;

    const int len = strlen(hex_file);
    
    table->source = malloc((len + 1) * sizeof(char));
    strncpy(table->source, hex_file, len);
    table->source[len] = '\0';

    array_t *buffer = array_ctor(1, plain_dtor, plain_cpy);
    disassemble(hex_file, buffer);
   
    table->breakc = 0;
    table->size = buffer->size;

    table->entry = malloc(buffer->size * sizeof(entry_t));

    for(int i = 0; i < buffer->size; i++) {

        plain_t *p = (plain_t*) array_at(buffer, i);
        
        const size_t len = strlen(p->mnem) + 1;
        table->entry[i].ln = malloc(len * sizeof(char));
        strncpy(table->entry[i].ln, p->mnem, len);

        table->entry[i].addr = p->addr;
        table->entry[i].breakp = false;
    }

    array_dtor(buffer);
    return table;
}

void table_dtor(struct _table *this) {

    for(int i = 0; i < this->size; i++)
        free(this->entry[i].ln);

    free(this->entry);
    free(this->source);
    free(this);
}

int table_write(struct _table *this, const int lnno, const char *line) {
    
    if(lnno < 0 || lnno >= this->size)
        return -1;
        
    free(this->entry[lnno].ln);
    
    const size_t len = strlen(line) + 1;
    this->entry[lnno].ln = malloc(len * sizeof(char));
    strncpy(this->entry[lnno].ln, line, len);

    return 0;
}

char* table_read(struct _table *this, const int lnno) {
    
    if(lnno < 0 || lnno >= this->size)
        return NULL;
        
    return this->entry[lnno].ln;
}

int table_add_breakp(struct _table *this, const char *point) {

    const int line = get_int(point);

    if(line < 0 || line >= this->size)
        return -1;

    if(this->entry[line].breakp == true)
        return -1;

    if(this->entry[line].addr < 0)
        return -1;

    this->entry[line].breakp = true;
    this->breakc += 1;

    return 0;
}

int table_del_breakp(struct _table *this, const char *point) {

    const int line = get_int(point);
	
    if(line < 0 || line >= this->size)
        return -1;

    if(this->entry[line].breakp == false)
        return -1;

    if(this->entry[line].addr < 0)
        return -1;

    this->entry[line].breakp = false;
    this->breakc -= 1;

    return 0;
}

void table_define(struct _table *this, const char *alias, const char *seq) {

    for(int i = 0; i < this->size; i++) {

        int pos;
        char *line = this->entry[i].ln;

        if(this->entry[i].addr < 0) {

            char *repl = strrepl(line, alias, seq);
            const int len = strlen(repl) + 1;

            free(this->entry[i].ln);

            this->entry[i].ln = malloc(len * sizeof(char));
            strncpy(this->entry[i].ln, repl, len);
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

        free(this->entry[i].ln);
        this->entry[i].ln = malloc(len * sizeof(char));
        strncpy(this->entry[i].ln, result, len);

        nfree(4, code, comment, repl, result);
    }
}

bool table_on_breakp(const struct _table *this, const int pc) {

    for(int i = 0; i < this->size; i++) {

        if(this->entry[i].addr == pc)
            return this->entry[i].breakp;
    }

    return false;
}

entry_t* table_dump(const struct _table *this) {

    return this->entry;
}

