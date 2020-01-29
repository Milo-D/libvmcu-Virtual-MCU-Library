/* Debug Commands Header */

#ifndef DEBUGCOMMANDS_HPP
#define DEBUGCOMMANDS_HPP

class Sys;
class Table;
class DebugWindow;

void jump_forward(DebugWindow *dwin, Sys *sys, Table *table);
void examine_data(DebugWindow *dwin, Sys *sys, std::string mem_cell);
void examine_eeprom(DebugWindow *dwin, Sys *sys, std::string mem_cell);

#endif