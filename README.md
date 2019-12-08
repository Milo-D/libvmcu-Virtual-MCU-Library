# MDX-Assembly-Debugger                        
An easy and fast CLI Assembly Debugger for 8-bit Atmel AVR
Microcontrollers.

# Overview
MDX is an easy and fast CLI Debugger for AVR Assembly Programs.

You are able to open up more than just one file while debugging. This
allows you to switch fast and spontanious between your files.

Every Instruction is colorfully visualized. You will see exactly what
happens in the program.

(MDX is still in Developement. There might be issues, feel free to submit them in the issue segment.)

##### Table of Contents
[I How MDX works](#How-MDX-works)

[II Installation](#Installation)

[III Troubleshooting](#Troubleshooting)

[IV Supported MCUs](#Supported-Microcontroller)

[V Features](#How-To)

[VI Instruction Set](#Instructions)

[VII Screenshots](#Screenshots)

# How MDX works
MDX accepts AVR Hex Files as input and decodes them. Then, a readable
Assembly Source File is reconstructed from the decoded Hexdump.
Now you are able to step through the Source Code and see what really happens.

You will find some example programs in the /test/ directory of this Repository.

# Installation
Setting up MDX isn't really difficult. There are only 5 Steps:

- Step 1: Clone this repo.
```console
You@Terminal:~$ git clone https://www.github.com/Milo-D/MDX-Assembly-Debugger.git/
```

- Step 2: Run 'build.sh' to compile MDX.
```console
You@Terminal:~$ ./build.sh
```

- Step 3: (Optional) Move mdx to /usr/bin/
```console
You@Terminal:~$ mv mdx /usr/bin/
```

- Step 4: Run MDX.
```console
You@Terminal:~$ mdx <file.hex>
```

# Troubleshooting

-  Issue: Could not parse Hexcode.

-  Solution: Because MDX is still in Development, it does not support every
   Instruction. I am working on adding new Instructions to MDX. 

# Supported Microcontroller
- [x] ATmega32
- [x] ATmega32L
- [ ] ATmega16
- [ ] ATmega8
- [ ] ...

# Features
- [x] Debugging Assembly
- [ ] Backwards Stepping
- [x] Simple and fancy Commandline Design
- [x] GPR Visualization
- [x] SRAM Visualization
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
Currently MDX supports: 35 Instructions. More Instructions are coming soon.

# Screenshots

By the way: You will find more Screenshots in the 'img' directory of this Repo.

![mdx_debug](https://user-images.githubusercontent.com/46600932/69670835-182d1280-1095-11ea-8fd7-0117d2dfbbbf.png)
