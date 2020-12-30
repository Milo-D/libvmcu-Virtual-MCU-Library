/* Table Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "table/table.h"
#include "table/breakpoint.h"
#include "misc/stringmanip.h"
#include "misc/memmanip.h"

/* --- Public --- */

struct _table* table_ctor(const int size) {

    struct _table *table;
    
    if((table = malloc(sizeof(struct _table))) == NULL)
        return NULL;

    table->breakpoints = malloc(size * sizeof(breakpoint_t));
    
    for(int i = 0; i < size; i++)
        table->breakpoints[i].active = false;

    table->breakc = 0;
    table->size = size;

    return table;
}

void table_dtor(struct _table *this) {

    free(this->breakpoints);
    free(this);
}

int table_add_breakp(struct _table *this, const int addr) {

    if(addr < 0 || addr >= this->size)
        return -1;

    if(this->breakpoints[addr].active == true)
        return -1;

    this->breakpoints[addr].active = true;
    this->breakc += 1;

    return 0;
}

int table_del_breakp(struct _table *this, const int addr) {
	
    if(addr < 0 || addr >= this->size)
        return -1;

    if(this->breakpoints[addr].active == false)
        return -1;

    this->breakpoints[addr].active = false;
    this->breakc -= 1;

    return 0;
}

bool table_on_breakp(const struct _table *this, const int addr) {

    return this->breakpoints[addr].active;
}

