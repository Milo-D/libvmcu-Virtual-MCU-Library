#include <stdbool.h>
#define HEAP_START (void*)0x64
#define HEAP_END (void*)0xE0

#define INDEX_FREE (void**)0x60 //HEAP START

void mem_init(){

	*((void**)INDEX_FREE) = (void*)HEAP_START;
}

void* malloc(int n){
	
	void* index_free = *((void**)INDEX_FREE);
	
	void* index_new = index_free + n;
	
	if(index_new <= HEAP_END){
	
		void* index_old = index_free;
		index_free = index_new;
		
		*((void**)INDEX_FREE) = index_free;
		
		return index_old;
	}
	
	return (void*)0;
}

void free(void* ptr){}
struct Node;
struct Node {
	int value;
	struct Node* left;
	struct Node* right;
	bool hasLeft;
	bool hasRight;
};
struct Node* newNode();
void delNode();
struct Node* copyNode();
struct Node* newNode(){
	struct Node* res = malloc(sizeof(struct Node));
	return res;
}
void delNode(struct Node* instance){
	free(instance);
}
struct Node* copyNode(struct Node* instance){
	struct Node* res = malloc(sizeof(struct Node));
	res->value = instance->value;
	res->left = instance->left;
	res->right = instance->right;
	res->hasLeft = instance->hasLeft;
	res->hasRight = instance->hasRight;
	return res;
}
bool check7(struct Node* node);
struct Node* makeNode(int v);
struct Node* makeBinTree();
bool traverse(struct Node* tree, bool(*checker) (struct Node*));
int main();
bool check7(struct Node* node){
	return node->value == 7;
}
struct Node* makeNode(int v){
	struct Node* res = newNode();
	res->value = v;
	res->hasLeft = false;
	res->hasRight = false;
	return res;
}
struct Node* makeBinTree(){
	struct Node* root = makeNode(6);
	struct Node* n7 = makeNode(7);
	root->right = n7;
	root->hasRight = true;
	n7->left = makeNode(8);
	n7->hasLeft = true;
	n7->right = makeNode(9);
	n7->hasRight = true;
	return root;
}
bool traverse(struct Node* tree, bool(*checker) (struct Node*)){
	if (checker(tree)){
		return true;
	}

	bool found = false;
	if (tree->hasLeft){
		found = traverse(tree->left, checker);
	}

	if (tree->hasRight){
		found = found || traverse(tree->right, checker);
	}

	return found;
}
int main(){
	struct Node* bintree = makeBinTree();
	bool found = traverse(bintree, check7);
	int k = 0;
	if (found){
		k = 11;
	}

	return 0;
}
