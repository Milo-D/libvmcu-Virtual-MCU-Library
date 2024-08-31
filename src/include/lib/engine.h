#ifndef VMCU_ENGINE_H
#define VMCU_ENGINE_H

#include <stdbool.h>
#include "rc.h"

/*
 * Endianness.
 * */
typedef enum {

    VMCU_ENDIAN_ENUM_START = -1, // start of enum
    VMCU_ENDIAN_BIG,             // big endian
    VMCU_ENDIAN_LITTLE,          // little endian
    VMCU_ENDIAN_ENUM_END         // end of enum

} vmcu_endian_t;

/*
 * Supported binary file formats
 * */
typedef enum {

    VMCU_FMT_ENUM_START = -1, // start of enum
    VMCU_FMT_IHEX,            // intel hex format
    VMCU_FMT_SREC,            // srec or motorola hex format
    VMCU_FMT_ENUM_END         // end of enum

} vmcu_fmt_t;

/*
 * AVR instruction set versions
 * */
typedef enum {

    VMCU_CORE_ENUM_START = -1, // start of enum
    VMCU_CORE_AVR,             // original instruction set (1995)
    VMCU_CORE_AVRe,            // extended AVR instruction set (movw, lpm)
    VMCU_CORE_AVRep,           // extended AVRe instruction set (xmulxx, eicall, ...)
    VMCU_CORE_AVRxm,           // extended AVRe+ instruction set (rmw, des, ...)
    VMCU_CORE_AVRxt,           // combination of AVRe+ and AVRxm
    VMCU_CORE_AVRrc,           // AVR reduced core (only r16-r31)
    VMCU_CORE_ENUM_END         // end of enum

} vmcu_core_t;

/*
 * Reader configuration.
 * */
typedef struct vmcu_reader_config {

    vmcu_fmt_t    file_format; // the format to parse
    vmcu_endian_t endianness;  // endianness of binary file
    bool          sort;        // sort the resulting words by address? (ascending)

} vmcu_reader_config_t;

/*
 * Disassembler configuration.
 * */
typedef struct vmcu_disassembler_config {

    bool words_mutable; // allows the disassembler to alter the words buffer

} vmcu_disassembler_config_t;

/*
 * Assembler configuration.
 * */
typedef struct vmcu_assembler_config {

    bool expect_operand_structs; // assembler interface expects operand structs as input

} vmcu_assembler_config_t;

/*
 * Callback to custom memory allocation routine
 * */
typedef void* (*vmcu_cb_alloc_t)(size_t);

/*
 * Callback to custom memory de-allocation routine
 * */
typedef void (*vmcu_cb_free_t)(void*);

/*
 * Callback namespace
 * */
typedef struct vmcu_callbacks {

    vmcu_cb_alloc_t alloc; // allocation routine
    vmcu_cb_free_t  free;  // de-allocation routine

} vmcu_callbacks_t;

/*
 * VMCU engine object, holding
 * the current engine configuration
 * for all VMCU submodules.
 * */
typedef struct vmcu_engine {

    vmcu_rc_t                  rc;              // most recent return code (similar to errno)
    vmcu_core_t                instruction_set; // instruction set version
    vmcu_callbacks_t           cb;              // VMCU callbacks
    vmcu_reader_config_t       reader;          // configuration for reader
    vmcu_disassembler_config_t disassembler;    // configuration for disassembler
    vmcu_assembler_config_t    assembler;       // configuration for assembler

} vmcu_engine_t;

/*
 * VMCU engine in TLS (thread local storage)
 * */
extern __thread vmcu_engine_t vmcu;

/*
 * vmcu_engine_reset - reset engine to default values
 * */
void vmcu_engine_reset(void);

#endif
