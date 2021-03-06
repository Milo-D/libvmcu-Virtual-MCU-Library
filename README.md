<img src="https://raw.githubusercontent.com/Milo-D/MDX-Assembly-Debugger/master/images/libvmcu_logo.svg" width="25%">

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5964af10f14742d19d0be39f8b3e10b6)](https://www.codacy.com/manual/Milo-D/MDX-Assembly-Debugger?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Milo-D/MDX-Assembly-Debugger&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/github/license/Milo-D/MDX-Assembly-Debugger.svg)](https://github.com/Milo-D/MDX-Assembly-Debugger.git/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/Milo-D/MDX-Assembly-Debugger.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/Milo-D/MDX-Assembly-Debugger/stargazers/)

# libvmcu - Virtual MCU Library

<img src="https://raw.githubusercontent.com/Milo-D/libvmcu-Virtual-MCU-Library/master/images/libvmcu_architecture.svg" width="75%">

VMCU is a small engine for static and dynamic analysis of AVR Microcontroller binaries. **This library is
still in development.**

libvmcu can be used to

- create (regression) tests for embedded systems
- perform binary analysis on AVR programs
- build debuggers and simulators
- explore disassembly

##### Table of Contents
[I Features](#Features)

[II Examples](#Examples)

[III Showcase](#Showcase)

[IV How VMCU works](#How-VMCU-works)

[V Setup VMCU](#Setup)

[VI Supported MCUs](#Supported-Microcontroller)

[VII Dynamic Analysis](#Dynamic-Analysis)

[VIII Static Analysis](#Static-Analysis)

[IX Instruction-Set](#Instructions)

[X Bindings](#Bindings)

[XI Contributing](#Contributing)

[XII Credits](#Credits)

[XIII Documentation](#Documentation)

# Features

### Take one step forward and one back

VMCU supports backstepping. You'll be able to simulate both, forth and back.
Note that backstepping will erase externally (via write_gpr, ...) added data.

### Cycle accurate realtime Simulation

Cycle accurate realtime simulation of the microcontroller including its peripherals.

### Decode, Decompose, Disassemble, Analyze

The pipeline offers an interface for each stage: decode, decompose, disassemble and analyze.
Stages can either operate on a single opcode or a whole binary. 

### Combine Static and Dynamic Analysis

Perform a static analysis on your binary to receive additional information, 
for example which SFRs are used by the program. Then, use this information
to improve your dynamic analysis.

### No further dependencies

VMCU comes with no further dependencies, thus allowing easy setup and easy usage.

# Examples

#### Unit-Test: timer0 interrupt frequency

```c
#define TESTFILE  "../../driver/led/led.hex"

#define PORTB     0x0025
#define PB5       0x05

#define bit(v, b) ((v & (1 << b)) >> b)

int main(const int argc, const char **argv) {

    uint8_t led;
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex(TESTFILE);
    vmcu_system_t *sys    = vmcu_system_ctor(report);
    
    do {

        vmcu_system_step(sys);
        led = vmcu_system_read_data(sys, PORTB);

    } while(bit(led, PB5) == 0x00);

    const double f    = 16000000U;
    const double c    = sys->cycles;
    const double time = (c / f);
    
    assert((0.95 <= time) && (time <= 1.05));
    printf("Time between LED toggle: %lf [s]\n", time);
    
    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);

    return EXIT_SUCCESS;
}
```

```console
[Test Result] Time between LED toggle: 1.000021 [s]
```

#### Printing disassembly of an intel hex file

```c
int main(const int argc, const char **argv) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    
    for(int32_t i = 0; i < report->progsize; i++)
        printf("%s\n", report->disassembly[i].mnem);
    
    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

```assembly
ldd r24, Y+1              ; R24 <- DATA[Y+1]
ldd r25, Y+2              ; R25 <- DATA[Y+2]
sbiw r25:r24, 0x14        ; R25:R24 <- R25:R24 - 0x14
brlt -55                  ; (N ^ V = 1): PC <- PC - 0x37 + 1
ldi r24, 0x00             ; R24 <- 0x00
ldi r25, 0x00             ; R25 <- 0x00
```

#### Printing potential labels

```c
int main(const int argc, const char **argv) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    
    for(int32_t i = 0; i < report->n_label; i++) {
        
        vmcu_label_t *lx = &report->label[i];
        
        printf("Label ID: %d, ", lx->id);
        printf("Address: 0x%04x\n", lx->addr);
    }
    
    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

```console
Label ID: 0, Address: 0x0000
Label ID: 1, Address: 0x011b
Label ID: 2, Address: 0x014d
Label ID: 3, Address: 0x0159
Label ID: 4, Address: 0x015b
Label ID: 5, Address: 0x0162
```

#### Printing xrefs of potential labels

```c
int main(const int argc, const char **argv) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    
    for(int32_t i = 0; i < report->n_label; i++) {

        vmcu_label_t *lx = &report->label[i];
        printf("0x%04x\tL%d\n\n", lx->addr, lx->id);

        for(int32_t j = 0; j < lx->n_xref; j++) {

            vmcu_xref_t *x = &lx->xref[j];

            printf(" xref from 0x%04x ", x->p->addr);
            printf("%s\n", x->p->mnem);
        }

        printf("\n");
    }
    
    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

```assembly
0x04c6  L75

 xref from 0x04a1 call +1222                ; PC <- 0x4c6
 xref from 0x0a84 call +1222                ; PC <- 0x4c6
 xref from 0x0b5c call +1222                ; PC <- 0x4c6

0x04e2  L76

 xref from 0x05d4 rjmp -243                 ; PC <- PC - 0xf3 + 1

0x05d0  L77

 xref from 0x04e1 rjmp +238                 ; PC <- PC + 0xee + 1
```

#### Printing xrefs of special function registers 

```c
int main(const int argc, const char **argv) {

    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");

    for(int32_t i = 0; i < report->n_sfr; i++) {

        vmcu_sfr_t *sfr = &report->sfr[i];
        printf("SFR ID: %d\n\n", sfr->id);

        for(int32_t j = 0; j < sfr->n_xref; j++) {

            vmcu_xref_t *x = &sfr->xref[j];

            printf(" xref from 0x%04x ", x->p->addr);
            printf("%s\n", x->p->mnem);
        }

        printf("\n");
    }

    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

```assembly
SFR ID: 17
       
 xref from 0x00f4 sbi 0x1f, 2               ; IO[1f, 2] <- 0x01
 xref from 0x00f5 sbi 0x1f, 1               ; IO[1f, 1] <- 0x01
 
SFR ID: 50

 xref from 0x004c sts 0x006e, r1            ; DATA[0x6e] <- R1
 xref from 0x0051 lds r24, 0x006e           ; R24 <- DATA[0x6e]
 xref from 0x0054 sts 0x006e, r24           ; DATA[0x6e] <- R24
```

# Showcase

![mdx_debug](https://user-images.githubusercontent.com/46600932/104666434-33f9da80-56d4-11eb-882b-724b13536412.png)
<sup>A small debugger written with libvmcu</sup>

![vcd_showcase](https://user-images.githubusercontent.com/46600932/109825592-430ffa00-7c3a-11eb-9af3-26175b962ef2.png)
<sup>VCD-Trace Tool by pointbazaar</sup>

# How VMCU works

### Analyzer Pipeline

**Stage 0:** The very first stage is the decoder. The decoder tries to decode the given Hex File.

**Stage 1:** Once the binary has been decoded successfully, the data will be sent to the decomposer, so that 
operands can be extracted and classified.

**Stage 2:** In this stage, the disassembler receives the result of Stage 0 and Stage 1 in order to generate 
mnemonics and some comments.

**Stage 3:** Now the analyzer comes into play. The analyzer takes all the data from the previous three steps 
and performs a static analysis on it. It then generates a report and returns it, so that
a virtual microcontroller can be initialized in order to start a dynamic analysis.

### Virtual System - Core

The virtual system core consists of following components:

**GPRs -** A set of general purpose registers (r0 - r31)

**SREG -** Status Register of the microcontroller

**FLASH -** Storage for program data.

**DATA -** The dataspace contains mapped GPRs, mapped SFRs and SRAM.

**I/O -** This module updates the peripherals and interrupts.

### Virtual System - Peripherals

Peripherals and interrupts are managed by the I/O module (core).

# Setup

Currently this library comes with two headers, both can be found in engine/include/libvmcu:

- libvmcu_analyzer.h  (static analysis)
- libvmcu_system.h    (dynamic analysis, simulation)

Let's say, we have a file called driver.c on top level of this repository 
and we want to link it with libvmcu:

#### Include libvmcu headers
```c
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

int main(void) {
    
    return 0;
}
```

#### Build libvmcu
```console
You@Terminal:~$ make clean all
```

#### Build driver object
```console
You@Terminal:~$ gcc -Iengine/include/libvmcu/ -c driver.c -o driver.o
```

#### Link with libvmcu (do not forget -lm)
```console
You@Terminal:~$ gcc -o driver driver.o -L/build/apps/ -lvmcu -lm
```

That's it. If you face issues, take look at some examples in the driver/ directory.

# Supported Microcontroller

The analyzer consists of architecture unspecific (called archless) and 
architecture specific submodules. Thus you can perform an analysis on any 
AVR microcontroller type.

Flags to enable/disable architecture specific analysis
and in general submodules will be added as soon as possible.

Simulation and architecture specific analysis are supported for following microcontrollers:

- [x] ATmega328(P)
- [ ] ATmega168
- [ ] ATmega88
- [ ] ATmega48

# Dynamic Analysis

- [x] Backstepping
- [x] Interrupt support
- [x] cycle accurate realtime simulation
- [x] Support for 133 AVR assembly instructions
  

- [x] Accurate simulation of internal peripherals
    - [x] 8-bit Timer (partial)
    - [ ] 16-bit Timer
    - [x] EEPROM
    - [ ] UART
    - [ ] SPI
    - [ ] WDT
    - [ ] ...
    
# Static Analysis

- [x] Decompose and classify instructions
- [x] Disassembler
- [x] Analyzer for AVR binaries
- [x] Cross references (xref-from)
- [ ] analyzer flags


- [ ] Analyzer Submodules
   - [x] Label analysis
   - [ ] Function analysis
   - [ ] ISR analysis
   - [x] SFR analysis
   - [ ] ...

- [ ] Format Reader
    - [x] intel hex
    - [ ] motorola hex
    - [ ] bin
    - [ ] elf 

# Instructions
Currently VMCU supports: ~ 133 Instructions. Some few instructions are implemented as 'nop'
instructions, therefore, have no real functionality. These instructions will be implemented
as soon as possible. Following instructions require further work:

- WDR
- ELPM
- DES
- SLEEP
- SPM
- BREAK

All other assembly instructions are working just fine.

# Bindings

- [x] Java
- [ ] Python

libvmcu has Java bindings for basic functionalities. For more information
take a look at bindings/java/

Also note that, bindings might not always work with the latest version due to development
of the engine.

# Contributing

| Engine                                       | Drivers                                        | Bindings                                       |Testing                                       |
|:--------------------------------------------:|:----------------------------------------------:|:----------------------------------------------:|:--------------------------------------------:|
| <span style="color:red">closed for PR</span> | <span style="color:green">open for PR</span>   | <span style="color:green">open for PR</span>   | <span style="color:green">open for PR</span>

# Credits

1) Huge thanks to <a href="https://alexander-hansen.dev">Alexander Hansen</a> for the new logo and architecture diagram. :)

# Documentation

The wiki will be updated as soon as possible. The libvmcu header files are documented, too.
