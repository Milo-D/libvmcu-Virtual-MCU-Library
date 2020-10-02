# Changelog

## Unreleased

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
