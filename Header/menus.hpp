/* Menu Header */

#ifndef MENUS_HPP
#define MENUS_HPP

class Table;
class SimSys;

void main_menu(std::string current_f);
void table_menu(void);
void debug_menu(SimSys *sys, Table *table, int row);
void prog_int_menu(std::string msg);
std::string prog_stdin_menu(void);

#endif