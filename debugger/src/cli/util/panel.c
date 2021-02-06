/* Panel Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Project Headers
#include "cli/util/panel.h"
#include "cli/util/highlighter.h"

/* Forward Declaration of static Functions */

static void panel_init(struct _panel *this, properties_t *prop);

/* --- Extern --- */

void panel_ctor(struct _panel *this, properties_t *prop) {

    this->hl = hl_ctor();

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    panel_init(this, prop);

    this->page = prop->page_init;
    this->range = prop->page_range;
}

void panel_dtor(struct _panel *this) {

    delwin(this->win);
    hl_dtor(this->hl);
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

void panel_resize(struct _panel *this, properties_t *prop) {

    delwin(this->win);
    panel_init(this, prop);
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

void panel_change_page(struct _panel *this, const int offs) {

    if((this->page + offs) < 0)
        return;

    if((this->page + offs) >= this->range)
        return;

    this->page += offs;    
}

void panel_set_page(struct _panel *this, const int at) {

    if(at < 0 || at >= this->range)
        return;

    this->page = at;
}

int panel_get_page(struct _panel *this) {

    return this->page;
}

/* --- Static --- */

static void panel_init(struct _panel *this, properties_t *prop) {

    const int h = prop->window.height;
    const int w = prop->window.width;
    const int y = prop->window.y;
    const int x = prop->window.x;

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
