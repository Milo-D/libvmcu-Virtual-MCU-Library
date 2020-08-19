/* Systemprinter Header */

#ifndef SYSTEMPRINTER_H
#define SYSTEMPRINTER_H

typedef struct _system system_t;
typedef struct _table table_t;
typedef struct _debugwindow debugwindow_t;

extern void system_to_win(debugwindow_t *window, system_t *sys, table_t *table);

#endif
