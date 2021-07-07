/* Controlflow Graph Driver for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

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

struct CFGBlock;
struct CFGBlock {
	uint32_t index;

	vmcu_cfg_node_t** cfg_nodes;
	uint32_t count_cfg_nodes;
	
	struct CFGBlock* t;
	struct CFGBlock* f;
};

/* Forward Declaration of static Functions */

static void print_instruction(vmcu_instr_t *instr);
static void print_instruction_details(vmcu_instr_t *instr);
static void print_colored_base(const char *basestr, VMCU_GROUP group);
static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype);
static void add_padding(const size_t length, const size_t max);
static void cleanup(void);

static struct CFGBlock** obtain_blocks(vmcu_report_t* report, uint32_t* size);

static void print_block(struct CFGBlock* block, FILE* file);

static uint32_t count_color(uint32_t* colors, uint32_t n, uint32_t color);

static vmcu_instr_t* find_next_instr(vmcu_report_t* report, int32_t addr);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./cfg <hexfile>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t* m328p   = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t* report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;
    
    FILE* file = fopen("graph.dot", "w");
    
    fprintf(file, "digraph {\n");
    fprintf(file, "bgcolor=\"#003949\";");
    fprintf(file, "node [shape=box, style=filled, fillcolor=\"#6D6D6D\", fontname=\"monospace\"];\n");
    
    vmcu_cfg_t *cfg = report->cfg;
    uint32_t nblocks = 0;
    struct CFGBlock** blocks = obtain_blocks(report, &nblocks);


    for(int32_t i = 0; i < nblocks; i++) {
	print_block(blocks[i], file);
    }
    
    for(int32_t i = 0; i < nblocks; i++) {
    	struct CFGBlock* block = blocks[i];
    	
    	//:n north corner
    	//:se south east corner
    	
    	if(block->t != NULL){
    		if(block->t == block){
    			//pointing to same block
    			fprintf(file, "node_%d:se -> node_%d:ne;\n", block->index, block->t->index);
	    	}else{
    	
	    		fprintf(file, "node_%d:s -> node_%d:n;\n", block->index, block->t->index);
    		}
    	}
    	if(block->f != NULL){
    		fprintf(file, "node_%d:s -> node_%d:n;\n", block->index, block->f->index);
    	}
    }
    
    fprintf(file, "\n }");
    
    fclose(file);
    
    system("dot -Tpng graph.dot > graph.png");
    
    for(int32_t i = 0; i < nblocks; i++) {
    	free(blocks[i]->cfg_nodes);
	free(blocks[i]);
    }
    free(blocks);
    
    if(report != NULL)
        vmcu_report_dtor(report);

    if(m328p != NULL)
        vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_block(struct CFGBlock* block, FILE* file){
	
	assert(block != NULL);
	
	fprintf(file, "node_%d", block->index);
	
	//fprintf(file, "[label=\"%s\"];", str);
	fprintf(file, "[label=\"");
	
	char str[1000];
	strcpy(str, "");
	
	for(uint32_t i = 0; i < block->count_cfg_nodes; i++){
	
		vmcu_cfg_node_t *node = block->cfg_nodes[i];
		
		vmcu_instr_t *instr = node->xto.i;
		
		vmcu_mnemonic_t *mnem = &instr->mnem;
		
		fprintf(file, "0x%04x %s ", instr->addr, mnem->base);
		
		if(instr->dest.type != VMCU_OPTYPE_NONE)
			fprintf(file, " %s, ", instr->mnem.dest);

		if(instr->src.type != VMCU_OPTYPE_NONE)
			fprintf(file, " %s", instr->mnem.src);
			
		//fprintf(file, "\n");
		fprintf(file, "\\l");
	}
	
	fprintf(file, "\"];");
	fprintf(file, "\n");
}

static vmcu_instr_t* find_next_instr(vmcu_report_t* report, int32_t addr){

	vmcu_instr_t *instr_next = NULL;
	vmcu_instr_t* disassembly = report->disassembly;
		
	for(int32_t k = 0; k < report->progsize; k++){
	
		if(disassembly[k].addr == addr+1){
			instr_next = &disassembly[k];
		}
	}
	if(instr_next == NULL){
		for(int32_t k = 0; k < report->progsize; k++){	
			if(disassembly[k].addr == addr+2){
				instr_next = &disassembly[k];
			}
		}
	}
	
	return instr_next;
}

static struct CFGBlock** obtain_blocks(vmcu_report_t* report, uint32_t* size){
	vmcu_cfg_t* cfg = report->cfg;
	vmcu_cfg_node_t* nodes = cfg->node;
	uint32_t count_nodes = cfg->used;
	
	vmcu_instr_t* disassembly = report->disassembly;

	//go over the nodes, number them 
	//with the same number, until they
	//have more than one branch, or 
	//have no true branch
	
	uint32_t* colors = malloc(sizeof(uint32_t)*count_nodes);
	uint32_t  color  = 0;
	for(uint32_t i = 0; i < count_nodes; i++){
		
		vmcu_cfg_node_t *node = &nodes[i];
		
		colors[i] = color;
		
		vmcu_instr_t *instr = node->xto.i;
		
		vmcu_instr_t *instr_next = find_next_instr(report, instr->addr);
		
		if(instr_next == NULL){ color++; continue; }
		//assert(instr_next != NULL);
		
		//if there is only one branch
		//and that is going to the next instruction
		//we are still within the same block
		if(node->t != NULL && node->f == NULL){
			if(node->t->xto.i == instr_next) {continue;}
		}
		
		color++;
	}
	
	//now we know the number of blocks
	struct CFGBlock** blocks = malloc(sizeof(void*)*color);
	*size = color;
	
	for(uint32_t mycolor = 0; mycolor < color; mycolor++){
		
		struct CFGBlock* block = malloc(sizeof(struct CFGBlock));
		block->index = mycolor;
		block->t     = NULL;
		block->f     = NULL;
		
		block->count_cfg_nodes = count_color(colors, count_nodes, mycolor);
		block->cfg_nodes = 
			malloc(sizeof(void*)*(block->count_cfg_nodes));
		
		//go over 'nodes' to find the instrs with our color
		//and collect them into our block
		uint32_t k = 0;
		for(uint32_t i = 0; i < count_nodes; i++){
			vmcu_cfg_node_t *node = &nodes[i];
			if(colors[i] == mycolor){
				block->cfg_nodes[k++] = node;			
			}
		}
		
		assert(k == block->count_cfg_nodes);
		
		blocks[mycolor] = block;
	}
	
	//now we have to connect the blocks to each other
	for(uint32_t i = 0; i < color; i++){
	
		struct CFGBlock* block = blocks[i];

		//find the last instr in that block
		vmcu_cfg_node_t *last = 
			block->cfg_nodes[block->count_cfg_nodes-1];
		
		assert(last != NULL);
		//find the CFGBlocks that these instructions
		//are part of
		
		if(last->t == NULL){ goto f; }
		
		for(uint32_t k1 = 0; k1 < count_nodes; k1++){
			vmcu_cfg_node_t *node = &nodes[k1];
			if(last->t == node){
				block->t = blocks[colors[k1]];
			}
		}
	
		
		f:
		
		if(last->f == NULL){ goto end; }
		
		for(uint32_t k1 = 0; k1 < count_nodes; k1++){
			vmcu_cfg_node_t *node = &nodes[k1];
			if(last->f == node){
				block->f = blocks[colors[k1]];
			}
		}
		
		end: continue;
	}
	
	free(colors);
	return blocks;
}

static uint32_t count_color(uint32_t* colors, uint32_t n, uint32_t color){
	uint32_t res = 0;
	for(uint32_t i = 0; i < n; i++){
		if(colors[i] == color){ res++; }
	}
	return res;
}

