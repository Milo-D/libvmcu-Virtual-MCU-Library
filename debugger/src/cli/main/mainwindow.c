/* MainWindow Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Headers (debugger)
#include "debugger/include/cli/main/mainwindow.h"

// Project Headers (shared)
#include "shared/include/collections/array.h"

/* Forward Declaration of Private MainWindow Functions */

static void init(struct _mainwindow *this);
static void key_up(struct _mainwindow *this);
static void key_down(struct _mainwindow *this);                                         

/* --- Public --- */

struct _mainwindow* mwin_ctor(array_t *files) {

    struct _mainwindow *mwin;

    if((mwin = malloc(sizeof(struct _mainwindow))) == NULL)
        return NULL;

    const int n = files->top;
    mwin->entry = array_ctor(n, NULL, NULL);

    for(int i = 0; i < n; i++) {

        char *file = (char*) array_at(files, i);
        const size_t bytes = (strlen(file) + 1) * sizeof(char);

        array_push(mwin->entry, (void*) file, bytes);
    }

    init(mwin);
    return mwin;
}

void mwin_dtor(struct _mainwindow *this) {

    mwin_destroy(this);
    endwin();

    array_dtor(this->entry);
    free(this);
}

int mwin_read_key(struct _mainwindow *this) {

    const int key = wgetch(this->win);
    int *i = &this->select; int press = VALID;

    char *item = (char*) array_at(this->entry, *i); 
    mvwprintw(this->win, *i + 1, 2, "%s", item);

    switch(key) {

        case KEY_UP: key_up(this); break;
        case KEY_DOWN: key_down(this); break;
        case 10: press = SELECT; break;
        case 'q': press = QUIT; break;

        default: /* ignoring invalid options */ break;
    }

    char *file = (char*) array_at(this->entry, *i);

    wattron(this->win, A_STANDOUT);
    mvwprintw(this->win, *i + 1, 2, "%s", file);
    wattroff(this->win, A_STANDOUT); 

    mwin_update(this);

    return press;    
}

int mwin_get_choice(const struct _mainwindow *this) {

    return this->select;
}

void mwin_update(const struct _mainwindow *this) {

    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->win);
}

void mwin_reinit(struct _mainwindow **this) {

    init(*this);
}

void mwin_destroy(struct _mainwindow *this) {

    delwin(this->win);
}

/* --- Private --- */

static void init(struct _mainwindow *this) {

    initscr();
    cbreak();
    refresh();
    noecho();
    curs_set(0);
    start_color();

    int ly, lx;
    getmaxyx(stdscr, ly, lx);
    
    const int y = ((ly / 2) - 3);
    const int x = ((lx / 2) - 22);

    this->select = 0;

    this->win = newwin(6, 42, y, x);
    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    wbkgd(this->win, COLOR_PAIR(1));
    scrollok(this->win, true);
    keypad(this->win, true);
    nl();

    for(int i = 0; i < this->entry->size; i++) {

        if(i == 0)
            wattron(this->win, A_STANDOUT);
        else
            wattroff(this->win, A_STANDOUT);

        char *file = (char*) array_at(this->entry, i);
        mvwprintw(this->win, i + 1, 2, "%s", file);
    }

    this->height = 6;
    this->width = 42;
    this->py = y;
    this->px = x;

    mwin_update(this);
}

static void key_up(struct _mainwindow *this) {

    this->select -= 1;

    if(this->select < 0)
        this->select = this->entry->size - 1;
}

static void key_down(struct _mainwindow *this) {

    this->select += 1;

    if(this->select >= this->entry->size)
        this->select = 0;
}
