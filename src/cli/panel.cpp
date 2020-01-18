/* Panel Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <ncurses.h>

// Project Headers
#include "cli/panel.hpp"

using namespace std;

Panel::Panel(int height, int width, int y, int x) {

    this->init(height, width, y, x);
    wmove(this->win, 1, 0);

    this->cursor = 0;

    this->height = height;
    this->width = width;
    this->py = y;
    this->px = x;
}

Panel::~Panel(void) {

    this->destroy();
}

void Panel::init(int height, int width, int y, int x) {

    this->win = newwin(height, width, y, x);
    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

    wbkgd(this->win, COLOR_PAIR(1));
    scrollok(this->win, true);
    nl();

    this->update();
}

void Panel::write(const string& data, int color) {

    int y, x;
    getyx(this->win, y, x);

    wattron(this->win, COLOR_PAIR(color));
    mvwprintw(this->win, y, x + (x == 0), "%s", data.c_str());
    wattroff(this->win, COLOR_PAIR(color));

    this->update();
}

void Panel::update(void) {

    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->win);
}

void Panel::clear(void) {

    werase(this->win);
    wmove(this->win, 1, 0);
    
    this->update();
}

int Panel::get_height(void) {

    return this->height;
}

int Panel::get_width(void) {

    return this->width;
}

int Panel::get_ypos(void) {

    return this->py;
}

int Panel::get_xpos(void) {

    return this->px;
}

void Panel::destroy(void) {

    this->cursor = 0;
    delwin(this->win);
}