# Changelog

## Unreleased

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
