#include <stdbool.h>

char* password = "secretpassword";
char* input    = "______________";

int main(void) {
	
	bool match = true;
	int l = 15;
	
	for(int i = 0; i < l; i++) {
	
		if(input[i] != password[i]) { 

			match = false;
			break; 
		}
	}

	return match;
}

