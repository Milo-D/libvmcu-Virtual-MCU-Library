/* Debug Commands Header */

#ifndef DEBUGCOMMANDS_H
#define DEBUGCOMMANDS_H

typedef struct _debugwindow debugwindow_t;
typedef struct _dbg dbg_t;

extern void command_rn(debugwindow_t *window);
extern void command_rp(debugwindow_t *window);
extern void command_dn(debugwindow_t *window);
extern void command_dp(debugwindow_t *window);
extern void command_en(debugwindow_t *window);
extern void command_ep(debugwindow_t *window);
extern void command_pn(debugwindow_t *window);
extern void command_pp(debugwindow_t *window);

extern void command_n(debugwindow_t *window, dbg_t *dbg);
extern void command_b(debugwindow_t *window, dbg_t *dbg);
extern void command_jb(debugwindow_t *window, dbg_t *dbg, const int delay);
extern void command_jc(debugwindow_t *window, dbg_t *dbg, const int n);
extern void command_break(debugwindow_t *window, dbg_t *dbg, const char *bp);
extern void command_unbreak(debugwindow_t *window, dbg_t *dbg, const char *bp);
extern void command_xd(debugwindow_t *window, dbg_t *dbg, const char *mem_cell);
extern void command_xe(debugwindow_t *window, dbg_t *dbg, const char *mem_cell);
extern void command_xdc(debugwindow_t *window, dbg_t *dbg, const char *mem_cell, const char *range);
extern void command_xec(debugwindow_t *window, dbg_t *dbg, const char *mem_cell, const char *range);
extern void command_leep(debugwindow_t *window, dbg_t *dbg, const char *file);
extern void command_clear(debugwindow_t *window);
extern void command_cycles(debugwindow_t *window, dbg_t *dbg);
extern void command_clock(debugwindow_t *window, dbg_t *dbg);
extern void command_time(debugwindow_t *window, dbg_t *dbg);
extern void command_xdb(debugwindow_t *window, dbg_t *dbg, const char *mem_cell);
extern void command_cc(debugwindow_t *window, dbg_t *dbg, const char *line, const char *comment);
extern void command_xeb(debugwindow_t *window, dbg_t *dbg, const char *mem_cell);
extern void command_jcb(debugwindow_t *window, dbg_t *dbg, const int n);
extern void command_def(debugwindow_t *window);

#endif
