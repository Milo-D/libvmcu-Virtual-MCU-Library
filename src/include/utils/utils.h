/* Misc utility functions for libvmcu */

#ifndef VMCU_UTILS_H
#define VMCU_UTILS_H

#include <inttypes.h>

/*
 * htou64 - convert a hex string to a 64-bit unsigned integer
 * @input   target hex string
 * @error   the location to store the error code. Caller must
 *          ensure that the pointer is valid.
 * @retval  the resulting integer
 * */
uint64_t htou64(const char* input, int* error);

#endif
