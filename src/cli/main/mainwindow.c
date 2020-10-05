/* MainWindow Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Headers
#include "cli/main/mainwindow.h"
#include "collections/array.h"

struct _private {

    WINDOW *win;        // MainWindow
    array_t *entry;     // MainWindow entries

    int select;         // file selector

    int height;         // height
    int width;          // width
    int py;             // y-position
    int px;             // x-position
};

/* Forward Declaration of Private MainWindow Functions */

static void init(struct _mainwindow *this);
static void key_up(const struct _mainwindow *this);
static void key_down(const struct _mainwindow *this);                                         

/* --- Public --- */

struct _mainwindow* mwin_ctor(array_t *files) {

    struct _mainwindow *mwin;

    if((mwin = malloc(sizeof(struct _mainwindow))) == NULL)
        return NULL;

    if((mwin->p = malloc(sizeof(struct _private))) == NULL) {

        free(mwin);
        return NULL;
    }

    const int n = files->top;
    mwin->p->entry = array_ctor(n, NULL, NULL);

    for(int i = 0; i < n; i++) {

        char *file = (char*) array_at(files, i);
        const size_t bytes = (strlen(file) + 1) * sizeof(char);

        array_push(mwin->p->entry, (void*) file, bytes);
    }

    init(mwin);
    return mwin;
}

void mwin_dtor(struct _mainwindow *this) {

    mwin_destroy(this);
    endwin();

    array_dtor(this->p->entry);
    free(this->p);
    free(this);
}

int mwin_read_key(const struct _mainwindow *this) {

    const int key = wgetch(this->p->win);
    int *i = &this->p->select; int press = VALID;

    char *item = (char*) array_at(this->p->entry, *i); 
    mvwprintw(this->p->win, *i + 1, 2, "%s", item);

    switch(key) {

        case KEY_UP: key_up(this); break;
        case KEY_DOWN: key_down(this); break;
        case 10: press = SELECT; break;
        case 'q': press = QUIT; break;

        default: /* ignoring invalid options */ break;
    }

    char *file = (char*) array_at(this->p->entry, *i);

    wattron(this->p->win, A_STANDOUT);
    mvwprintw(this->p->win, *i + 1, 2, "%s", file);
    wattroff(this->p->win, A_STANDOUT); 

    mwin_update(this);

    return press;    
}

int mwin_get_choice(const struct _mainwindow *this) {

    return this->p->select;
}

void mwin_update(const struct _mainwindow *this) {

    wborder(this->p->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->p->win);
}

void mwin_reinit(struct _mainwindow **this) {

    init(*this);
}

void mwin_destroy(struct _mainwindow *this) {

    delwin(this->p->win);
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

    this->p->select = 0;

    this->p->win = newwin(6, 42, y, x);
    wborder(this->p->win, 0, 0, 0, 0, 0, 0, 0, 0);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    wbkgd(this->p->win, COLOR_PAIR(1));
    scrollok(this->p->win, true);
    keypad(this->p->win, true);
    nl();

    for(int i = 0; i < this->p->entry->size; i++) {

        if(i == 0)
            wattron(this->p->win, A_STANDOUT);
        else
            wattroff(this->p->win, A_STANDOUT);

        char *file = (char*) array_at(this->p->entry, i);
        mvwprintw(this->p->win, i + 1, 2, "%s", file);
    }

    this->p->height = 6;
    this->p->width = 42;
    this->p->py = y;
    this->p->px = x;

    mwin_update(this);
}

static void key_up(const struct _mainwindow *this) {

    this->p->select -= 1;

    if(this->p->select < 0)
        this->p->select = this->p->entry->size - 1;
}

static void key_down(const struct _mainwindow *this) {

    this->p->select += 1;

    if(this->p->select >= this->p->entry->size)
        this->p->select = 0;
}
