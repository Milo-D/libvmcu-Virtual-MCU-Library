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
int main(void) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    
    for(int32_t i = 0; i < report->progsize; i++)
        printf("%s\n", report->disassembly[i].mnem);
    
    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

#### Printing potential labels

```c
int main(void) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    
    for(int32_t i = 0; i < report->nlabels; i++) {
        
        vmcu_label_t *lx = &report->labels[i];
        
        printf("Label ID: %d, ", lx->id);
        printf("Address: 0x%04x\n", lx->addr);
    }
    
    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

#### Printing xrefs of potential labels

```c
int main(void) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    
    for(int32_t i = 0; i < report->nlabels; i++) {

        vmcu_label_t *lx = &report->labels[i];
        printf("0x%04x\tL%d\n\n", lx->addr, lx->id);

        for(int32_t j = 0; j < lx->nxrefs; j++) {

            vmcu_xref_t *x = &lx->xrefs[j];

            printf(" xref from 0x%04x ", x->p->addr);
            printf("(%s)\n", x->p->mnem);
        }

        printf("\n");
    }
    
    vmcu_report_dtor(report);
    return EXIT_SUCCESS;
}
```

#### Simulating until hitting endless loop 

```c
int main(void) {
    
    /* ignoring checks for this example */
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex");
    vmcu_system_t *sys    = vmcu_system_ctor(report);
    
    do {
        
        int32_t pc   = vmcu_system_get_pc(sys);         // read current pc
        uint16_t opc = vmcu_system_read_flash(sys, pc); // read current opcode
        
        if(opc == 0xcfff)       // 0xcfff (big endian) = 'rjmp -1' (endless loop)
            break;
        
        vmcu_system_step(sys);
        
    } while(true);
    
    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);
    
    return EXIT_SUCCESS;
}
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

Static analysis can be performed on any AVR microcontroller type. The dynamic analysis supports
the following microcontrollers

- [x] ATmega328
- [x] ATmega328P
- [ ] ATmega48
- [ ] ATmega88 
- [ ] ATmega168
- [ ] ATmega16
- [ ] ATmega8
- [ ] ...

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
| <span style="color:red">closed for PR</span> | <span style="color:green">open for PR</span>   | <span style="color:green">open for PR</span>   | <span style="color:red">closed for PR</span>

# Credits

1) Huge thanks to <a href="https://alexander-hansen.dev">Alexander Hansen</a> for the new logo and architecture diagram. :)

# Documentation

The wiki will be updated as soon as possible. The libvmcu header files are documented, too.
