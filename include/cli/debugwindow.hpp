/* DebugWindow Header */

#ifndef DEBUGWINDOW_HPP
#define DEBUGWINDOW_HPP

#define GPR_PANEL 0
#define SREG_PANEL 1
#define CODE_PANEL 2
#define DATA_PANEL 3
#define EEPROM_PANEL 4
#define OUTPUT_PANEL 5
#define SIDE_PANEL 6

#define N_PANEL 7

class Panel;
class Prompt;

class DebugWindow {

public:
    DebugWindow(void);                                                          // DebugWindow Constructor
    ~DebugWindow(void);                                                         // DebugWindow Destructor

    std::string read_prompt(void);                                              // reading from prompt

    void write(int ptype, const std::string& data, int color);                  // writing to specific panel
    void clear(void);                                                           // clear non-static panels

    void update(int ptype);                                                     // refresh a specific panel
    void update_all(void);                                                      // refresh all panels

    int get_height(int ptype);                                                  // getting height of specific panel
    int get_width(int ptype);                                                   // getting width of specific panel
    int get_y(int ptype);                                                       // getting y position of specific panel
    int get_x(int ptype);                                                       // getting x position of specific panel

    void close_panel(int ptype);                                                // close a specific panel

private:
    Panel **panel;                                                              // Panels of DebugWindow
    Prompt *prompt;                                                             // Prompt of DebugWindow
};

#endif