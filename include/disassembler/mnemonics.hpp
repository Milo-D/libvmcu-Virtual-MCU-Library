/* Mnemonic Generator */

#ifndef MNEMONICS_HPP
#define MNEMONICS_HPP

#define INSTR_MAX

extern std::string (*mnemonics[INSTR_MAX]) (int opcode);

#endif