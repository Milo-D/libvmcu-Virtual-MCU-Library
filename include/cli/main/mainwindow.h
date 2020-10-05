/* MainWindow Header */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define SELECT 0
#define QUIT 1
#define VALID 2

typedef struct _array array_t;

struct _private;

typedef struct _mainwindow {

    struct _private *p;

} mainwindow_t;

extern struct _mainwindow* mwin_ctor(array_t *files);           // MainWindow Constructor
extern void mwin_dtor(struct _mainwindow *this);                // MainWindow Destructor

extern int mwin_read_key(const struct _mainwindow *this);       // reading user input
extern int mwin_get_choice(const struct _mainwindow *this);     // getting last position of selector

extern void mwin_update(const struct _mainwindow *this);        // refreshing MainWindow
extern void mwin_reinit(struct _mainwindow **this);             // reinitializing MainWindow (no constructor)
extern void mwin_destroy(struct _mainwindow *this);             // destroying MainWindow (no destructor)

#endif