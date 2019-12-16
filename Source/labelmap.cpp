/* LabelMap Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <bits/stdc++.h> 

// Project Headers
#include "labelmap.hpp"
#include "stringmanip.hpp"

using namespace std;

namespace {

    int find_addr(string line) {

        int pos, addr; 
        bool neg = false;

        if((pos = line.find("+")) > 7) {

            pos = line.find("-");
            neg = true;
        }

        line = line.substr(pos + 1, line.size());
        pos = line.find(" ");

        line = line.substr(0, pos);
        addr = to_dec(line);

        return (neg == true) ? (addr * -1) : addr;
    }

    int label_exists(vector < tuple <int, string> > labels, int addr) {

        for(int i = 0; i < labels.size(); i++) {

            if(get <0> (labels[i]) == addr) {

                string label = get <1> (labels[i]);

                int pos = label.find(":");
                int lx = to_dec(label.substr(1, pos - 1));

                return lx;
            }
        }

        return -1;
    }
};

/* --- Public --- */

LabelMap::LabelMap(void) {

    const int size = 7;

    string instr[size] = { "rjmp", "brne", "breq", "brge", "brpl", "brlo", "rcall" };

    for(int i = 0; i < size; i++)
        this->imap[ instr[i] ] = true;

    this->size = 0;
}

int LabelMap::add(string line, int i) {

    int pos, addr, lx;

    if((pos = line.find(" ")) == string::npos)
        return -1;

    if(this->imap[ line.substr(0, pos) ] == false)
        return - 1;

    addr = (find_addr(line) + i + 1);

    if((lx = label_exists(this->labels, addr)) >= 0)
        return lx;

    tuple <int, string> t = make_tuple(addr, this->get_label());

    this->labels.push_back(t);
    this->size += 1;

    return (this->size - 1);
}

void LabelMap::sort_up(void) {

    sort(this->labels.begin(), this->labels.end());
}

int LabelMap::size_of(void) {

    return this->size;
}

tuple <int, string> LabelMap::get(int index) {

    tuple <int, string> next = make_tuple(-1, "");

    if(index < 0 || index > this->size - 1)
        return next;

    return this->labels[index];
}

/* --- Private --- */

string LabelMap::get_label(void) {

    string label = "L" + to_string(this->size) + ":";
    return label;
}