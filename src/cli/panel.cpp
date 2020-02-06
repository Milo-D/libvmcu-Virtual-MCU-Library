/* Panel Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <ncurses.h>

// Project Headers
#include "cli/panel.hpp"
#include "cli/highlighter.hpp"

using namespace std;

Panel::Panel(int h, int w, int y, int x, int cs, int cr) {

    this->init(h, w, y, x);
    wmove(this->win, 1, 0);

    this->highlighter = new Highlighter;

    this->cursor = cs;
    this->range = cr;

    this->height = h;
    this->width = w;
    this->py = y;
    this->px = x;
}

Panel::~Panel(void) {

    this->destroy();
}

void Panel::init(int h, int w, int y, int x) {

    this->win = newwin(h, w, y, x);
    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    wbkgd(this->win, COLOR_PAIR(1));
    scrollok(this->win, true);
    nl();

    this->update();
}

void Panel::add(const string & data, const int color) {

    int y, x;
    getyx(this->win, y, x);

    wattron(this->win, COLOR_PAIR(color));
    mvwprintw(this->win, y, x + (x == 0), "%s", data.c_str());
    wattroff(this->win, COLOR_PAIR(color));
}

void Panel::write(const string & data, const int color) {

    this->add(data, color);
    this->update();
}

void Panel::highlight(const string & data) {

    this->highlighter->paint(this, data);
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

void Panel::move_cursor(int offs) {

    if((this->cursor + offs) < 0)
        return;

    if((this->cursor + offs) >= this->range)
        return;

    this->cursor += offs;
}

void Panel::set_cursor(int at) {

    if(at < 0 || at >= this->range)
        return;

    this->cursor = at;
}

int Panel::get_cursor(void) {

    return this->cursor;
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

    delwin(this->win);
}