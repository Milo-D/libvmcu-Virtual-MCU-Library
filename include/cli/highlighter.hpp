/* Syntax Highlighter Class */

#ifndef HIGHLIGHTER_HPP
#define HIGHLIGHTER_HPP

// C++ Headers
#include <map>

class Panel;

class Highlighter {

public:
    Highlighter(void);

    void paint(Panel *panel, const std::string & line);

private:
    std::map <std::string, bool> flow;
    std::map <std::string, bool> misc;
    std::map <std::string, bool> logic;
    std::map <std::string, bool> access;
    std::map <std::string, bool> stackop;
    std::map <std::string, bool> arithmetic;

    void paint_instruction(Panel *panel, const std::string & part);
    void paint_operand(Panel *panel, const std::string & part);

    void init_flow(void);
    void init_misc(void);
    void init_logic(void);
    void init_access(void);
    void init_stackop(void);
    void init_arithmetic(void);
};

#endif