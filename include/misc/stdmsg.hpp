/* Standard Output Messages */

#ifndef STDMSG_HPP
#define STDMSG_HPP

#define INIT_SUCCESS "(mdx) DebugWindow successfully initialized.\n"
#define sim_start(f) "(mdx) Starting simulation of " + f + "...\n"
#define SYS_TERM "(mdx) System terminated. Simulation has stopped.\n"

#define MEM_CELL_ERR "(mdx) Illegal memory cell.\n"

#define val_of(c, v) "(mdx) " + c + " ~> 0x" + v + "\n"
#define val_of_eep(a, o, t) "(mdx) " + a + " [" + o + "] ~> \"" + t + "\"\n"

#define BREAK_REACHED "(mdx) <--- Breakpoint reached --->\n"
#define eep_success(f) "(mdx) Successfully loaded \"" + f + "\" into EEPROM.\n"

#define EEP_DECODE_ERR "(mdx) Could not decode EEPROM Hex File.\n"
#define file_invalid(f) "(mdx) Could not open file \"" + f + "\"\n"

#define MAX_FILE_REACH "Maximum of 4 files reached."

#endif