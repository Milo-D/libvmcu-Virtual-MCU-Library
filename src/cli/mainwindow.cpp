/* MainWindow Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ncurses.h>

// Project Headers
#include "cli/mainwindow.hpp"

using namespace std;

MainWindow::MainWindow(vector <string>& files) {

    files.swap(this->entry);
    this->init();
}

MainWindow::~MainWindow(void) {

    this->destroy();
    endwin();
}

int MainWindow::read_key(void) {

    int key = wgetch(this->win);
    int *i = &this->selector; int press = VALID;

    mvwprintw(this->win, *i + 1, 2, "%s", this->entry[*i].c_str());

    switch(key) {

        case KEY_UP: this->mv_up(); break;
        case KEY_DOWN: this->mv_down(); break;
        case 10: press = SELECT; break;
        case 'q': press = QUIT; break;

        default: /* ignoring */ break;
    }

    wattron(this->win, A_STANDOUT);
    mvwprintw(this->win, *i + 1, 2, "%s", this->entry[*i].c_str());
    wattroff(this->win, A_STANDOUT); 

    this->update();

    return press;
}

int MainWindow::get_choice(void) {

    return this->selector;
}

void MainWindow::update(void) {

    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->win);
}

void MainWindow::re_init(void) {

    this->init();
}

void MainWindow::destroy(void) {

    delwin(this->win);
}

/* --- Private --- */

void MainWindow::init(void) {

    initscr();
    cbreak();
    refresh();
    noecho();
    curs_set(0);
    start_color();

    int ly, lx;
    getmaxyx(stdscr, ly, lx);
    
    int py = ((ly / 2) - 3);
    int px = ((lx / 2) - 22);

    this->selector = 0;

    this->win = newwin(6, 42, py, px);
    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);

    init_pair(1, COLOR_RED, COLOR_BLACK);

    wbkgd(this->win, COLOR_PAIR(1));
    scrollok(this->win, true);
    keypad(this->win, true);
    nl();

    for(int i = 0; i < this->entry.size(); i++) {

        if(i == 0)
            wattron(this->win, A_STANDOUT);
        else
            wattroff(this->win, A_STANDOUT);

        mvwprintw(this->win, i + 1, 2, "%s", this->entry[i].c_str());
    }

    this->height = 6;
    this->width = 42;
    this->py = py;
    this->px = px;

    this->update();
}

void MainWindow::mv_up(void) {

    this->selector -= 1;

    if(this->selector < 0)
        this->selector = this->entry.size() - 1;
}

void MainWindow::mv_down(void) {

    this->selector += 1;

    if(this->selector >= this->entry.size())
        this->selector = 0;
}