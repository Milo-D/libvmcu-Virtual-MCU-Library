<img src="https://raw.githubusercontent.com/Milo-D/MDX-Assembly-Debugger/master/images/libvmcu_logo.svg" width="25%">

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5964af10f14742d19d0be39f8b3e10b6)](https://www.codacy.com/manual/Milo-D/MDX-Assembly-Debugger?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Milo-D/MDX-Assembly-Debugger&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/github/license/Milo-D/MDX-Assembly-Debugger.svg)](https://github.com/Milo-D/MDX-Assembly-Debugger.git/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/Milo-D/MDX-Assembly-Debugger.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/Milo-D/MDX-Assembly-Debugger/stargazers/)

# libvmcu - Virtual MCU Library

<img src="https://raw.githubusercontent.com/Milo-D/libvmcu-Virtual-MCU-Library/master/images/libvmcu_architecture.svg" width="75%">

libvmcu is a small engine for static and dynamic analysis of AVR Microcontroller binaries.

It takes care of the preparation of raw data, which can then be further processed by other programs. 
The goal here is to make it possible to interact programmatically with AVR source code.

libvmcu can be used to

- perform binary analysis on AVR programs
- build debuggers and simulators
- explore disassembly

**Note: This library is still in development.**

### Table of Contents
[I Features](#Features)

[II Examples](#Examples)

[III Showcase](#Showcase)

[IV How VMCU works](#How-libvmcu-works)

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

### Interact with the binary

With libvmcu you are able to interact programmatically with your AVR source code. You can filter
disassembly, find endless loops, extract details from opcode (for example affected flags) and more.

### Analyzer Pipeline

The pipeline offers an interface for each stage: decode, annotate, decompose, disassemble and analyze.
Stages can either operate on a single opcode or a whole binary.

### Cycle accurate realtime Simulation

Although this project primarily focus on static analysis, it offers an accurate realtime simulation of 
some few microcontroller including their peripherals. 

### No further dependencies

libvmcu comes with no further dependencies, thus allowing easy setup and easy usage.

# Examples

#### Extracting details from opcode

```c
/* 0x6a97 (little endian) <=> sbiw r29:r28, 0x1a */

int main(const int argc, const char **argv) {
    
    /* initialize a device model */
    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    
    vmcu_instr_t instr;
    vmcu_disassemble_bytes(0x6a97, &instr, m328p);
    
    const VMCU_IKEY key    = instr.key;           // VMCU_IKEY_SBIW
    const VMCU_GROUP grp   = instr.group;         // VMCU_GROUP_MATH_LOGIC
    
    const uint32_t opcode  = instr.opcode;        // 0x976a (big endian)
    const uint16_t addr    = instr.addr;          // 0x0000 (undefined)

    const bool dword       = instr.dword;         // false
    const bool exec        = instr.exec;          // true
    
    vmcu_operand_t *src    = &instr.src;          // source operand
    vmcu_operand_t *dest   = &instr.dest;         // destination operand

    VMCU_OPTYPE src_type   = src->type;           // VMCU_OPTYPE_K6
    VMCU_OPTYPE dest_type  = dest->type;          // VMCU_OPTYPE_RP
    
    const uint8_t src_val  = src->k;              // 0x1a
    
    VMCU_REGISTER dest_rh  = dest->rp.high;       // VMCU_REGISTER_R29
    VMCU_REGISTER dest_rl  = dest->rp.low;        // VMCU_REGISTER_R28
    
    const bool writes_hf   = instr.writes.h_flag; // false
    const bool writes_cf   = instr.writes.c_flag; // true
    
    const bool reads_io    = instr.reads.io;      // false
    const bool reads_nf    = instr.reads.n_flag;  // false
    
    vmcu_mnemonic_t *mnem  = &instr.mnem;         // instruction mnemonic
    
    const char *base_str   = mnem->base;          // "sbiw"
    const char *dest_str   = mnem->dest;          // "r29:r28"
    const char *src_str    = mnem->src;           // "0x1a"
    const char *com_str    = mnem->comment;       // "r29:r28 <- r29:r28 - 0x1a"
    
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

#### Example of an instruction-printer function

```c
/* this snippet can be used to assemble and print an instruction */

void print_instruction(const vmcu_instr_t *instr) {

    printf("%s",  instr->mnem.base);

    if(instr->dest.type != VMCU_OPTYPE_NONE)
        printf(" %s,", instr->mnem.dest);

    if(instr->src.type != VMCU_OPTYPE_NONE)
        printf(" %s", instr->mnem.src);

    printf(" %s\n", instr->mnem.comment);
}
```

#### Printing disassembly of an intel hex file

```c
/* A possible implementation of print_instruction can be found above */

int main(const int argc, const char **argv) {
    
    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);
    
    for(int32_t i = 0; i < report->progsize; i++) {

        printf("0x%04x ", report->disassembly[i].addr);
        print_instruction(&report->disassembly[i]);
    }
        
    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

```assembly
0x004e ldi r27, 0x06 ; r27 <- 0x06
0x004f rjmp 1        ; PC <- PC + 1 + 1
0x0050 st X+, r1     ; DS[X+] <- r1
0x0051 cpi r26, 0x20 ; r26 - 0x20
0x0052 cpc r27, r18  ; r27 - r18 - CF
0x0053 brne -4       ; (ZF == 0): PC <- PC + -4 + 1
0x0054 call 0x60b    ; PC <- 0x60b
```

#### Filtering read/write access on status flags

```c
/* A possible implementation of print_instruction can be found above */

int main(const int argc, const char **argv) {

    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];
        
        if(instr->writes.c_flag == true)
            print_instruction(instr);
        
        if(instr->reads.c_flag == true)
            print_instruction(instr);
    }
    
    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

```assembly
subi r18, 0x00     ; r18 <- r18 - 0x00
adiw r29:r28, 0x1a ; r29:r28 <- r29:r28 + 0x1a
sbci r23, 0xff     ; r23 <- r23 - 0xff - CF
cpc r19, r17       ; r19 - r17 - CF
```

#### Printing interrupt vectors and their xref-to

```c
int main(const int argc, const char **argv) {

    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);

    for(int32_t i = 0; i < report->n_vector; i++) {

        vmcu_vector_t *vect = &report->vector[i];
        vmcu_instr_t  *isr  = vect->xto->i;
        
        printf("Vector ID %d @ 0x%04x\n", vect->id, vect->addr);
        printf(" interrupt service routine at 0x%04x", isr->addr);
        printf("\n\n");
    }
    
    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

```assembly
Vector ID 16 @ 0x0020
 interrupt service routine at 0x03f5

Vector ID 17 @ 0x0022
 interrupt service routine at 0x008a

Vector ID 18 @ 0x0024
 interrupt service routine at 0x03c3

Vector ID 19 @ 0x0026
 interrupt service routine at 0x039d
```

#### Printing potential labels

```c
int main(const int argc, const char **argv) {
    
    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);
    
    for(int32_t i = 0; i < report->n_label; i++) {
        
        vmcu_label_t *lx = &report->label[i];
        
        printf("Label ID: %d, ", lx->id);
        printf("Address: 0x%04x\n", lx->addr);
    }
    
    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
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
/* A possible implementation of print_instruction can be found above */

int main(const int argc, const char **argv) {
    
    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);
    
    for(int32_t i = 0; i < report->n_label; i++) {

        vmcu_label_t *lx = &report->label[i];
        printf("0x%04x\tL%d\n\n", lx->addr, lx->id);

        for(int32_t j = 0; j < lx->n_xfrom; j++) {

            vmcu_xref_t *x = &lx->xfrom[j];
            
            printf(" xref from 0x%04x ", x->i->addr);
            print_instruction(x->i);
        }

        printf("\n");
    }
    
    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

```assembly
0x04c6  L75

 xref from 0x04a1 call +1222 ; PC <- 0x4c6
 xref from 0x0a84 call +1222 ; PC <- 0x4c6
 xref from 0x0b5c call +1222 ; PC <- 0x4c6

0x04e2  L76

 xref from 0x05d4 rjmp -243  ; PC <- PC - 0xf3 + 1

0x05d0  L77

 xref from 0x04e1 rjmp +238  ; PC <- PC + 0xee + 1
```

#### Printing xrefs of special function registers 

```c
/* A possible implementation of print_instruction can be found above */

int main(const int argc, const char **argv) {

    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);

    for(int32_t i = 0; i < report->n_sfr; i++) {

        vmcu_sfr_t *sfr = &report->sfr[i];
        printf("SFR ID: %d\n\n", sfr->id);

        for(int32_t j = 0; j < sfr->n_xfrom; j++) {

            vmcu_xref_t *x = &sfr->xfrom[j];

            printf(" xref from 0x%04x ", x->i->addr);
            print_instruction(x->i);
        }

        printf("\n");
    }

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

```assembly
SFR ID: 17
       
 xref from 0x00f4 sbi 0x1f, 2     ; IO[1f, 2] <- 0x01
 xref from 0x00f5 sbi 0x1f, 1     ; IO[1f, 1] <- 0x01
 
SFR ID: 50

 xref from 0x004c sts 0x006e, r1  ; DATA[0x6e] <- R1
 xref from 0x0051 lds r24, 0x006e ; R24 <- DATA[0x6e]
 xref from 0x0054 sts 0x006e, r24 ; DATA[0x6e] <- R24
```

#### Discovering strings in binary

```c
int main(const int argc, const char **argv) {

    /* ignoring checks for this example */
    vmcu_model_t  *m328p  = vmcu_model_ctor(VMCU_DEVICE_M328P); 
    vmcu_report_t *report = vmcu_analyze_ihex("file.hex", m328p);

    for(int32_t i = 0; i < report->n_string; i++) {

        vmcu_string_t *str = &report->string[i];

        printf("Found string \"%s", str->bytes);
        printf("\" l = %d", str->length);
        printf(" @ 0x%04x\n", str->addr);
    }
    
    printf("\nTotal strings found: %d\n", report->n_string);

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);
    
    return EXIT_SUCCESS;
}
```

```console
Found string "Welcome " l = 8 @ 0x092e
Found string "[1] Login\n" l = 11 @ 0x0933
Found string "[2] Memory management\n" l = 23 @ 0x0939
Found string "Please authenticate yourself with your hardware token\n" l = 55 @ 0x0946
Found string "Please insert token. (%d characters)\n" l = 38 @ 0x0962
Found string "Token can only contain the characters [A-Z/a-z/0-9]\n" l = 53 @ 0x0975

Total strings found: 6
```

# Showcase

![mdx_debug](https://user-images.githubusercontent.com/46600932/104666434-33f9da80-56d4-11eb-882b-724b13536412.png)
<sup>A small debugger written with libvmcu</sup>

![vcd_showcase](https://user-images.githubusercontent.com/46600932/109825592-430ffa00-7c3a-11eb-9af3-26175b962ef2.png)
<sup>VCD-Trace Tool by pointbazaar</sup>

![disasm_driver](https://user-images.githubusercontent.com/46600932/122659005-6a4ff000-d173-11eb-9c11-bb161de24d5d.png)
<sup>An example of a small disassembler</sup>
# How libvmcu works

### Device Models

A device model is an abstraction over a microcontroller type. It contains MCU specific
data, like memory sections and layouts.

Each implementation of a microcontroller has a device loader which fills the 
device model with data. The device model is then used to supply the
analyzer pipeline with all the relevant data it needs.

### Analyzer Pipeline

**Stage 0:** The very first stage is the decoder. The decoder tries to decode the given Hex File.

**Stage 1:** Once the binary has been decoded successfully, the data will be sent to the annotator. This stage
annotates instructions by adding additional information about the instruction itself, like groups and
explicit/implicit read/write access.

**Stage 2:** The decomposer takes care of opcodes and tries to extract and classify their operands.

**Stage 3:** In this stage, the disassembler receives the result of the previous stage in order to generate 
mnemonics and some comments.

**Stage 4:** Now the analyzer comes into play. The analyzer takes all the data from the previous three steps 
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

Let's say, we have a file called prog.c on top level of this repository 
and we want to link it with libvmcu:

#### Include libvmcu headers
```c
/* prog.c */

#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

int main(void) {
    
    /* do something */
    return 0;
}
```

#### Build libvmcu
```console
You@Terminal:~$ cd build-release/
You@Terminal:~$ make -f build.mk
```

#### Build driver object
```console
You@Terminal:~$ gcc -Iengine/include/libvmcu/ -c prog.c -o prog.o
```

#### Link with libvmcu (do not forget -lm)
```console
You@Terminal:~$ gcc -o prog prog.o -Lbuild-release/ -lvmcu -lm
```

That's it. If you face issues, take look at some examples in the driver/ directory.

# Supported Microcontroller

libvmcu tries to support as many AVR types as possible for static analysis. The
dynamic analysis is currently only planned for the ATmega328 family but may be extended
in the future.

It should be pretty easy to add new microcontrollers to the static analysis. For more information
take a look at engine/*/arch/

#### Supported MCUs for static analysis

- [ ] AVR Device Core
  - [ ] ATtiny15
  - [ ] ...
  
- [ ] AVRe Device Core
  - [ ] ATtiny1634
  - [ ] ...
  
- [ ] AVRe+ Device Core
  - [x] ATmega328(P)
  - [ ] ...
  
- [ ] AVRxm Device Core
  - [ ] ATxmega128A1
  - [ ] ...
  
- [ ] AVRxt Device Core
  - [ ] ATtiny827
  - [ ] ...
  
- [ ] AVRrc Device Core
  - [ ] ATtiny10
  - [ ] ...

#### Supported MCUs for dynamic analysis

- [x] ATmega328(P)
- [ ] ATmega168
- [ ] ATmega88
- [ ] ATmega48

# Static Analysis

- [x] Disassembler
- [x] Cross references (xref-from, xref-to)
- [ ] analyzer flags

- [x] Decompose and classify instructions
  - [x] instruction groups
  - [x] operands and operand types

- [x] Analyzer for AVR binaries
   - [x] Label analysis
   - [x] Vector analysis  
   - [ ] Function analysis
   - [ ] ISR analysis
   - [x] SFR analysis
   - [ ] Cycle analysis
   - [x] String analysis
     - [x] ASCII
     - [ ] UTF16
   - [ ] ...

- [ ] Format Reader
    - [x] intel hex
    - [ ] motorola hex
    - [ ] bin
    - [ ] elf 

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

![vdoc](https://user-images.githubusercontent.com/46600932/123187328-3db11680-d49a-11eb-8482-ea5fb7cc3014.png)

By the time of writing this the documentation is still in development. The (incomplete) documentation can be found at https://github.com/Milo-D/libvmcu-Virtual-MCU-Library/wiki 

If you are missing information and don't want to wait for the wiki, the libvmcu header files are pretty well documented, too.
