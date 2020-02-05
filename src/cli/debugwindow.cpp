/* DebugWindow Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <ncurses.h>

// Project Headers
#include "cli/debugwindow.hpp"
#include "cli/panel.hpp"
#include "cli/prompt.hpp"
#include "system/mcu.hpp"

using namespace std;

DebugWindow::DebugWindow(const int table_size) {

    initscr();
    cbreak();
    refresh();
    echo();
    curs_set(0);
    start_color();

    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int mx = (scr_x / 2);
    const int mmx = (mx - (mx / 2));
    const int gy = (scr_y - 40);

    this->panel = (Panel**) malloc(N_PANEL * sizeof(Panel*));

    const int dim[N_PANEL][4] = { 

        { 6, mx, 0, 0                     },
        { 6, mx, 6, 0                     },
        { 13, mx, 12, 0                   },
        { 13, (mx / 2), 25, 0             },  
        { 13, mmx, 25, (mx / 2)           }, 
        { (gy - 1), mx, 38, 0             },
        { (scr_y - 3), (scr_x / 2), 0, mx }
    };

    int n_pages = (table_size / (scr_y - 4));
    n_pages += ((table_size % (scr_y - 4)) != 0);

    const int cs[N_PANEL] = { 0, 0, 0, 0x0060, (EEPROM_SIZE / 2), 
                              0, 0 };

    const int cr[N_PANEL] = { (GPR_SIZE / 8), 0, 0, (RAM_END + 1), 
                              (EEPROM_SIZE), 0, n_pages };

    for(int i = 0; i < N_PANEL; i++) {

        int h = dim[i][0]; int w = dim[i][1];
        int y = dim[i][2]; int x = dim[i][3];

        this->panel[i] = new Panel(h, w, y, x, cs[i], cr[i]);
    }

    this->prompt = new Prompt(3, scr_x, (38 + gy - 1), 0);
}

DebugWindow::~DebugWindow(void) {

    for(int i = 0; i < N_PANEL; i++)
        delete this->panel[i];

    free(this->panel);
    endwin();
}

string DebugWindow::read_prompt(void) {

    return this->prompt->read();
}

void DebugWindow::add(const int ptype, const string & data, const int color) {

    this->panel[ptype]->add(data, color);
}

void DebugWindow::write(const int ptype, const string & data, const int color) {

    this->panel[ptype]->write(data, color);
}

void DebugWindow::clear_specific(const int ptype) {

    this->panel[ptype]->clear();
}

void DebugWindow::clear(void) {

    for(int i = 0; i < 5; i++)
        this->panel[i]->clear();

    this->panel[SIDE_PANEL]->clear();
}

void DebugWindow::update(const int ptype) {

    this->panel[ptype]->update();
}

void DebugWindow::update_all(void) {

    for(int i = 0; i < N_PANEL; i++)
        this->panel[i]->update();
}

void DebugWindow::move_cursor(const int ptype, const int offs) {

    this->panel[ptype]->move_cursor(offs);
}

void DebugWindow::set_cursor(const int ptype, const int at) {

    this->panel[ptype]->set_cursor(at);
}

int DebugWindow::cursor_of(const int ptype) {

    return this->panel[ptype]->get_cursor();
}

int DebugWindow::get_height(const int ptype) {

    return this->panel[ptype]->get_height();
}

int DebugWindow::get_width(const int ptype) {

    return this->panel[ptype]->get_width();
}

int DebugWindow::get_y(const int ptype) {

    return this->panel[ptype]->get_ypos();
}

int DebugWindow::get_x(const int ptype) {

    return this->panel[ptype]->get_xpos();
}

void DebugWindow::close_panel(const int ptype) {

    this->panel[ptype]->destroy();
}
