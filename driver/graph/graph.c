/* Controlflow Graph Driver for libvmcu */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

#define COLOR_NONE 0

struct CFGBlock;
struct CFGBlock {
	uint32_t index;

	vmcu_cfg_node_t** cfg_nodes;
	uint32_t count_cfg_nodes;
	
	struct CFGBlock* t;
	struct CFGBlock* f;
};

static struct CFGBlock** obtain_blocks(vmcu_report_t* report, uint32_t* size);
static void connect_blocks(
	struct CFGBlock** blocks, 
	uint32_t nblocks,
	
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size
);

static uint32_t* color_cfg_nodes(
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size,
	
	uint32_t* color,
	
	vmcu_cfg_node_t* nodes,
	uint32_t count_nodes
);

static struct CFGBlock** collect_blocks(
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size,
	
	uint32_t* colors, 
	uint32_t color,
	uint32_t count_nodes, 
	uint32_t* count_blocks
);

static void print_block(struct CFGBlock* block, FILE* file);
static void print_block_connections(struct CFGBlock* block, FILE* file);
static uint32_t count_color(uint32_t* colors, uint32_t n, uint32_t color);

static uint32_t branch_from(vmcu_cfg_node_t* node);
static uint32_t branch_here(vmcu_cfg_node_t* nodes, uint32_t count_nodes, vmcu_cfg_node_t* node);

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./cfg <binary>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t* m328p   = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t* report = vmcu_analyze_file(argv[1], m328p);

    if(report == NULL)
        return EXIT_FAILURE;
    
    FILE* file = fopen("graph.dot", "w");
    
    fprintf(file, "digraph {\n");
    fprintf(file, "bgcolor=\"#003949\";");
    fprintf(file, "node [shape=box, style=filled, fillcolor=\"#6D6D6D\", fontname=\"monospace\"];\n");
    
    uint32_t nblocks = 0;
    struct CFGBlock** blocks = obtain_blocks(report, &nblocks);

    for(uint32_t i = 0; i < nblocks; i++) {
	    print_block(blocks[i], file);
    }
    
    for(uint32_t i = 0; i < nblocks; i++) {
    	print_block_connections(blocks[i], file);
    }
    
    fprintf(file, "\n }");
    
    fclose(file);
    
    system("dot -Tpng graph.dot > graph.png");
    
    for(uint32_t i = 0; i < nblocks; i++) {
    	free(blocks[i]->cfg_nodes);
	free(blocks[i]);
    }
    free(blocks);
    
    if(report != NULL) vmcu_report_dtor(report);
    if(m328p != NULL) vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static void print_block_connections(struct CFGBlock* block, FILE* file){

	//:n north corner
    	//:se south east corner
    	//char* color_green = "[color=\"green\"]";
    	//char* color_red   = "[color=\"red\"]";
    	char* color_black = "[color=\"black\"]";
    	
    	char* color = color_black;
    	
    	if(block->t != NULL){
    		//color = color_green;
    		
    		//if(block->f == NULL){ color = color_black; }
    	
    		if(block->t == block){
    			//pointing to same block
    			fprintf(file, "node_%d:se -> node_%d:ne  %s;\n", block->index, block->t->index, color);
	    	}else{
    	
	    		fprintf(file, "node_%d:s -> node_%d:n %s;\n", block->index, block->t->index, color);
    		}
    	}
    	if(block->f != NULL){
    		//color = color_red;
    	
    		fprintf(file, "node_%d:s -> node_%d:n %s;\n", block->index, block->f->index, color);
    	}
}

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

static uint32_t branch_here(vmcu_cfg_node_t* nodes, uint32_t count_nodes, vmcu_cfg_node_t* node){
	assert(node != NULL);
	uint32_t branch_here = 0;
	for(uint32_t j = 0; j < count_nodes; j++){
	
		vmcu_cfg_node_t *other = &nodes[j];
		
		if(other->t == node || other->f == node){
			branch_here++;			
		}
	}
	return branch_here;
}

static uint32_t branch_from(vmcu_cfg_node_t* node){
	assert(node != NULL);
	uint32_t res = 0;
	if(node->t != NULL){ res++; }
	if(node->f != NULL){ res++; }
	return res;
}

static uint32_t get_highest_addr(vmcu_report_t* report){
	vmcu_instr_t* disassembly = report->disassembly;
	uint32_t highest_addr = 0;
	for(uint32_t i = 0; i < report->progsize; i++){
		if(disassembly[i].addr > highest_addr){
			highest_addr = disassembly[i].addr;
		}
	}
	return highest_addr;
}

static vmcu_cfg_node_t** make_cfg_arr(
	uint32_t highest_addr, 
	vmcu_cfg_node_t* nodes,
	uint32_t count_nodes
){
	
	vmcu_cfg_node_t** res = malloc(sizeof(void*)*(highest_addr+1));
	memset(res, 0, sizeof(void*)*(highest_addr+1));
	
	for(uint32_t i = 0; i < count_nodes; i++){
		vmcu_cfg_node_t* node = &nodes[i];
		res[node->xto.i->addr] = node;
	}
	return res;
}

static struct CFGBlock** obtain_blocks(vmcu_report_t* report, uint32_t* size){

	vmcu_cfg_t* cfg           = report->cfg;
	vmcu_cfg_node_t* nodes    = cfg->node;
	uint32_t count_nodes      = cfg->used;
	
	uint32_t highest_addr = get_highest_addr(report);
	
	//indexable with instr.addr
	vmcu_cfg_node_t** cfg_arr = make_cfg_arr(highest_addr, nodes, count_nodes);
	
	uint32_t cfg_arr_size = highest_addr+1;
	
	//create our blocks (colors)
	//COLOR_NONE means no cfg node at that addr
	uint32_t color = COLOR_NONE + 1;
	uint32_t* colors = color_cfg_nodes(
		cfg_arr, 
		cfg_arr_size, 
		&color, 
		nodes,
		count_nodes
	);
	
	struct CFGBlock** blocks = collect_blocks(
		cfg_arr, 
		cfg_arr_size,
		colors, 
		color,
		count_nodes, 
		size
	);
	
	connect_blocks(blocks, *size, cfg_arr, cfg_arr_size);
	
	free(cfg_arr);
	free(colors);
	
	return blocks;
}

static struct CFGBlock* collect_block(
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size,
	
	uint32_t* colors, 
	uint32_t mycolor
){
	//collect cfg_node s of a color into a block
	//collect all nodes of that color into a block
	struct CFGBlock* b = malloc(sizeof(struct CFGBlock));
	b->index = mycolor;
	b->t = NULL;
	b->f = NULL;
	b->count_cfg_nodes = count_color(colors, cfg_arr_size, mycolor);
	
	assert(b->count_cfg_nodes > 0);
	
	b->cfg_nodes = malloc(sizeof(void*)*b->count_cfg_nodes);
	
	uint32_t index = 0;
	for(uint32_t k = 0; k < cfg_arr_size; k++){
	
		if(colors[k] != mycolor){ continue; }
		
		b->cfg_nodes[index++] = cfg_arr[k];
	}
	
	return b;
}

static struct CFGBlock** collect_blocks(
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size,
	
	uint32_t* colors, 
	uint32_t color,
	uint32_t count_nodes, 
	uint32_t* count_blocks
){
	struct CFGBlock** blocks = malloc(sizeof(void*)*count_nodes);

	for(uint32_t mycolor = COLOR_NONE+1; mycolor < color; mycolor++){
	
		if(count_color(colors, cfg_arr_size, mycolor) == 0){ continue; }
		
		struct CFGBlock* b = collect_block(cfg_arr, cfg_arr_size, colors, mycolor);
		
		blocks[*count_blocks] = b;
		*count_blocks += 1;
	}
	
	return blocks;
}

static vmcu_instr_t* get_next_instr(
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size,
	
	vmcu_instr_t* instr
){
	vmcu_instr_t* next = NULL;
	for(
		uint32_t i = instr->addr+1; 
		i <= (instr->addr+2) && (i < cfg_arr_size); 
		i++
	){
		
		vmcu_cfg_node_t* node = cfg_arr[i];
		if(node == NULL){ continue; }
		
		return node->xto.i;
	}
	return next;
}

static uint32_t* color_cfg_nodes(
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size,
	
	uint32_t* out_color,
	
	vmcu_cfg_node_t* nodes,
	uint32_t count_nodes
){
	uint32_t color = *out_color;
	
 	uint32_t* colors = malloc(sizeof(uint32_t)*cfg_arr_size);
 	memset(colors, COLOR_NONE, sizeof(uint32_t)*cfg_arr_size);

	uint32_t i = 0;
	while(i < cfg_arr_size){
		
		vmcu_cfg_node_t* node = cfg_arr[i];
		
		//no instr at that addr
		if(node == NULL){ 
			colors[i] = COLOR_NONE;
			i++; 
			continue; 
		}
		
		uint32_t in_edges  = branch_here(nodes, count_nodes, node);
		uint32_t out_edges = branch_from(node);
		
		vmcu_instr_t* instr = node->xto.i;
		vmcu_instr_t* next_instr = 
			get_next_instr(cfg_arr, cfg_arr_size, instr);
			
		bool tb_next = false;
		
		if((node->t != NULL) && (next_instr != NULL)){
			tb_next = node->t->xto.i == next_instr;
		}
		
		if(in_edges > 1){
		
			//start a new block
			color++;
			colors[i] = color;
			
			if(!tb_next){ color++; }
			
		}else if((out_edges != 1) || (!tb_next)){
			//next node is for the next block
			
			//add current node to block
			//and start next block
			colors[i] = color;
			color++;
		}else{
			//add current node to block
			colors[i] = color;
		}
		i++;
	}
	
	*out_color = color;
	
	return colors;
}

static struct CFGBlock* find_block_with_first_node(
	struct CFGBlock** blocks, 
	uint32_t nblocks, 
	
	vmcu_cfg_node_t* node
){
	for(uint32_t k = 0; k < nblocks; k++){
		struct CFGBlock* block = blocks[k];
		
		assert(block->count_cfg_nodes > 0);
		
		if(block->cfg_nodes[0] == node){ return block; }
	}
	return NULL;
}

static void connect_block(
	struct CFGBlock** blocks, 
	uint32_t nblocks,
	
	struct CFGBlock* block,
	
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size
){

	vmcu_cfg_node_t *last = block->cfg_nodes[block->count_cfg_nodes-1];
		
	//find the CFGBlocks that these instructions are part of
	if(last->t == NULL){ goto f; }
	
	for(uint32_t k1 = 0; k1 < cfg_arr_size; k1++){
	
		vmcu_cfg_node_t *node = cfg_arr[k1];
		
		if(node == NULL){ continue; }
		
		if(last->t == node){
			block->t = find_block_with_first_node(blocks, nblocks, node);
		}
	}
	
	f:
	
	if(last->f == NULL){ goto end; }
	
	for(uint32_t k1 = 0; k1 < cfg_arr_size; k1++){
	
		vmcu_cfg_node_t *node = cfg_arr[k1];
		
		if(node == NULL){ continue; }
		
		if(last->f == node){
			block->f = find_block_with_first_node(blocks, nblocks, node);
		}
	}
	
	end:
	return;
}

static void connect_blocks(

	struct CFGBlock** blocks, 
	uint32_t nblocks,
	
	vmcu_cfg_node_t** cfg_arr,
	uint32_t cfg_arr_size
){	
	//now we have to connect the blocks to each other
	for(uint32_t i = 0; i < nblocks; i++){
	
		struct CFGBlock* block = blocks[i];

		connect_block(blocks, nblocks, block, cfg_arr, cfg_arr_size);
	}
}

static uint32_t count_color(uint32_t* colors, uint32_t n, uint32_t color){
	uint32_t res = 0;
	for(uint32_t i = 0; i < n; i++){
		if(colors[i] == color){ res++; }
	}
	return res;
}

