/* (Debug CLI) Printer Header */

#ifndef DEBUGPRINTER_H
#define DEBUGPRINTER_H

typedef struct _dbg dbg_t;
typedef struct _debugwindow debugwindow_t;

extern void print_debug(debugwindow_t *window, dbg_t *dbg);

#endif
