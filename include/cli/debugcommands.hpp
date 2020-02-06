/* Debug Commands Header */

#ifndef DEBUGCOMMANDS_HPP
#define DEBUGCOMMANDS_HPP

class Sys;
class Table;
class DebugWindow;

void jump_forward(DebugWindow *dwin, Sys *sys, Table *table, const int delay);
void set_breakpoint(DebugWindow *dwin, Table *table, const std::string & bp);
void remove_breakpoint(DebugWindow *dwin, Table *table, const std::string & bp);
void examine_data(DebugWindow *dwin, Sys *sys, const std::string & mem_cell);
void examine_eeprom(DebugWindow *dwin, Sys *sys, const std::string & mem_cell);
void examine_eeprom_char(DebugWindow *dwin, Sys *sys, const std::string & mem_cell, const std::string & range);
void load_eep_hex(DebugWindow *dwin, Sys *sys, const std::string & file);
void clear_output(DebugWindow *dwin);

#endif