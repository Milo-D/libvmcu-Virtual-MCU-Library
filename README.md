# noBG-Assembly-Debugger
An easy and fast CLI Debugger for Assembler.

# Overview
noBG is an easy and fast CLI Debugger for Assembly programs written with
the Intel Syntax.

You are able to open up more than just one file while debugging. This
allows you to switch fast and spontanious between your files.

If you have already loaded your file with noBG and there is a thing
you forget to add - no problem - you do not need to close the Debugger.
Just open the file you want to change, and do it. noBG will automatically
refresh all selected files.

And because noBG simulates a System for Assembly Instructions, it will be
possible to move back in time and do a "backstep". Atleast that's what
we are working on.

Every Instruction is colorfully visualized. You will see exactly what
happens in the program, and which flags/registers/... had been changed.

(Note: noBG is still in Developement. There might be issues, feel free to
submit them in the issue segment.)

# Features
- [x] Debugging Assembly (Intel Syntax)
- [ ] Backwards Stepping (coming soon)
- [x] Dynamically refreshing Source File, while debugging.
- [x] Simple and fancy Commandline Design
- [x] Register Visualization
- [x] Stack Visualization
- [ ] Heap Visualization
- [ ] Flag Visualization
- [x] Open and debug more than one file in the same session
- [ ] Customize it
- [x] Fast and easy to use
- [x] Short CLI Commands to debug faster
- [ ] Support for other Syntax
- [ ] Full Instruction Support
