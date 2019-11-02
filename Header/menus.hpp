/* Menu Header */

#ifndef MENUS_HPP
#define MENUS_HPP

class Table;
class Sys;

void main_menu(std::string current_f);
void table_menu(Table *table);
void debug_menu(Sys *sys, Table *table, int cursor);

#endif
