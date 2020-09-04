/* Debug Commands Header */

#ifndef DEBUGCOMMANDS_H
#define DEBUGCOMMANDS_H

typedef struct _debugwindow debugwindow_t;
typedef struct _system system_t;
typedef struct _table table_t;

extern void jump_forward(debugwindow_t *window, system_t *sys, table_t *table, const int delay);
extern void set_breakpoint(debugwindow_t *window, table_t *table, const char *bp);
extern void remove_breakpoint(debugwindow_t *window, table_t *table, const char *bp);
extern void examine_data(debugwindow_t *window, system_t *sys, const char *mem_cell);
extern void examine_eeprom(debugwindow_t *window, system_t *sys, const char *mem_cell);
extern void examine_data_char(debugwindow_t *window, system_t *sys, const char *mem_cell, const char *range);
extern void examine_eeprom_char(debugwindow_t *window, system_t *sys, const char *mem_cell, const char *range);
extern void load_eep_hex(debugwindow_t *window, system_t *sys, const char *file);
extern void clear_output(debugwindow_t *window);
extern void show_cycles(debugwindow_t *window, system_t *sys);
extern void show_clock(debugwindow_t *window, system_t *sys);
extern void show_time(debugwindow_t *window, system_t *sys);

#endif