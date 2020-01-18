/* Prompt Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <ncurses.h>

// Project Headers
#include "cli/prompt.hpp"

using namespace std;

Prompt::Prompt(int height, int width, int y, int x) {

    this->init(height, width, y, x);
}

Prompt::~Prompt(void) {

    this->destroy();
}

void Prompt::init(int height, int width, int y, int x) {

    this->win = newwin(3, width, y, x);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    wbkgd(this->win, COLOR_PAIR(1));

    curs_set(2);
    scrollok(this->win, true);

    this->update();
}

string Prompt::read(void) {

    char buffer[64];

    mvwprintw(this->win, 1, 1, ">>> ");
    wgetnstr(this->win, buffer, 64);

    wmove(this->win, 1, 5);
    wclrtoeol(this->win);

    this->update();

    return (string) buffer;
}

void Prompt::write(const string& data) {

    /* in progress */
}

void Prompt::update(void) {

    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->win);
}

void Prompt::destroy(void) {

    delwin(this->win);
}
