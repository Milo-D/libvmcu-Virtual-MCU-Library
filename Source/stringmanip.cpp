/* String-Manipulators Implementation */

// C++ Header
#include <iostream>
#include <iomanip>
#include <string>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <bits/stdc++.h> 

// Project Header
#include "stringmanip.hpp"

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

string fill(int len) {

    string space = "";

    for(int i = 0; i < 24 - len; i++)
        space += " ";

    return space;
}

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

string get_hex(int8_t input) {

    char hex[3] = { '0', '0' }; 
    int dec = 0; int i = 0;

    for(int i = 0; i < 8; i++)
        dec |= ((1 << i) & input);

    while(dec != 0) {

        int res = (dec % 16);

        if(res < 10)
            hex[i] = (res + 48);
        else
            hex[i] = (res + 87);

        i += 1;
        dec /= 16;
    }

    char temp = hex[0];

    hex[0] = hex[1];
    hex[1] = temp;
    hex[2] = '\0';

    return string(hex);
}

string mix_memory(vector <string> first, vector <string> second) {

    stringstream stream;

    for(int i = 0; i < first.size(); i++) {

        stream << first[i] << right << setfill(' ');

        if(i == 0)
            stream << setw(33);
        else
            stream << setw(46);

        stream << second[i] << endl;

        if(i == 0)
            stream << endl;
    }

    return stream.str();
}