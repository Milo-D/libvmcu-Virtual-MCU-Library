/* CodeGenerator Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <tuple>

// Project Headers
#include "codegen.hpp"
#include "ehandling.hpp"
#include "decoder.hpp"
#include "mnemonics.hpp"

using namespace std;

namespace {

    vector < tuple <string, int> > replace_addr(vector < tuple <string, int> > source) {

        /* in progress */
        return source;
    }

    vector < tuple <string, int> > add_labels(vector < tuple <string, int> > source) {

        int label_index = 1;

        source.insert(source.begin(), make_tuple("L0:", -1));

        for(int i = 0; i < source.size() - 1; i++) {

            string line = get <0> (source[i]);

            if(line.find("rjmp") == string::npos)
                continue;

            string label = "L" + to_string(label_index) + ":";

            tuple <string, int> blank_line = make_tuple("", -1);
            tuple <string, int> label_line = make_tuple(label, -1);

            source.insert(source.begin() + i + 1, blank_line);
            source.insert(source.begin() + i + 2, label_line);

            label_index += 1;
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

    add_labels(source).swap(source);

    return source;
}