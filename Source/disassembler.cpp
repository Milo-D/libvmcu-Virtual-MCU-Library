/* Implementation of Disassembler */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <tuple>

// Project Headers
#include "disassembler.hpp"
#include "ehandling.hpp"
#include "decoder.hpp"
#include "mnemonics.hpp"
#include "labelmap.hpp"

using namespace std;

namespace {

    string replace_addr(string line, int lx) {

        string mnemonic, trash, comment;
        int pos = line.find(" ");

        mnemonic = line.substr(0, pos);
        trash = line.substr(pos + 1, line.size());
        comment = trash.substr(trash.find(" ") + 1, line.size());

        return (mnemonic + " L" + to_string(lx) + " " + comment);
    }

    vector < tuple <string, int> > add_labels(vector < tuple <string, int> > source) {

        LabelMap lmap; int offs = 0;

        for(int i = 0; i < source.size(); i++) {

            string line = get <0> (source[i]);
            int lx = lmap.add(line, i);

            if(lx >= 0)
                get <0> (source[i]) = replace_addr(line, lx);
        }

        lmap.sort_up();

        for(int i = 0; i < lmap.size_of(); i++) {

            tuple <int, string> t = lmap.get(i);

            int t_addr = get <0> (t);
            string t_label = get <1> (t);

            tuple <string, int> blank = make_tuple("", -1);
            tuple <string, int> label = make_tuple(t_label, -1);

            if(t_addr > 0) {

                source.insert(source.begin() + t_addr + offs, blank);
                offs += 1;
            }
            
            source.insert(source.begin() + t_addr + offs, label);
            offs += 1;
        }

        return source;
    }
};

vector < tuple <string, int> > create_source(string hex_file) {

    vector < tuple <string, int> > source;

    vector <struct plain> dump;
    decode_file(hex_file).swap(dump);

    for(int i = 0; i < dump.size(); i++) {

        struct plain p = dump[i];

        string current = (*mnemonics[p.key])(p.opcode);
        tuple <string, int> t = make_tuple(current, p.addr);

        source.push_back(t);
    }

    if(source.size() == 0)
        return source;

    add_labels(source).swap(source);

    return source;
}