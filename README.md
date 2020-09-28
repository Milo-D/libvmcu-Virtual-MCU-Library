<img src="https://raw.githubusercontent.com/Milo-D/MDX-Assembly-Debugger/master/images/mdx-logo.svg" width="25%">

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5964af10f14742d19d0be39f8b3e10b6)](https://www.codacy.com/manual/Milo-D/MDX-Assembly-Debugger?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Milo-D/MDX-Assembly-Debugger&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/github/license/Milo-D/MDX-Assembly-Debugger.svg)](https://github.com/Milo-D/MDX-Assembly-Debugger.git/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/Milo-D/MDX-Assembly-Debugger.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/Milo-D/MDX-Assembly-Debugger/stargazers/)
[![Chat](https://img.shields.io/badge/chat-on%20discord-7289da.svg?sanitize=true)](https://discord.gg/X9kmyyh)
# MDX-Assembly-Debugger (In Development)
An easy and fast CLI Assembly Debugger and Disassembler for 8-bit Atmel AVR
Microcontrollers.

# Overview
![mdx_debug](https://user-images.githubusercontent.com/46600932/92971591-0b8d7580-f481-11ea-96e0-4e8191679f15.png)

MDX is an easy and fast CLI Debugger for AVR Assembly Programs.

You are able to open up more than just one file while debugging. This
allows you to switch fast and spontanious between your files.

Did you ever stepped too far? No problem, you don't have to restart the Session.
Just **step back** and continue debugging.

Additionally, every Instruction is colorfully visualized. You will see exactly what
happens in the program.

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

[IX Screenshots](#Screenshots)

# How MDX works
MDX accepts AVR Hex Files as input and decodes them. Then, a readable
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

-  Issue: Could not parse Hexcode.

-  Solution: Because MDX is still in Development, it does not support every
   Instruction. I am working on adding new Instructions to MDX. Also manipulating
   the Hexfile can lead to a corrupted Hexfile, which MDX wont parse.
   
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
- [x] ATmega32
- [x] ATmega32L
- [ ] ATmega16
- [ ] ATmega8
- [ ] ...

# Features
- [x] Backwards Stepping
- [x] Advanced Disassembler, recovering Labels
- [x] Syntax Highlight for the disassembled Sourcecode
- [x] Interrupts supported
- [x] Open and debug more than one file in the same session
- [ ] Currently supporting ~ 93 assembly instructions

- [x] Breakpoints
- [ ] Watchpoints
- [x] Examine Memory

- [x] Interactive Mode
- [x] Disassembler Mode
- [x] Headless Mode

- [ ] Peripherals
    - [x] 8-bit Timer (partial)
    - [ ] 16-bit Timer
    - [ ] EEPROM
    - [ ] UART
    - [ ] SPI
    - [ ] ...

# Instructions
Currently MDX supports: ~ 93 Instructions. More Instructions are coming soon.

# Contributing

Currently I wont merge Pull-Requests. This will change once I've organized the
basic structure and architecture of this Project. But still bug reports (or any other report of an issue) 
are welcome and if you have some changes/wishes in mind, do not hesitate to contact me.

# Screenshots

By the way: You will find more Screenshots in the 'img' directory of this Repo.

Interactive Debugger:
![mdx_debug](https://user-images.githubusercontent.com/46600932/92971591-0b8d7580-f481-11ea-96e0-4e8191679f15.png)

File Selector:
![mdx_file](https://user-images.githubusercontent.com/46600932/92972200-1eed1080-f482-11ea-92cb-8cee66b31dfb.png)

Disassembler:
![mdx_disassembler](https://user-images.githubusercontent.com/46600932/92750758-aefa5f80-f387-11ea-9c2f-23b040b2b161.png)

Headless Mode:
![mdx_headless](https://user-images.githubusercontent.com/46600932/72665868-e7ba8280-3a0c-11ea-9c0a-5482cd337d63.png)
