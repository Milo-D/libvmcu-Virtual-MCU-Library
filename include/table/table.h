/* Table Header */

#ifndef TABLE_H
#define TABLE_H

// C Headers
#include <stdbool.h>

typedef struct _entry entry_t;
typedef struct _array array_t;

struct _private;

typedef struct _table {

    int size;
    int breakc;
    char *source;

    struct _private *p;

} table_t;

extern struct _table* table_ctor(const char *hex_file);
extern void table_dtor(struct _table *this);

extern int table_add_breakp(struct _table *this, const char *point);
extern int table_del_breakp(struct _table *this, const char *point);

extern void table_define(struct _table *this, const char *alias, const char *seq);
extern bool table_on_breakp(const struct _table *this, const int pc);

extern entry_t* table_dump(const struct _table *this);

#endif
