# Changelog

## Unreleased

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
