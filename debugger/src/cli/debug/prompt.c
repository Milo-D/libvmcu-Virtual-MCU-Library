/* Prompt Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

// Project Headers (debugger)
#include "debugger/include/cli/debug/prompt.h"

/* Forward Declaration of static Functions */

static void prompt_default_properties(win_properties_t *prop);
static void prompt_init(struct _prompt *this, win_properties_t *prop);

/* --- Extern --- */

struct _prompt* prompt_ctor(void) {

    struct _prompt *prompt;

    if((prompt = malloc(sizeof(struct _prompt))) == NULL)
        return NULL;

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    
    win_properties_t prop;
    
    prompt_default_properties(&prop);
    prompt_init(prompt, &prop);

    return prompt;
}

void prompt_dtor(struct _prompt *this) {

    delwin(this->win);
    free(this);
}

void prompt_read(const struct _prompt *this, char *buffer) {

    mvwprintw(this->win, 1, 1, ">>> ");
    wgetnstr(this->win, buffer, PLEN);

    wmove(this->win, 1, 5);
    wclrtoeol(this->win);

    prompt_update(this);
}

void prompt_write(const struct _prompt *this, const char *str) {

    /* autocompletion in progress */
}

void prompt_resize(struct _prompt *this) {

    delwin(this->win);
    
    win_properties_t prop;
    prompt_default_properties(&prop);
    prompt_init(this, &prop);
}

void prompt_update(const struct _prompt *this) {

    wborder(this->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->win);
}

/* --- Static --- */

static void prompt_default_properties(win_properties_t *prop) {
    
    int scr_y, scr_x;
    getmaxyx(stdscr, scr_y, scr_x);

    const int gy = (scr_y - 40);

    *prop = (win_properties_t) {
        
        .height = 3,
        .width  = scr_x,
        .y      = 38 + gy - 1,
        .x      = 0
    };
}

static void prompt_init(struct _prompt *this, win_properties_t *prop) {

    const int h = prop->height;
    const int w = prop->width;
    const int y = prop->y;
    const int x = prop->x;

    this->win = newwin(h, w, y, x);
    wbkgd(this->win, COLOR_PAIR(1));

    curs_set(2);
    scrollok(this->win, true);
    prompt_update(this);
}
