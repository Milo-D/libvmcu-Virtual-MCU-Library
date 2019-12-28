/* File Manipulation Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "misc/filemanip.hpp"

using namespace std;

bool file_exists(const string &file) {

    ifstream check_file(file);
    return check_file.good();
}

bool is_hex_file(string file) {

    unsigned int pos;

    if((pos = file.find(".hex")) == string::npos)
        return false;

    if(pos + 4 != file.size())
        return false;

    if(pos == 0)
        return false;

    return true;
}

bool check_files(int argc, char **argv) {

    for(int i = 1; i < argc; i++) {

        string current = (string) argv[i];

        if(file_exists(current) == false)
            return false;

        if(is_hex_file(current) == false)
            return false;
    }

    return true;
}

vector <string> get_files(int argc, char **argv) {

    vector <string> files;

    for(int i = 1; i < argc; i++)
        files.push_back( (string) argv[i] );

    return files;
}