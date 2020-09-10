/* Standard Output Messages */

#ifndef STDMSG_H
#define STDMSG_H

// C Headers
#include <inttypes.h>

#define MAX_FILE_REACH "Can not open more than 4 files."
#define SYS_INIT_ERR "Could not initialize System."

#define INIT_SUCCESS "(mdx) DebugWindow successfully initialized.\n"
#define ILLEGAL_OPC "(mdx) Warning: Executing illegal opcode.\n"
#define SIM_TERM "(mdx) System terminated. Simulation has stopped.\n"

#define JUMP_START "(mdx) Jumping to next Breakpoint...\n"
#define BREAK_REACHED "(mdx) <--- Breakpoint reached --->\n"
#define DELAY_ERR "(mdx) Invalid delay. Usage: jb <delay in ms>\n"

#define MEM_CELL_ERR "(mdx) Illegal memory cell.\n"
#define MEM_RANGE_ERR "(mdx) Literal length must be > 0.\n"
#define EEP_DECODE_ERR "(mdx) Could not decode EEPROM Hex File.\n"

#define NOT_AVAIL "(mdx) This command is currently unavailable.\n"


extern char* sim_start(const char *file);

extern char* val_of(const char *cell, const char *value);
extern char* val_of_eep(const char *addr, const char *range, const char *value);
extern char* val_of_data(const char *addr, const char *range, const char *value);

extern char* bp_set_success(const char *line);
extern char* bp_set_failure(const char *line);

extern char* bp_del_success(const char *line);
extern char* bp_del_failure(const char *line);

extern char* eep_success(const char *file);
extern char* file_err(const char *file);

extern char* get_cycles(const uint64_t cycles);
extern char* get_clock(const uint32_t clock);
extern char* get_time(const double time);

#endif