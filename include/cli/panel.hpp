/* Panel Header */

#ifndef PANEL_HPP
#define PANEL_HPP

class Panel {

public:
    Panel(int h, int w, int y, int x, int cs, int cr);                              // Panel Constructor
    ~Panel(void);                                                                   // Panel Destructor

    void init(int h, int w, int y, int x);                                          // Panel Initialization

    void add(const std::string & data, const int color);                            // add string to Panel without updating
    void write(const std::string & data, const int color);                          // add string to Panel and update
    void update(void);                                                              // refresh Panel
    void clear(void);                                                               // clearing content of Panel

    void move_cursor(int offs);                                                     // moving Panel cursor
    void set_cursor(int at);                                                        // setting Panel cursor
    int get_cursor(void);                                                           // get Panel cursor

    int get_height(void);                                                           // get Panel height
    int get_width(void);                                                            // get Panel width
    int get_ypos(void);                                                             // get Panel's y-Position
    int get_xpos(void);                                                             // get Panel's x-Position

    void destroy(void);                                                             // destroy Panel Window

private:
    WINDOW *win;                                                                    // Panel Window

    int cursor;                                                                     // Panel's State Cursor
    int range;                                                                      // Cursor Range

    int height;                                                                     // Panel Height
    int width;                                                                      // Panel Width
    int py;                                                                         // Panel y-Position
    int px;                                                                         // Panel x-Position
};

#endif