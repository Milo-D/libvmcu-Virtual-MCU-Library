/* Panel Header */

#ifndef PANEL_HPP
#define PANEL_HPP

class Panel {

public:
    Panel(int height, int width, int y, int x);         // Panel Constructor
    ~Panel(void);                                       // Panel Destructor

    void init(int height, int width, int y, int x);     // Panel Initialization

    void write(const std::string& data, int color);     // write 'data' to Panel
    void update(void);                                  // refresh Panel
    void clear(void);                                   // clearing content of Panel

    int get_height(void);                               // get Panel height
    int get_width(void);                                // get Panel width
    int get_ypos(void);                                 // get Panel's y-Position
    int get_xpos(void);                                 // get Panel's x-Position

    void destroy(void);                                 // destroy Panel Window

private:
    WINDOW *win;                                        // Panel Window

    int cursor;                                         // Panel's Cursor to track lines

    int height;                                         // Panel Height
    int width;                                          // Panel Width
    int py;                                             // Panel y-Position
    int px;                                             // Panel x-Position
};

#endif