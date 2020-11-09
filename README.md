<img src="https://raw.githubusercontent.com/Milo-D/MDX-Assembly-Debugger/master/images/mdx-logo.svg" width="25%">

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5964af10f14742d19d0be39f8b3e10b6)](https://www.codacy.com/manual/Milo-D/MDX-Assembly-Debugger?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Milo-D/MDX-Assembly-Debugger&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/github/license/Milo-D/MDX-Assembly-Debugger.svg)](https://github.com/Milo-D/MDX-Assembly-Debugger.git/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/Milo-D/MDX-Assembly-Debugger.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/Milo-D/MDX-Assembly-Debugger/stargazers/)
[![Chat](https://img.shields.io/badge/chat-on%20discord-7289da.svg?sanitize=true)](https://discord.gg/X9kmyyh)
# MDX-Micro-Debugger (In Development)
An easy and fast CLI Debugger and Disassembler for Atmel's 8-bit AVR
Microcontrollers, including ATmega328P.

# Overview
![mdx_debug0](https://user-images.githubusercontent.com/46600932/94999441-c2c96800-05b9-11eb-8c72-4e9cdeea0899.png)

MDX is an easy and fast CLI Debugger for AVR programs, with a focus on accurate simulation. It has some nice extra
features built in like:

- Backstepping
- Peripheral Simulation (Timers, ...)
- Annotations for IO-Registers
- Dataflow Visualization
- Headless Mode
- easy setup and easy usage

(MDX is still in Developement. There might be issues, feel free to submit them in the issue segment.)

##### Table of Contents
[I How MDX works](#How-MDX-works)

[II Installation](#Installation)

[III Cheatsheet](#Cheatsheet)

[IV Troubleshooting](#Troubleshooting)

[V Supported MCUs](#Supported-Microcontroller)

[VI Features](#How-To)

[VII Instruction Set](#Instructions)

[VIII Contributing](#Contributing)

[IX Credits](#Credits)

[X Screenshots](#Screenshots)

[XI Wiki](#Wiki)

# How MDX works
MDX accepts AVR Hex Files (Intel Hex) as input and decodes them. Then, a readable
Assembly Source File is reconstructed from the decoded Hexdump.
Now you are able to step through the Source Code and see what really happens.

You will find some example programs in the /test/ directory of this Repository.

# Installation
Setting up MDX isn't really difficult. Since the new CLI is based on NCurses,
you will need to get NCurses before hitting make.

After that, only 4 Steps are left:

- Step 1: Clone this repo.
```console
You@Terminal:~$ git clone https://www.github.com/Milo-D/MDX-Assembly-Debugger.git/
```

- Step 2: Run 'make' to compile MDX.
```console
You@Terminal:~$ make clean all
```

- Step 3: (Optional) Move mdx to /usr/bin/
```console
You@Terminal:~$ mv build/apps/mdx /usr/bin/
```

- Step 4: Run MDX.
```console
You@Terminal:~$ mdx <file.hex>
```

# Cheatsheet

Note: You may open multiple files in interactive Debugging Mode.

| CL Options    | Arguments     | Description                 |
| ------------- |:-------------:|:----------------------------|
| none          | <file.hex>    | Interactive Debugging Mode  |
| -d            | <file.hex>    | Disassembler                |
| -hl           | <file.hex>    | Headless Mode               |
| -h            | none          | Help (in progress)          |



| Table Commands| Arguments     | Description                 |
| ------------- |:-------------:|:----------------------------|
| break         | address       | Setting a Debug Breakpoint  |
| unbreak       | address       | Removing Debug Breakpoint   |
| def           | alias seq     | Defining 'seq' as 'alias'   |
| pn            | none          | Show next Table Page        |
| pp            | none          | Show previous Table Page    |
   
   
   
| Debug Commands| Arguments     | Description                 |
| ------------- |:-------------:|:----------------------------|
| n             | none          | Single Step forward         |
| b             | none          | Step backwards              |
| rn            | none          | Show next GPR Page          |
| rp            | none          | Show prev. GPR Page         |
| dn            | none          | Scroll to next Data Cell    |
| dp            | none          | Scroll to prev. Data Cell   |
| jb            | delay in ms   | Jump to next Breakpoint     |
| en            | none          | Scroll to next EEPROM Cell  |
| ep            | none          | Scroll to prev. EEPROM Cell |
| xd            | address       | Examine Data Memory         |
| xe            | address       | Examine EEPROM Memory       |
| xec           | start end     | EEP. Mem. as char literal   |
| xdc           | start end     | Data Mem. as char literal   |
| xdb           | address       | Data Mem. as Bitfield       |
| leep          | file.eep.hex  | Load .eep.hex into MCU      |
| cycles        | none          | Show current Cycles         |
| clock         | none          | Show Clock Frequency        |
| time          | none          | Show elapsed Time           |
| q             | none          | Back to File Selector       |
| q + q         | none          | Quit                        |
| ?             | none          | Show Help (in progress)     |


# Troubleshooting

-  Issue: Stepping over a non-nop instruction has no effect on the MCU.

-  Solution: Some few instructions (like spm, wdr, break) require some further work.
   For example, the 'wdr' (watchdog reset) instruction requires (obviously) a watchdog
   timer, which MDX, currently, does not support. I will be working on adding functionality
   to these few instructions, as soon as possible. For further information, see (#Instructions)
   
- Issue: Wrong Hex Format.

- Solution: Currently, the Decoder accepts only capital hexadecimal letters. This, or any
  other manipulation (adding useless space, new-lines, ...) could lead to such an exception.
  Non-capital letters will be supported soon.

- Issue: CLI Glitches

- Solution: MDX has a new CLI, which is based on NCurses. There might be some glitches.
  In general, if you experience some glitches, try resizing your terminal and hit a key.
  This may trigger the Signal Handler, and the screen will be rewritten.
  In case of serious glitches and issues, feel free to submit them in the Issue Segment.

# Supported Microcontroller
- [x] ATmega328
- [x] ATmega328P
- [ ] ATmega16
- [ ] ATmega8
- [ ] ...

# Features
- [x] Backwards Stepping
- [x] Advanced Disassembler, recovering Labels
- [x] Syntax Highlight for the disassembled Sourcecode
- [x] Interrupts supported
- [x] Open and debug more than one file in the same session
- [ ] Currently supporting ~ 133 assembly instructions

- [x] Breakpoints
- [ ] Watchpoints
- [x] Examine Memory

- [x] Interactive Mode
- [x] Disassembler Mode
- [x] Headless Mode

- [ ] Peripherals
    - [x] 8-bit Timer (partial)
    - [ ] 16-bit Timer
    - [x] EEPROM
    - [ ] UART
    - [ ] SPI
    - [ ] ...

# Instructions
Currently MDX supports: ~ 133 Instructions. Some few instructions are implemented as 'nop'
instructions, therefore, have no real functionality. These instructions will be implemented
as soon as possible. Following instructions require further work:

- WDR
- ELPM
- DES
- SLEEP
- SPM
- BREAK

All other assembly instructions are working just fine.

# Contributing

Currently I wont merge Pull-Requests. This will change once I've organized the
basic structure and architecture of this Project. But still bug reports (or any other report of an issue) 
are welcome and if you have some changes/wishes in mind, do not hesitate to contact me.

# Credits

1) Huge thanks to <a href="https://alexander-hansen.dev">Alexander Hansen</a> for the new Logo :)

# Screenshots

By the way: You will find more Screenshots in the 'img' directory of this Repo.

Interactive Debugger:

<img src="https://user-images.githubusercontent.com/46600932/92971591-0b8d7580-f481-11ea-96e0-4e8191679f15.png" width="75%">

File Selector:

<img src="https://user-images.githubusercontent.com/46600932/92972200-1eed1080-f482-11ea-92cb-8cee66b31dfb.png" width="75%">

Disassembler:

<img src="https://user-images.githubusercontent.com/46600932/92750758-aefa5f80-f387-11ea-9c2f-23b040b2b161.png" width="75%">

Headless Mode:

<img src="https://user-images.githubusercontent.com/46600932/72665868-e7ba8280-3a0c-11ea-9c0a-5482cd337d63.png" width="75%">

# Wiki

For more details and information on MDX, see https://github.com/Milo-D/MDX-Assembly-Debugger/wiki
