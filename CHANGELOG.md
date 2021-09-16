# Changelog

# Unreleased

- new driver: mbc - a driver to find multibyte comparisons in AVR binaries

# v.0.8.13 - 2021-09-06

- Now supporting Motorola SRecord

- merged pull-request (#68) from pointbazaar (Alexander Hansen)
  - added format reader for motorola hex (SRecord)
  - thanks for that :)

- refactored srec format reader (PR #68)
  - adjusted style
  - fixed some minor bugs

- connected srec format reader to pipeline

- Important: file extension decides which reader will be invoked 
  - *.hex  ==> invokes intel hex reader
  - *.srec ==> invokes srec reader

- added format reader interface (reader/reader.c)
  - reader for different file formats can be now added to reader/format/
    - for example reader/format/ihex.c to read the intel hex format

# v.0.8.12 - 2021-08-10

- reduction of heap usage and overall memory usage
  - only 10 heap allocs needed to disassemble an intel hex file

- getting rid of basic integers in important sections
  - flash address is now represented as uint32_t 
  - opcodes are (finally) represented as uint32_t
  - for more user relevant changes see libvmcu headers

- the disassembly is now sorted (ascending, sorted by address)
  - this change is mandatory for the cfg

- refactored decoder
  - decoder operates now on an intermediate representation (binary_buffer) of the binary

- added vmcu_binary_buffer_t (only for internal usage)
  - this serves as an intermediate representation for the decoder
  - every single format reader (reader/) should return a binary buffer

- refactored intel hex reader

- adjusted example section in README

# v.0.8.11 - 2021-07-07

- added: CFG - control flow graph
  - vmcu_cfg_t is the controlflow graph of libvmcu. It holds,
    - vmcu_cfg_t node*, a dynamic array of vmcu_node_t structures
    - node[0] is the entry node
      - a node consists of
        - vmcu_xref_t xto, a cross-reference to the corresponding instruction (xref-to)
        - vmcu_cfg_node_t *t, a node pointer for the true branch
        - vmcu_cfg_node_t *f, a node pointer for the false branch
    - int32_t used, a simple node counter, should be equal to report.progsize
  
- added: CFG example driver (driver/cfg/)

- using CMake now
- added syntax highlight for stepper driver

# v.0.8.10 - 2021-06-28

- Added: vmcu_access_t
  - vmcu_access_t holds bitfields (representing booleans)
  - following bitfields are available
    - registers (true if instr reads/writes gpr)
    - flash     (true if instr reads/writes flash)
    - stack     (true if instr reads/writes stack)
    - io        (true if instr reads/writes io segment)
    - ds        (true if instr reads/writes data segment)
    - sp        (true if instr reads/writes stack pointer)
    - pc        (true if instr reads/writes program counter)
    - c_flag    (true if instr reads/writes carry flag)
    - z_flag    (true if instr reads/writes zero flag)
    - n_flag    (true if instr reads/writes negative flag)
    - v_flag    (true if instr reads/writes overflow flag)
    - s_flag    (true if instr reads/writes sign flag)
    - h_flag    (true if instr reads/writes halfcarry flag)
    - t_flag    (true if instr reads/writes t flag)
    - i_flag    (true if instr reads/writes interrupt flag)
    
  - NOTE: Although (stack, io, registers ⊂ ds), (stack = true) or (io = true) 
    or (registers = true) does not imply (ds = true).

- Added: annotator stage in pipeline
  - this stage annotates instructions by adding additional information about
    the instruction itself, like groups and explicit/implicit read/write access.

- Added: rwaccess driver (driver/rwaccess/)
- Adjusted drivers, etc.

# v.0.8.9 - 2021-06-22

- VMCU_OP rework
  - rename to VMCU_OPTYPE
  - added new types to enumeration. Enumeration after rework:
    - VMCU_OPTYPE_NONE = -1 (no operand, therefore no type)
    - VMCU_OPTYPE_R,        (register operand)
    - VMCU_OPTYPE_RP,       (registerpair operand)
    - VMCU_OPTYPE_X,        (x pointer operand)
    - VMCU_OPTYPE_Y,        (y pointer operand)
    - VMCU_OPTYPE_Z,        (z pointer operand)
    - VMCU_OPTYPE_B,        (bit number 0-7)
    - VMCU_OPTYPE_K4,       (4-bit immediate)
    - VMCU_OPTYPE_K6,       (6-bit immediate)
    - VMCU_OPTYPE_K8,       (8-bit immediate)
    - VMCU_OPTYPE_IO5,      (5-bit I/O address)
    - VMCU_OPTYPE_IO6,      (6-bit I/O address)
    - VMCU_OPTYPE_D7,       (7-bit data address)
    - VMCU_OPTYPE_D16,      (16-bit data address)
    - VMCU_OPTYPE_P22,      (22-bit program address)
    - VMCU_OPTYPE_S7,       (7-bit signed displacement, program memory)
    - VMCU_OPTYPE_S12       (12-bit signed displacement, program memory)
  - similar types can be found at https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set

- Added: VMCU_REGISTER - an enumeration of AVR general purpose registers (r0-r31)
  
- Added: vmcu_registerpair_t structure
  - this structure is able to represent a registerpair by holding
    - a VMCU_REGISTER low - low byte of pair
    - a VMCU_REGISTER high - high byte of pair

- vmcu_operand_t rework
  - vmcu_operand_t now consists of a
    - VMCU_OPTYPE - an enumeration of different operand types
    - value union - a c-union holding different kinds of operand values

- The value union of vmcu_operand_t contains
  - uint8_t k  for type = K4, K6, K8
  - uint8_t b  for type = B
  - uint8_t io for type = IO5, IO6
  - uint16_t d for type = D7, D16
  - uint32_t p for type = P22
  - int16_t s  for type = S7, S12

- disassembled .dw directive (= illegal opcode) does not have its opcode as source operand anymore
  - instead, an illegal opcode (.dw xxxx) has following attributes
    - instr->src.type  = VMCU_OPTYPE_NONE
    - instr->dest.type = VMCU_OPTYPE_NONE
    - therefore no operand values
  - in order to simplify printing these "instructions", I've decided to move the .dw value (xxxx)
    to instr->mnem.base. So the base mnemonic is now ".dw xxxx" with no operands. 

- endloop driver (driver/endloop/) improvement
  - endloop driver is now able to detect unconditional and conditional endless loops

- adjusted driver code
- minor improvements

# v.0.8.8 - 2021-06-20

- Added: vmcu_mnemonic_t structure to vmcu_instr_t
  - replaces char *mnem
  - holds char literals (base, src, dest, comment)
  - base = instruction string ("ldi", "movw", etc.)
  - src = source operand string ("r29", "0xff", etc.)
  - dest = destination operand string ("r29", "0xff", etc.)
  - comment = comment string ("; r21 <- 0xab", "IF <- 0", etc.)

- Added: disasm driver with syntax highlighting (driver/disasm/)

- disassembler rewrite
- adjusted examples, drivers, readme

# v.0.8.7 - 2021-05-13

- opcode cleanup
  - removed bset due to opcode shadowing (se<*>)
  - removed bclr due to opcode shadowing (cl<*>)
  - removed brcs due to opcode shadowing (brlo)
  
- fixed inconsistency in function signatures

# v.0.8.6 - 2021-04-08 

- Added: submodule for string analysis
  - discovers strings (ascii) in binary
  
- Minor bug fix in intel hex reader

# v.0.8.5 - 2021-03-27

- Completed decoder (byte pipeline) testing.
  - Each instruction was tested by decoding two single opcodes
    - one opcode with no variable bits set
    - one opcode with random bits set
  
- Added: decoder tests for st_z and sts_32

# v.0.8.4 - 2021-03-18

- Worked on naming Convention of xrefs
  - xref-from and xref-to are using the same structure (vmcu_xref_t)
  - xref-from's will always be exposed as xfrom with a list counter n_xfrom
  - xref-to's will always be exposed as xto with a list counter n_xto

# v.0.8.3 - 2021-03-18

- Naming Convention of enums (preventing future internal name clashes)
  - VMCU_ENUMNAME and its members VMCU_ENUMNAME_*

# v.0.8.2 - 2021-03-17

- Added: interrupt vector analysis
  - finding vectors in disassembly at default table position
  - storing vector ID (VMCU_VECT enum), vector address and a xref-to vector's ISR
  
- Added: vtable section in vmcu_model_t.
  - holds data related to the MCU's vector table 
  
- Created analyzer/util/util.h for frequently (> 1) used subroutines
  - moved get_abs_addr to analyzer/util/util.h:vmcu_get_abs_addr

# v.0.8.1 - 2021-03-15

- Added: Instruction Groups
  - arithmetic/logic instructions (VMCU_GROUP_MATH_LOGIC)
  - data transfer instructions    (VMCU_GROUP_TRANSFER)
  - MCU control instructions      (VMCU_GROUP_SYS_CTRL)
  - flow control instructions     (VMCU_GROUP_FLOW)
  - bit and bit-test instructions (VMCU_GROUP_BIT)

- rename vmcu_plain_t to vmcu_instr_t
  - this might be a more obvious name than "plain"

- fixed some bugs

- added decoder tests (sbis -> sez)
  - added decoder tests (sleep -> st(Y))

# v.0.8.0 - 2021-03-14

- added devices and device models
  - in order to support (easily) more AVR types for static analysis

- added missing vmcu_ prefix in exposed functions
  - no name collisions possible anymore

- added more decoder integration tests (com -> elpm)
  - added decoder tests (elpm -> lpm)
  - added decoder tests (lsr -> push)
  - added decoder tests (rcall -> sbic)

- moved sfr_lookup[] table to a separate file. 
- adjusting readme ;)

# v.0.7.4 - 2021-03-09

- Improved SFR analyzer module
  - added vmcu_sfr_t to vmcu_report_t
  - added vmcu_xref_t to vmcu_sfr_t
  - removed mnemonic manipulation in sfr analyzer
  - sfr analyzer is now collecting SFRs and storing xrefs

- Minor change in naming convention
  - using singular of structnames instead of plural
  - using prefix n_ for list counts
  - example: lists of structs in report: structname
  - example: list count of structs in report: n_structname

- Bugfix in occurence driver
- COMn(A/B)x bits are now overriding normal port function

# v.0.7.3

- Added: xrefs (xref-from)
  - a xref holds a reference to vmcu_plain_t, so that
    all instruction-specific details can be accessed
    through an instance of vmcu_xref_t
  - for example: xref->p->mnem, xref->p->addr, etc.
    
  - replaced vmcu_caller_t with vmcu_xref_t
  - labels now have xrefs

- Bug fix in 'or' assembly instruction
  - forgot to set flags. Sorry :(

- Thanks to pointbazaar for the VCD driver

# v.0.7.2 - 2021-02-26

- Improved stepper driver
  
- Added: Integration tests
  - system integration tests
  - decoder integration tests
  
- minor refactor of the ihex decoder

# v.0.7.1 - 2021-02-22

- Added Java Bindings for libvmcu
    - supports basic functionalities of libvmcu

- Added example of a possible firmware unit test with libvmcu (driver/led/)

- Added option to build libvmcu as a dynamic library

# v.0.7.0 - 2021-02-20

- MDX is now a library for static and dynamic analysis of
  AVR binaries called libvmcu
  
- rename MDX to libvmcu (VMCU)

- removed CLI and debugger from engine
  - debugger will be available in another repository
  
Comment: I don't like working on graphical user interfaces so I've decided
to kick out the GUI, so that I can focus on the interesting part: the engine.

The engine is available as a statically linked library, called libvmcu. It offers
static analysis of AVR source code on assembly level. It is possible to analyze
potential labels and their callers/jumpers, special functions register analysis,
instruction decomposing, and more. Besides that, libvmcu offers dynamic analysis in
the form of cycle accurate realtime simulation of the ATmega328(P) family, including
its peripherals.

# v.0.6.0 - 2021-02-13

- Massive reduction of heap usage (Issue #47)
  
- Added: New Analyzer Submodule: Label-Analyzer
  - This module analyzes jumps, branches and calls in order to
    find possible labels in disassembly.
  - it also discovers callers of labels
  
- Generated Labels are now sorted (ascending)
- Fixed Bug in Disassembler (Issue #46)

- Performance Enhancement (runtime) of Disassembler and Analyzer

- Added new example program (test/m328p/misc/delay.hex)
- removed include path
- renamed function sys_read_instr to sys_read_progmem

# v.0.5.5

- Seperated Simulation Engine (engine/) from CLI (debugger/) 
  - Might create a statically linked library out of engine/,
  so that the core can be reused in different projects.

# v.0.5.4

- Added first Submodule for the Analyzer: Static SFR Analysis
  - This submodule explores special function registers in disassembly
  - once an address to a special function register has been detected 
    by the module, an annotation will be created with its corresponding name
    
  - example: out 0x3f, r1 ; SREG

- Minor refactor of timer8.
- New Debugcommand: xeb - examine EEPROM as bitmap
- Cleanup: unused Headers, unused variables

# v.0.5.3 - 2020-01-23

- Added: Instruction Decomposer - Stage 1 of the Analyzer Pipeline
    - decomposes AVR Opcodes into its components

- Extended Analyzer Pipeline:
    - Stage 0: Decoder        (decodes binaries, extracts opcodes)
    - Stage 1: Decomposer     (decomposes opcodes into its operands)
    - Stage 2: Disassembler   (disassembles opcodes into mnemonics)
    - Stage 3: Analyzer       (performs static analysis on disassembly)

- Timer0: Minor work on Prescaler.
- Added: EEPROM Write-Only Mode
    - Now supporting all modes of EEPROM programming

- Added: Opcode view in disassembly.

## v.0.5.2 - 2020-01-13

- This update reduces heap allocations for an overall better memory performance
- Further improvements regarding memory consumption are coming soon

- Now using sstream in mnemonics generator and debugprinter
    - avoiding unnecessary heap allocations
    
- Added: stringstream collection (sstream.c, sstream.h)
    - will be replacing queue collection (queue.c, queue.h)

## v.0.5.1 - 2020-01-10

- Systemprinter refactored
    - Rename sytemprinter.* to debugprinter.*
    - Seperated printer functionality into six files:
        - gpnl_printer.* (printer for the gpr panel)
        - spnl_printer.* (printer for the sreg panel)
        - fpnl_printer.* (printer for the flash panel)
        - dpnl_printer.* (printer for the data panel)
        - epnl_printer.* (printer for the eeprom panel)
        - rpnl_printer.* (printer for the right panel)
    - functionality stayed the same

- Added credits for the architecture diagram.
- Fixed comments for push/pop mnemonic.

## v.0.5.0 - 2020-12-31

- Added: Analyzer Module.
    - The Analyzer Module consists of multiple (future) submodules.
    - Each submodule will be covering exactly one aspect of static code analysis.
    - Planned submodules:
        - ISR analysis (explore details about ISRs in your code)
        - SFR analysis (annotate addresses which target special function registers)
        - Function analysis (analyse function calls and maybe even their arguments/return-values)
        - many more...
    - The analyzer is a work in progress

- Performance: Reached 17 MHz execution speed. It is now possible to simulate default Arduino in realtime.

- Refactor of toplevel structures
    - table_t class is now only for user-defined entries like breakpoints, etc.
    - system_t class does not contain table_t class anymore.
    - report_t class is the report of the analyzer after decoding and disassembling hexfile.

- Displaying hexadecimal addresses instead of line numbers in sidepanel.
- Table commands (like break, unbreak) are now requiring a hexadecimal address

- Added: Diagram of the analyzer dataflow
- Added: New screenshot

- DebugWindow Refactor (II)
    - panel properties are now calculated seperately in <g,s,f,d,e,o,r> pnl_ctor

## v.0.4.5 - 2020-12-21

- DebugWindow refactored.
    - No use of generic panels in DebugWindow anymore
    - Created subdirectory: src/cli/util/ and include/cli/util
    - Panel class is now stored in cli/util/
    - Created individual panels (gpnl_t, etc.) in cli/debug/
    - gpnl_t, spnl_t, fpnl_t, dpnl_t, epnl_t, opnl_t, rpnl_t are extending panel_t (cli/util/panel.h)
    - Created _win_properties which holds data related to the window (WINDOW*) state (height, width, y, x)
    - struct _properties extends struct _win_properties (adding page properties)
    - Panels are now initialized by struct _properties*
    - Prompt is now initialized by struct _win_properties* (prompt has no page properties)
    - Right now extending panel_t class has no real benefits, since all individual panels are the same,
    but this will change in the future.

- Added: 'cc' debugcommand. Create comments in disassembly with 'cc <line> <comment>'
- Statusregister can be read/written now by writing to its dataspace address (0x005f)

## v.0.4.4 - 2020-12-01

- Major Performance Enhancement
    - 56% better runtime performance
        - before: 7.250.000 Cycles per second (7,25 MHz)
        - after:  12.800.000 Cycles per second (12,8 MHz)
    - Tested on an Intel Core i5. So actual execution speed may differ.
    - Goal: atleast 20 MHz

- Fixed some issues in the Makefile

## v.0.4.3 - 2020-11-17

- Added new Debugcommand: jc <cycle offset n> (jump cycles)
    - Fast forwarding simulation without the need for setting breakpoints
    - Useful when waiting for an event in an endless loop, like EEPROM Ready or a Timer Overflow.

- Bugfix: Timer increments now, even when executing illegal opcode.
- Timer Interrupts are now handled in TIFR0 and TIMSK0, too.

## v.0.4.2 - 2020-11-13

- Added two functions to system_t: 
    - sys_set_sfr(system_t*, const uint16_t, const int)
    - sys_clear_sfr(system_t*, const uint16_t, const int)
    
- The first 32 registers in IO-Space are now set/cleared by writing single bits when using sbi, cbi
- Headless Mode is now ignoring EEPROM values equals to 0xff, in order to minimize output 
- Bugfix: Flag clearing in TIFR0
- Added new testfile: kmp.asm (KMP Pattern Matching for M328P)

## v.0.4.1 - 2020-11-10

- Refactor: Splitted IO into two submodules (io.h, sfr.h)
- Cleanup: Improved code readability of bitfunctions in peripherals
- Fixed some unwanted gcc warnings
- Bugfix: COM0Bn bits are now working correctly
- Stackpointer Initialization to RAMEND

## v.0.4.0 - 2020-11-09

- Added EEPROM Module
    - Cycle accurate simulation of EEPROM
    - 2 out of 3 EEP Modes implemented
        - Atomic Erase/Write Mode
        - Erase Mode
    - EEPROM Ready Interrupts (ERDY)
    - Reading EEPROM by setting EERE

- Switching to ATmega328P
- Stack Bug-Fix

Note: I've decided to switch to the ATmega328P due to the popularity
of these chips (Arduino). Once the core of ATmega328P is done, the
ATmega32(L) (and other MCUs) will be included.

## v.0.3.0 - 2020-10-23

- MDX is now supporting whole AVR Instruction-Set. Some few instructions still have no
functionality when simulating, but, atleast, it is possible to decode and disassemble them. 
Missing functionality of instructions will be added soon.

- Now supporting .dw directive
- Added new assembly instructions:
    - wdr (watchdog reset) (currently no functionality)
    - break (break for on-chip debugsystem) (currently no functionality)
    - lds (16-bit, load direct from data space)
    - brhc (branch if halfcarry is cleared)
    - brhs (branch if halfcarry is set)
    - brid (branch if global interrupts are disabled)
    - brie (branch if global interrupts are enabled)
    - brvc (branch if overflow flag is cleared)
    - fmuls (fractional multiply, signed)
    - fmulsu (fractional multiply, signed-unsigned)
    - spm (store program memory) (currently no functionality) 
    - spm(Z+) (store program memory + post-increment) (currently no functionality)

## v.0.2.2 - 2020-10-22

- Timer Interrupt flags can be cleared now by user-software
- Added new assembly instructions:
    - mul (multiply unsigned)
    - ld(-Z) (indirect load using Z pointer + pre-decrement)
    - sbic (skip if bit in IO space is cleared)
    - cbi (clear bit in IO space)

- New assembly instructions (currently without functionality):
    - elpm (extented load program memory, R0)
    - elpm(Z) (extended load program memory, Rd)
    - elpm(Z+) (extended load program memory + post-increment)
    - sleep (sets the circuit in sleep mode)
    - des (single iterations of the data encryption standard)
    - eijmp (extended indirect jump)
    - eicall (extended indirect call)
    
Note: instructions without functionality will be implemented later, as soon as
some new MCUs (which actually support these instructions) are added to MDX. Also
some instructions (like sleep and spm) are requiring specific functionalities in the IO-space, 
which are, currently, not supported by MDX.
    
- Timer0: Wave Generation using COM-bits in non-PWM Mode is now supported
- Added: special behaviour for registers located in IO-Space.
- Bug fix: 'eor' is no longer decoded as 'clr'

## v.0.2.1 - 2020-10-12

- Bugfix for the CTC Mode
- Simulation now cycle based

## v.0.2.0 - 2020-10-10

- Now supporting CTC Mode for Timer0
- Added new assembly instructions:
    - st(Y) (indirect store using Y pointer)
    - st(Y+) (indirect store using Y pointer + post-increment)
    - st(-Y) (indirect store using Y pointer + pre-decrement)
    - st(Z) (indirect store using Z pointer)
    - st(Z+) (indirect store using Z pointer + post-increment)
    - st(-Z) (indirect store using Z pointer + pre-decrement)

- Minor bug fix for the 8-bit timer0 
- Added annotations for special function registers

## v.0.1.1 - 2020-09-30

- Added new assembly instructions:
    - lpm(Z+) (load program memory)
    - lds(32-bit) (load data memory)
    - cpse (compare skip if equal)
    - sbrs (skip if bit in register is set)
    - st(-X) (store to dataspace with pre-decrement)

- Improved memory usage of interactive mode
- Now passing ∆t instead of ∆c to update_io function

## v.0.1.0 - 2020-09-28

- Peripheral: Timer0 (8-bit) is now supported.
    - Supported Prescaling: 1/8/64/256/1024
    - Supported Mode: Normal Mode
    - Currently not supported: PWM and external clock select  

- Timer interrupts are now available.

- Added new assembly instructions:
    - lpm(R0) (load program memory)
    - lpm(Z) (load program memory)
    - call (32-bit call)
    - las (load and set byte in data space)
    - lac (load and clear byte in data space)
    - fmul (fractional multiply unsigned)
    - reti (return from interrupt)

- Bug fix for the preprocessor
- Skip instructions are now compatible with 32-bit opcode
- Jumps, skips, etc. are now able to wrap around FLASH memory

- Refactored System class (Core / Peripherals)
- Seperate IO space class
- IRQ Handler

## v.0.0.2 - 2020-09-17

- 32-bit Decoder: 32-bit opcodes are now supported
- Memory usage of MDX reduced by one third
- Slight performance ehancement (Disassembler)
- Major CLI performance enhancement when stepping through code
- Now supporting .org directive
- New debugcommand: 'xdb' - examine byte in dataspace as bitfield
- Refactored ALU class
- New colorscheme
- Bugfix: Labels are now generated correctly even when using .org
- Added: FLASH visualization with correct flash addresses
- Added: Testfiles including functional programming and memory allocation in avr-asssembly 
- Added: jmp assembly instruction
- Added: sts (32-bit) assembly instruction
- Added: ijmp assembly instruction
- Added: sbci assembly instruction
- Added: sbrs assembly instruction

## v.0.0.1 - 2020-09-08

- Refactor: Table is now member of system
- Refactor: Minor changes in system class
- Refactor: System members now have 'reboot' functionality
- Enhancement: .ORG assembly directive is now supported
- Enhancement: Backstepping is a tiny bit more efficient
- Enhancement: FLASH is now visible in the codepanel
- Enhancement: Minor CLI changes
- Added: Changelog.md
