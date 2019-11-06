/* String-Parse Implementation */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream>  
#include <iterator>

// Project Header
#include "stringparse.hpp"

using namespace std;

string get_file_name(string file) {

    int pos;

    if((pos = file.find(".")) == string::npos)
        return "";

    return file.substr(0, pos);
}

string del_comment(string line) {

    int pos;

    if((pos = line.find(";")) == string::npos)
        return line;

    return line.substr(0, pos);     
}

void trim(string *x) {

    x->erase(remove(x->begin(), x->end(), ' '), x->end());
    x->erase(remove(x->begin(), x->end(), '\t'), x->end());
}

int to_dec(string input) {

    int dec;
    istringstream ss(input);

    ss >> dec;

    if(ss.fail())
        return -1;

    return dec;
}

int hex_to_dec(string input) {

    unsigned int len = input.size();
    int dec = 0; int base = 1;

    int end = 0;

    if(input.substr(0, 2) == "0x")
        end += 2;

    for(int i = len - 1; i >= end; i--) {

        if(input[i] >= '0' && input[i] <= '9') {

            dec += (input[i] - '0') * base;
            base *= 16;
                        
        } else if(input[i] >= 'A' && input[i] <= 'F') {

            dec += (10 + input[i] - 'A') * base;
            base *= 16;
            
        } else {

            return -1;
        }
    }

    return dec;
}

bool file_exists(const string &file) {

    ifstream check_file(file);
    return check_file.good();
}