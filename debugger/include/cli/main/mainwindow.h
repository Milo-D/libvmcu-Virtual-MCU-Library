/* MainWindow Header */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define SELECT 0
#define QUIT 1
#define VALID 2

typedef struct _win_st WINDOW;
typedef struct _array array_t;

typedef struct _mainwindow {

    WINDOW *win;        										// MainWindow
    array_t *entry;     										// MainWindow entries

    int select;         										// file selector

    int height;         										// height
    int width;          										// width
    int py;             										// y-position
    int px;             										// x-position

} mainwindow_t;

extern struct _mainwindow* mwin_ctor(array_t *files);           // MainWindow Constructor
extern void mwin_dtor(struct _mainwindow *this);                // MainWindow Destructor

extern int mwin_read_key(struct _mainwindow *this);	            // reading user input
extern int mwin_get_choice(const struct _mainwindow *this);     // getting last position of selector

extern void mwin_update(const struct _mainwindow *this);        // refreshing MainWindow
extern void mwin_reinit(struct _mainwindow **this);             // reinitializing MainWindow (no constructor)
extern void mwin_destroy(struct _mainwindow *this);             // destroying MainWindow (no destructor)

#endif