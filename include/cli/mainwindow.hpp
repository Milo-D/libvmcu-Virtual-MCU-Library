/* MainWindow Header */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define SELECT 0
#define QUIT 1
#define VALID 2

typedef struct _win_st WINDOW;

class MainWindow {

public:
    MainWindow(std::vector <std::string>& files);               // MainWindow Constructor
    ~MainWindow(void);                                          // MainWindow Destructor

    int read_key(void);                                         // reading user action
    int get_choice(void);                                       // get MainWindow selector

    void update(void);                                          // update MainWindow
    void re_init(void);
    void destroy(void);                                         // destroy MainWindow

private:
    WINDOW *win;                                                // MainWindow

    int selector;                                               // file selector
    std::vector <std::string> entry;                            // menu entries

    int height;
    int width;
    int py;
    int px;

    void init(void);             // inititialize MainWindow
    void mv_up(void);                                           // move one entry up
    void mv_down(void);                                         // move one entry down     
};

#endif