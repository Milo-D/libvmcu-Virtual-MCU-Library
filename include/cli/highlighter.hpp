/* Syntax Highlighter Class */

#ifndef HIGHLIGHTER_HPP
#define HIGHLIGHTER_HPP

// C++ Headers
#include <map>

class Panel;

class Highlighter {

public:
    Highlighter(void);                                                          // Constructor

    void paint(Panel *panel, const std::string & line);                         // paint 'line'

private:
    std::map <std::string, bool> flow;                                          // contains flow control assembly mnemonics
    std::map <std::string, bool> misc;                                          // contains misc assembly mnemonics
    std::map <std::string, bool> logic;                                         // contains logical assembly mnemonics
    std::map <std::string, bool> access;                                        // contains data access assembly mnemonics
    std::map <std::string, bool> stackop;                                       // contains stack based assembly mnemonics
    std::map <std::string, bool> arithmetic;                                    // contains arithmetical assembly mnemonics

    void paint_instruction(Panel *panel, const std::string & part);             // paint instruction part of line
    void paint_operand(Panel *panel, const std::string & part);                 // paint operand part of line

    void init_flow(void);                                                       // init flow map, used by Constructor
    void init_misc(void);                                                       // init misc map, used by Constructor
    void init_logic(void);                                                      // init logic map, used by Constructor
    void init_access(void);                                                     // init access map, used by Constructor
    void init_stackop(void);                                                    // init stackop map, used by Constructor
    void init_arithmetic(void);                                                 // init arithmetic map, used by Constructor
};

#endif