/* JSon Writer for Headless Output */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>

// C Headers
#include <inttypes.h>

// Project Headers
#include "misc/jsonwriter.hpp"
#include "misc/stringmanip.hpp"
#include "system/mcu.hpp"
#include "system/sys.hpp"

#define CONST_TAB "    "

using namespace std;

namespace {

    string new_numerical_pair(string key, int value, bool last) {

        stringstream stream;

        stream << CONST_TAB << CONST_TAB;
        stream << "\"" << key << "\": " << value;

        if(last == false)
            stream << ",";

        stream << "\n";
        return stream.str();
    }

    string new_float_pair(string key, float value, bool last) {

        stringstream stream;

        stream << CONST_TAB << CONST_TAB;
        stream << "\"" << key << "\": " << value;

        if(last == false)
            stream << ",";

        stream << "\n";
        return stream.str();
    }

    string new_alpha_pair(string key, string value, bool last) {

        stringstream stream;

        stream << CONST_TAB << CONST_TAB;
        stream << "\"" << key << "\": \"" << value << "\"";

        if(last == false)
            stream << ",";

        stream << "\n";
        return stream.str();
    }

    string sys_prop_obj(void) {

        stringstream stream;

        stream << CONST_TAB;
        stream << "\"system_properties\": {\n\n";

        stream << new_alpha_pair("ARCH", SYSTEM, false);
        stream << new_numerical_pair("FLASH", (FLASH_SIZE * 2), false);
        stream << new_numerical_pair("SRAM", SRAM_SIZE, false);
        stream << new_numerical_pair("EEPROM", EEPROM_SIZE, true);

        stream << CONST_TAB;
        stream << "}, \n\n";

        return stream.str();
    }

    string simul_status_obj(Sys *sys) {

        stringstream stream;

        stream << CONST_TAB;
        stream << "\"simulation_status\": {\n\n";

        string sys_status = "running";

        if(sys->is_terminated() == true)
            sys_status = "terminated";

        stream << new_alpha_pair("elapsed_time", "...", false);
        stream << new_alpha_pair("status", sys_status, true);

        stream << CONST_TAB;
        stream << "}, \n\n";

        return stream.str();
    }

    string gpr_obj(Sys *sys) {

        stringstream stream;

        stream << CONST_TAB;
        stream << "\"general_purpose_registers\": {\n\n";

        vector < tuple <string, string> > element;

        for(int i = 0; i < GPR_SIZE; i++) {

            int8_t cell_data = sys->read_gpr(i);

            if(cell_data == 0x00)
                continue;

            string value = "0x" + get_hex(cell_data);
            string key = "R" + to_string(i);

            tuple <string, string> t = make_tuple(key, value);
            element.push_back(t);
        }

        int size = element.size();

        for(int i = 0; i < size; i++) {

            string key = get <0> (element[i]);
            string value = get <1> (element[i]);

            bool last = (i == (size - 1));
            stream << new_alpha_pair(key, value, last);
        }

        stream << CONST_TAB;
        stream << "}, \n\n";

        return stream.str();
    }

    string status_register_obj(Sys *sys) {

        stringstream stream;

        stream << CONST_TAB;
        stream << "\"status_register\": {\n\n";

        for(int i = 0; i < SREG_SIZE; i++) {

            bool value = sys->read_sreg(i);
            
            bool last = (i == SREG_SIZE - 1);
            stream << new_numerical_pair(flags[i], value, last);
        }

        stream << CONST_TAB;
        stream << "}, \n\n";

        return stream.str();
    }

    string data_memory_obj(Sys *sys) {

        stringstream stream;

        stream << CONST_TAB;
        stream << "\"data_memory\": {\n\n";

        vector < tuple <string, string> > element;

        for(int i = 0; i < (RAM_END + 1); i++) {

            stringstream addr_stream;

            int8_t cell_data = sys->read_data(i);

            if(cell_data == 0x00)
                continue;

            addr_stream << "0x" << setfill('0') << setw(4);
            addr_stream << hex << i;

            string value = "0x" + get_hex(cell_data);
            string key = addr_stream.str();

            tuple <string, string> t = make_tuple(key, value);
            element.push_back(t);
        }

        int size = element.size();

        for(int i = 0; i < size; i++) {

            string key = get <0> (element[i]);
            string value = get <1> (element[i]);

            bool last = (i == (size - 1));
            stream << new_alpha_pair(key, value, last);
        }

        stream << CONST_TAB;
        stream << "}, \n\n";

        return stream.str();
    }

    string eeprom_memory_obj(Sys *sys) {

        stringstream stream;

        stream << CONST_TAB;
        stream << "\"eeprom_memory\": {\n\n";

        vector < tuple <string, string> > element;

        for(int i = 0; i < EEPROM_SIZE; i++) {

            stringstream addr_stream;

            int8_t cell_data = sys->read_eeprom(i);

            if(cell_data == 0x00)
                continue;

            addr_stream << "0x" << setfill('0') << setw(4);
            addr_stream << hex << i;

            string value = "0x" + get_hex(cell_data);
            string key = addr_stream.str();

            tuple <string, string> t = make_tuple(key, value);
            element.push_back(t);
        }

        int size = element.size();

        for(int i = 0; i < size; i++) {

            string key = get <0> (element[i]);
            string value = get <1> (element[i]);

            bool last = (i == (size - 1));
            stream << new_alpha_pair(key, value, last);
        }

        stream << CONST_TAB;
        stream << "} \n";

        return stream.str();
    }
};

string create_json(Sys *sys) {

    stringstream stream;

    stream << "{\n";

    stream << sys_prop_obj();
    stream << simul_status_obj(sys);
    stream << gpr_obj(sys);
    stream << status_register_obj(sys);
    stream << data_memory_obj(sys);
    stream << eeprom_memory_obj(sys);

    stream << "}\n";

    return stream.str();
}