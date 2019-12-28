/* Disassembler Header */

#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

/*
* This Disassembler re-creates the Assembly
* Source Code out of the Hexcode found in Hex File.
* Then it automatically generates Labels after
* Jumps and Branches and replaces the relative addresses
* of these instructions.
*/

std::vector < std::tuple <std::string, int> > create_source(std::string hex_file);

#endif