/* Table Header */

#ifndef TABLE_H
#define TABLE_H

// C Headers
#include <stdbool.h>

typedef struct _breakpoint breakpoint_t;

typedef struct _table {

    int size;
    int breakc;

    breakpoint_t *breakpoints;

} table_t;

extern struct _table* table_ctor(const int size);
extern void table_dtor(struct _table *this);

extern int table_add_breakp(struct _table *this, const char *point);
extern int table_del_breakp(struct _table *this, const char *point);
extern bool table_on_breakp(const struct _table *this, const int addr);

#endif
