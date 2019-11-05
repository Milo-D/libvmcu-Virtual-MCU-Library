/* Implementation of User Input Parser*/

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Project Headers
#include "uparse.hpp"
#include "ehandling.hpp"
#include "stringparse.hpp"

using namespace std;

namespace {

    vector <string> split(string s, vector <string> res) {

        int pos = s.find(" ");

        if(pos == string::npos) {

            res.push_back(s);
			
        } else {

            res.push_back(s.substr(0, pos));
            s = s.substr(pos + 1, s.size());

            split(s, res).swap(res);
        }

        return res;
    }

    bool is_num(const string &token) {

        string::const_iterator it = token.begin();

        while(it != token.end() && isdigit(*it))
            ++it;

        return !token.empty() && it == token.end();
	}

	void is_asm(string file) {

        unsigned int pos;

        if((pos = file.find(".asm")) == string::npos)
            print_status("Could not parse file.", true);

        if(pos + 4 != file.size())
            print_status("Could not parse file.", true);

        if(pos == 0)
            print_status("Invalid file name.", true);
    }
	
};

vector <string> parse_main_in(int argsz, char **argls) {

    vector <string> arguments;

    print_status("Parsing files...", false);

    if(argsz < 2)
        print_status("Missing arguments.", true);

    for(int i = 1; i < argsz; i++) {

        string current = (string) argls[i];

        if(file_exists(current) == false)
            print_status("Specified file does not exist.", true);

        is_asm(current);
        arguments.push_back(current);
    }

    print_status("Done!", false);

    return arguments;
}

vector <string> parse_table_in(string cmd) {

    vector <string> cmd_split;
    split(cmd, cmd_split).swap(cmd_split);

    unsigned int cmd_size = cmd_split.size();

    if(cmd_size == 1) {

        if(cmd_split[0] != "exit" && cmd_split[0] != "e")
            cmd_split.clear();

        return cmd_split;
    }

    if(cmd_size != 2) {

        cmd_split.clear();
        return cmd_split;
    }

    if(cmd_split[0] != "break" && cmd_split[0] != "unbreak") {

        cmd_split.clear();
        return cmd_split;
    }

    return cmd_split;
}

