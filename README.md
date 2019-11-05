# MDX-Assembly-Debugger
An easy and fast CLI Assembly Debugger for 8-bit Atmel AVR
Microprocessors.

# Overview
MDX is an easy and fast CLI Debugger for AVR Assembly Programs.

You are able to open up more than just one file while debugging. This
allows you to switch fast and spontanious between your files.

If you have already loaded your file with MDX and there is a thing
you forget to add - no problem - you do not need to close the Debugger.
Just open the file you want to change, and do it. noBG will automatically
refresh all selected files.

And because MDX simulates a System for Assembly Instructions, it will be
possible to move back in time and do a "backstep".

Every Instruction is colorfully visualized. You will see exactly what
happens in the program, and which sreg's/registers/... had been changed.

(Note: MDX is still in Developement. There might be issues, feel free to
submit them in the issue segment.)

# Supported Microcontroller
- [x] ATmega32
- [x] ATmega32L

# Features
- [x] Debugging Assembly
- [ ] Backwards Stepping (coming soon)
- [x] Dynamically refreshing Source File, while debugging.
- [x] Simple and fancy Commandline Design
- [x] GPR Visualization
- [ ] SRAM Visualization
- [ ] EEPROM Visualization
- [x] SREG Visualization
- [ ] FLASH Visualization
- [ ] I/O Support
- [x] Open and debug more than one file in the same session
- [ ] Customize it
- [x] Fast and easy to use
- [x] Short CLI Commands to debug faster
- [ ] Full Instruction Support
