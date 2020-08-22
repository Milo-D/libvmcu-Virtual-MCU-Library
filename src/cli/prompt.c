/* Prompt Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

// Project Headers
#include "cli/prompt.h"

struct _private {

    WINDOW *win;
};

/* Forward Declaration of static Functions */

static void prompt_init(struct _prompt *this, int h, int w, int y, int x);

/* --- Extern --- */

struct _prompt* prompt_ctor(int h, int w, int y, int x) {

    struct _prompt *prompt;

    if((prompt = malloc(sizeof(struct _prompt))) == NULL)
        return NULL;

    if((prompt->p = malloc(sizeof(struct _private))) == NULL) {

        free(prompt);
        return NULL;
    }

    init_pair(1, COLOR_RED, COLOR_BLACK);
    prompt_init(prompt, h, w, y, x);

    return prompt;
}

void prompt_dtor(struct _prompt *this) {

    delwin(this->p->win);

    free(this->p);
    free(this);
}

void prompt_read(const struct _prompt *this, char *buffer) {

    mvwprintw(this->p->win, 1, 1, ">>> ");
    wgetnstr(this->p->win, buffer, PLEN);

    wmove(this->p->win, 1, 5);
    wclrtoeol(this->p->win);

    prompt_update(this);
}

void prompt_write(const struct _prompt *this, const char *str) {

    /* autocompletion in progress */
}

void prompt_resize(struct _prompt *this, int h, int w, int y, int x) {

    delwin(this->p->win);
    prompt_init(this, h, w, y, x);
}

void prompt_update(const struct _prompt *this) {

    wborder(this->p->win, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(this->p->win);
}

/* --- Static --- */

static void prompt_init(struct _prompt *this, int h, int w, int y, int x) {

    this->p->win = newwin(h, w, y, x);
    wbkgd(this->p->win, COLOR_PAIR(1));

    curs_set(2);
    scrollok(this->p->win, true);
    prompt_update(this);
}