/* Panel Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Headers
#include "cli/panel.h"
#include "cli/highlighter.h"

struct _private {

    WINDOW *win;
    highlighter_t *hl;

    int cursor;
    int range;

    int height;
    int width;
    int y;
    int x;
};

struct _panel* panel_ctor(int h, int w, int y, int x, int cs, int cr) {

    struct _panel *panel; 

    if((panel = malloc(sizeof(struct _panel))) == NULL)
        return NULL;

    if((panel->p = malloc(sizeof(struct _private))) == NULL) {

        free(panel);
        return NULL;
    }

    panel->p->hl = hl_ctor();

    panel_init(panel, h, w, y, x, cs, cr);
    wmove(panel->p->win, 1, 0);

    panel->p->cursor = cs;
    panel->p->range = cr;

    panel->p->height = h;
    panel->p->width = w;
    panel->p->y = y;
    panel->p->x = x;

    return panel;
}

void panel_dtor(struct _panel *this) {

    hl_dtor(this->p->hl);
    panel_destroy(this);

    free(this->p);
    free(this);
}

void panel_init(struct _panel *this, int h, int w, int y, int x, int cs, int cr) {

    this->p->win = newwin(h, w, y, x);
    wborder(this->p->win, 0, 0, 0, 0, 0, 0, 0, 0);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    wbkgd(this->p->win, COLOR_PAIR(1));
    scrollok(this->p->win, true);
    nl();

    panel_update(this);
}

void panel_add(struct _panel *this, const char *str, const COLOR col) {

    int y, x;
    getyx(this->p->win, y, x);

    wattron(this->p->win, COLOR_PAIR(col + 1));
    mvwprintw(this->p->win, y, x + (x == 0), "%s", str);
    wattroff(this->p->win, COLOR_PAIR(col + 1));
}


void panel_write(struct _panel *this, const char *str, const COLOR col) {

    panel_add(this, str, col);
    panel_update(this);
}

void panel_highlight(struct _panel *this, const char *str) {

    hl_paint(this->p->hl, this, str);
}

void panel_update(struct _panel *this) {

    wborder(this->p->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->p->win);
}

void panel_clear(struct _panel *this) {

    werase(this->p->win);
    wmove(this->p->win, 1, 0);

    panel_update(this);
}

void panel_mv_curs(struct _panel *this, const int offs) {

    if((this->p->cursor + offs) < 0)
        return;

    if((this->p->cursor + offs) >= this->p->range)
        return;

    this->p->cursor += offs;    
}

void panel_set_curs(struct _panel *this, const int at) {

    if(at < 0 || at >= this->p->range)
        return;

    this->p->cursor = at;
}

int panel_get_curs(struct _panel *this) {

    return this->p->cursor;
}

int panel_height(struct _panel *this) {

    return this->p->height;
}

int panel_width(struct _panel *this) {

    return this->p->width;
}

int panel_y(struct _panel *this) {

    return this->p->y;
}

int panel_x(struct _panel *this) {

    return this->p->x;
}

void panel_destroy(struct _panel *this) {

    delwin(this->p->win);
}