![mdx_logo](https://user-images.githubusercontent.com/46600932/72194353-9ce9ab00-340d-11ea-9757-d223de68ccfa.png)

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5964af10f14742d19d0be39f8b3e10b6)](https://www.codacy.com/manual/Milo-D/MDX-Assembly-Debugger?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Milo-D/MDX-Assembly-Debugger&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/github/license/Milo-D/MDX-Assembly-Debugger.svg)](https://github.com/Milo-D/MDX-Assembly-Debugger.git/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/Milo-D/MDX-Assembly-Debugger.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/Milo-D/MDX-Assembly-Debugger/stargazers/)
# MDX-Assembly-Debugger (In Development)
An easy and fast CLI Assembly Debugger and Disassembler for 8-bit Atmel AVR
Microcontrollers.

# Overview
![mdx](https://user-images.githubusercontent.com/46600932/73902171-2cf80480-4895-11ea-8ce9-a26198dc3ba9.gif)

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

[VIII Screenshots](#Screenshots)

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
| leep          | file.eep.hex  | Load .eep.hex into MCU      |
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
- [x] Debugging Assembly
- [x] Advanced Disassembler, recovering Labels
- [x] Backwards Stepping
- [x] Simple and fancy Commandline Design
- [x] GPR Visualization
- [x] SRAM Visualization
- [x] Loading .eep.hex into the Microcontroller
- [ ] EEPROM Visualization
- [x] SREG Visualization
- [ ] FLASH Visualization
- [ ] Relation between Code Segments
- [ ] I/O Support
- [ ] Seperate I/O Pin View
- [x] Open and debug more than one file in the same session
- [x] Fast and easy to use
- [x] Short CLI Commands to debug faster
- [ ] Full Instruction Support

# Instructions
Currently MDX supports: ~ 72 Instructions. More Instructions are coming soon.

# Screenshots

By the way: You will find more Screenshots in the 'img' directory of this Repo.

Interactive Debugger:
![mdx_debug](https://user-images.githubusercontent.com/46600932/73900886-5dd63a80-4891-11ea-8e89-114e67dfb1a3.png)

File Selector:
![mdx_file](https://user-images.githubusercontent.com/46600932/73900923-77778200-4891-11ea-9e05-0dce09b05fbc.png)

Disassembler:
![mdx_disassembler](https://user-images.githubusercontent.com/46600932/72665866-e7ba8280-3a0c-11ea-8bd5-24d1f4a1c3aa.png)

Headless Mode:
![mdx_headless](https://user-images.githubusercontent.com/46600932/72665868-e7ba8280-3a0c-11ea-9c0a-5482cd337d63.png)
