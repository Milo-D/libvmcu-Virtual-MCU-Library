/* Syntax Highlighter Implemetation */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>
#include <ncurses.h>

// Project Headers
#include "cli/highlighter.hpp"
#include "cli/debugwindow.hpp"
#include "cli/panel.hpp"
#include "misc/stringmanip.hpp"
#include "misc/mnemstr.hpp"

using namespace std;

namespace {

    string get_code(const string & line) {

        size_t pos;
        pos = line.find("  ");

        if(pos == string::npos)
            return line;

        return line.substr(0, pos);
    }

    string get_comment(const string & line) {

        size_t pos;
        pos = line.find("  ");

        if(pos == string::npos)
            return line;

        return line.substr(pos + 1, line.size());
    }
};

/* --- Public --- */

Highlighter::Highlighter(void) {

    this->init_flow();
    this->init_misc();
    this->init_logic();
    this->init_access();
    this->init_stackop();
    this->init_arithmetic();
}

void Highlighter::paint(Panel *panel, const string & line) {

    const string code = get_code(line);
    const string comment = get_comment(line);

    vector <string> part;
    split(code, part).swap(part);

    if(part[0].find(":") != string::npos) {

        panel->add(line, C);
        return;
    }

    for(int i = 0; i < part.size(); i++) {

        switch(i) {

            case 0: this->paint_instruction(panel, part[0]); break;
            case 1: this->paint_operand(panel, part[1]);     break;
            case 2: this->paint_operand(panel, part[2]);     break;

            default: panel->add(line, DEF); break;
        }
    }

    panel->add(comment, R);
}

/* --- Private --- */

void Highlighter::paint_instruction(Panel *panel, const std::string & part) {

    map <string, bool> :: iterator it;

    if((it = this->flow.find(part)) != this->flow.end()) {

        panel->add(part + " ", G);
        return;
    }

    if((it = this->misc.find(part)) != this->misc.end()) {

        panel->add(part + " ", DEF);
        return;
    }

    if((it = this->logic.find(part)) != this->logic.end()) {

        panel->add(part + " ", M);
        return;
    }

    if((it = this->access.find(part)) != this->access.end()) {

        panel->add(part + " ", DEF);
        return;
    }

    if((it = this->stackop.find(part)) != this->stackop.end()) {

        panel->add(part + " ", B);
        return;
    }

    if((it = this->arithmetic.find(part)) != this->arithmetic.end()) {

        panel->add(part + " ", R);
        return;
    }

    panel->add(part + " ", DEF);
}

void Highlighter::paint_operand(Panel *panel, const std::string & part) {

    if((hex_to_dec(part) < 0) && (to_dec(part) < 0)) {

        panel->add(part + " ", C);
        return;
    }

    panel->add(part + " ", B);
}

void Highlighter::init_flow(void) {

    for(int i = 0; i < N_FLOW; i++)
        this->flow[ flow_mnem[i] ] = true;
}

void Highlighter::init_misc(void) {

    for(int i = 0; i < N_MISC; i++)
        this->misc[ misc_mnem[i] ] = true;
}

void Highlighter::init_logic(void) {

    for(int i = 0; i < N_LOGIC; i++)
        this->logic[ logic_mnem[i] ] = true;
}

void Highlighter::init_access(void) {

    for(int i = 0; i < N_ACCESS; i++)
        this->access[ access_mnem[i] ] = true;
}

void Highlighter::init_stackop(void) {

    for(int i = 0; i < N_STACKOP; i++)
        this->stackop[ stackop_mnem[i] ] = true;
}

void Highlighter::init_arithmetic(void) {

    for(int i = 0; i < N_ARITHMETIC; i++)
        this->arithmetic[ arithmetic_mnem[i] ] = true;
}