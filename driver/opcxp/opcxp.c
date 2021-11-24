/* Opcode Explorer for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

// libvmcu
#include "libvmcu_analyzer.h"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define mnemlen(mnem) strlen(mnem->base) \
                    + strlen(mnem->src)  \
                    + strlen(mnem->dest)

/* Forward Declaration of static Functions */

static bool filter(const vmcu_instr_t* instr);

static uint32_t swp_endian(uint32_t opc);

static void print_instruction(vmcu_instr_t *instr, vmcu_model_t *model);
static void print_instruction_details(vmcu_instr_t *instr);
static void print_colored_base(const char *basestr, VMCU_GROUP group);
static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype);
static void add_padding(const size_t length, const size_t max);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc > 4) {
        //-g stands for grouping, not printing the same instruction
        //twice if a variant was already found
        printf("Usage: ./opcxp [opcode bound] [-g] [-s]\n");
        
        printf(" -g groups instructions, not printing the same instruction twice\n");
        printf(" -s stands for summary, omitting the printing of instructions\n");
        return EXIT_FAILURE;
    }
    
    bool summary   = false;
    bool group     = false;
    uint32_t bound = 0xffffffff;
    
    for(uint32_t i = 1; i < argc; i++){
        
        const char* arg = argv[i];
        
        if(arg[0] == '-'){
            if(strcmp(arg, "-g") == 0) group   = true;
            if(strcmp(arg, "-s") == 0) summary = true;
        }else{
            bound = (uint32_t)strtol(arg, NULL, 16);
        }
    }
    
    setvbuf(stdout, NULL, _IOFBF, BUFSIZ);

    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    
    if(m328p == NULL)
    	return EXIT_FAILURE;
    
    const uint32_t COUNT_INSTR_MAX = (VMCU_IKEY_CLN+1);
    bool* hitmap = malloc(sizeof(bool)*COUNT_INSTR_MAX);
    memset(hitmap, false, COUNT_INSTR_MAX * sizeof(bool));
    
    uint32_t count = 0;
    uint32_t instruction_count = 0;
    uint32_t count_legal_opcodes = 0;

    for(uint64_t opcode = 0; opcode <= bound; opcode++){

    	vmcu_instr_t instr;
    
        //decompose omits menmonic and comment stuff which we dont
        //need for filtering
    	if(vmcu_decompose_bytes(opcode, &instr, m328p) == -1)
    	    continue;
    	    
    	if(instr.key == VMCU_IKEY_DATA)
    	    continue;

	count_legal_opcodes++;

	if(filter(&instr)){
	    if( (!group) || !hitmap[instr.key] ){
	    	if(!summary)
	            print_instruction(&instr, m328p);
            }
	        
	    hitmap[instr.key] = true;
	    count++;
	}
    }
    
    for(uint32_t i = 0; i < COUNT_INSTR_MAX; i++){
        instruction_count += hitmap[i];
    }
    
    float percentage = 0.0;
    
    if(count_legal_opcodes > 0)
        percentage = (count*1.0 / count_legal_opcodes) * 100;
    
    printf("found %"PRIu32" matching opcodes\n", count);
    printf("	which is %.2f%% of the inspected instructions\n", percentage);
    printf("found %"PRIu32" matching instructions\n", instruction_count);

    free(hitmap);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static bool filter(const vmcu_instr_t* instr){

    //return instr->writes.stack && instr->reads.pc;
    
    //return instr->group == VMCU_GROUP_FLOW;
    //return instr->group == VMCU_GROUP_BIT;
    //return instr->group == VMCU_GROUP_MATH_LOGIC;
    //return instr->group == VMCU_GROUP_TRANSFER;
    //return instr->group == VMCU_GROUP_SYS_CTRL;
    
    return instr->group != VMCU_GROUP_NONE;
    
    //return instr->group == VMCU_GROUP_NONE;
}

static uint32_t swp_endian(uint32_t opc){

    const uint16_t opcl = (opc & 0x0000ffff);
    const uint16_t swpl = (opcl >> 8) | (opcl << 8);

    const uint16_t opch = ((opc & 0xffff0000) >> 16);
    const uint16_t swph = (opch >> 8) | (opch << 8);
    
    return (swph << 16) | swpl;
}

static void print_instruction(vmcu_instr_t *instr, vmcu_model_t *model) {

    if(vmcu_disassemble_bytes(swp_endian(instr->opcode), instr, model) == -1)
    	return;

    vmcu_mnemonic_t *mnem = &instr->mnem;

    print_instruction_details(instr);
    print_colored_base(mnem->base, instr->group);

    size_t sz = 0;

    if(instr->dest.type != VMCU_OPTYPE_NONE) {

        print_colored_operands(mnem->dest, instr->dest.type);
        sz += printf(", ");
    }

    if(instr->src.type != VMCU_OPTYPE_NONE) {

        print_colored_operands(mnem->src, instr->src.type);
        sz += printf(" ");
    }

    add_padding(mnemlen(mnem) + sz, 26);
    printf("%s%s%s\n", COLOR_RED, mnem->comment, COLOR_RESET);
}

static void print_instruction_details(vmcu_instr_t *instr) {


    const uint16_t opcl = (instr->opcode & 0x0000ffff);
    const uint16_t swpl = (opcl >> 8) | (opcl << 8);

    const uint16_t opch = ((instr->opcode & 0xffff0000) >> 16);
    const uint16_t swph = (opch >> 8) | (opch << 8);

    if(instr->dword == false)
        printf("%s....%s ", COLOR_GREEN, COLOR_RESET);
    else
        printf("%s%04" PRIx16 "%s ", COLOR_GREEN, swph, COLOR_RESET);

    printf("%s%04" PRIx16 "%s      ", COLOR_YELLOW, swpl, COLOR_RESET);
}

static void print_colored_base(const char *basestr, VMCU_GROUP group) {

    switch(group) {

        case VMCU_GROUP_NONE: printf("%s", COLOR_YELLOW);    break;
        case VMCU_GROUP_MATH_LOGIC: printf("%s", COLOR_RED); break;
        case VMCU_GROUP_SYS_CTRL: printf("%s", COLOR_CYAN);  break;
        case VMCU_GROUP_TRANSFER: printf("%s", COLOR_RESET); break;
        case VMCU_GROUP_FLOW: printf("%s", COLOR_GREEN);     break;
        case VMCU_GROUP_BIT: printf("%s", COLOR_MAGENTA);    break;

        default: break;
    }

    printf("%s%s ", basestr, COLOR_RESET);
}

static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype) {

    switch(optype) {

        case VMCU_OPTYPE_B:
        case VMCU_OPTYPE_K4:
        case VMCU_OPTYPE_K6:
        case VMCU_OPTYPE_K8:
        case VMCU_OPTYPE_IO5:
        case VMCU_OPTYPE_IO6:
        case VMCU_OPTYPE_D7:
        case VMCU_OPTYPE_D16:
        case VMCU_OPTYPE_P22:
        case VMCU_OPTYPE_S7:
        case VMCU_OPTYPE_S12:

            printf("%s", COLOR_BLUE);

        break;

        case VMCU_OPTYPE_R:
        case VMCU_OPTYPE_RP:
        case VMCU_OPTYPE_X:
        case VMCU_OPTYPE_Y:
        case VMCU_OPTYPE_Z:

            printf("%s", COLOR_CYAN);

        break;

        default: break;
    }

    printf("%s%s", opstr, COLOR_RESET);
}

static void add_padding(const size_t length, const size_t max) {

    if(length >= max)
        return;

    char pad[max - length + 1];

    memset(pad, ' ', (max - length));
    pad[max - length] = '\0';

    printf("%s", pad);
}

