/* LabelMap Header */

#ifndef LABELMAP_HPP
#define LABELMAP_HPP

#include <map>

class LabelMap {

public:
    LabelMap(void);                                                 // LabelMap Constructor

    int add(std::string line, int i);                               // adding a label to its position, returning label queue index
    void sort_up(void);                                             // sorting 'labels' by 'addr' (ascending)
    std::tuple <int, std::string> get(int index);                   // getting label at 'index'

    int size_of(void);                                              // returning LabelMap size

private:
    int size;                                                       // size of labelmap
    std::vector < std::tuple <int, std::string> > labels;           // storage for future labels
    std::map <std::string, bool> imap;                              // map for instructions with an address as operand

    std::string get_label(void);                                    // creating a new label based on size
};

#endif