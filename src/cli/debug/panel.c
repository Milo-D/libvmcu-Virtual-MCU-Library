/* Panel Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Headers
#include "cli/debug/panel.h"
#include "cli/debug/highlighter.h"

/* Forward Declaration of static Functions */

static void panel_init(struct _panel *this, int h, int w, int y, int x);

/* --- Extern --- */

struct _panel* panel_ctor(int h, int w, int y, int x, int cs, int cr) {

    struct _panel *panel; 

    if((panel = malloc(sizeof(struct _panel))) == NULL)
        return NULL;

    panel->hl = hl_ctor();

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    panel_init(panel, h, w, y, x);

    panel->cursor = cs;
    panel->range = cr;

    return panel;
}

void panel_dtor(struct _panel *this) {

    delwin(this->win);
    hl_dtor(this->hl);
    
    free(this);
}

void panel_add(struct _panel *this, const char *str, const COLOR col) {

    int y, x;
    getyx(this->win, y, x);

    wattron(this->win, COLOR_PAIR(col + 1));
    mvwprintw(this->win, y, x + (x == 0), "%s", str);
    wattroff(this->win, COLOR_PAIR(col + 1));
}

void panel_write(struct _panel *this, const char *str, const COLOR col) {

    panel_add(this, str, col);
    panel_update(this);
}

void panel_highlight(struct _panel *this, const char *str) {

    hl_paint(this->hl, this, str);
}

void panel_resize(struct _panel *this, int h, int w, int y, int x) {

    delwin(this->win);
    panel_init(this, h, w, y, x);
}

void panel_update(struct _panel *this) {

    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->win);
}

void panel_clear(struct _panel *this) {

    werase(this->win);
    wmove(this->win, 1, 0);

    panel_update(this);
}

void panel_mv_curs(struct _panel *this, const int offs) {

    if((this->cursor + offs) < 0)
        return;

    if((this->cursor + offs) >= this->range)
        return;

    this->cursor += offs;    
}

void panel_set_curs(struct _panel *this, const int at) {

    if(at < 0 || at >= this->range)
        return;

    this->cursor = at;
}

int panel_get_curs(struct _panel *this) {

    return this->cursor;
}

/* --- Static --- */

static void panel_init(struct _panel *this, int h, int w, int y, int x) {

    this->win = newwin(h, w, y, x);
    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);

    wbkgd(this->win, COLOR_PAIR(1));
    scrollok(this->win, true);
    nl();

    panel_update(this);
    wmove(this->win, 1, 0);

    this->height = h;
    this->width = w;
    this->y = y;
    this->x = x;
}