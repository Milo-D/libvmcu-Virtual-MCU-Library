/* Debug Commands Header */

#ifndef DEBUGCOMMANDS_HPP
#define DEBUGCOMMANDS_HPP

class Sys;
class Table;
class DebugWindow;

void jump_forward(DebugWindow *dwin, Sys *sys, Table *table);
void set_breakpoint(DebugWindow *dwin, Table *table, std::string bp);
void remove_breakpoint(DebugWindow *dwin, Table *table, std::string bp);
void examine_data(DebugWindow *dwin, Sys *sys, std::string mem_cell);
void examine_eeprom(DebugWindow *dwin, Sys *sys, std::string mem_cell);
void examine_eeprom_char(DebugWindow *dwin, Sys *sys, std::string mem_cell, std::string range);
void load_eep_hex(DebugWindow *dwin, Sys *sys, std::string file);

#endif