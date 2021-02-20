/* String-Manipulators Implementation */

// C Header
#include <stdlib.h>
#include <string.h>

// Project Header (engine)
#include "engine/include/misc/stringmanip.h"

int strpos(const char *str, const char *sub) {

    char *c = NULL;

    if((c = strstr(str, sub)) == NULL)
        return -1;

    return (c - str);
}

char* substr(const char *str, const int from, const int to) {

    char *sub = NULL;
    const int diff = (to - from);

    if(diff < 0)
        return NULL;

    if((sub = malloc((diff + 2) * sizeof(char))) == NULL)
        return NULL;

    strncpy(sub, (str + from), diff + 1);
    sub[diff + 1] = '\0';

    return sub;
}

int htoi(const char *input) {

    unsigned int len = strlen(input);
    int dec = 0; int base = 1; int end = 0;

    if(len > 1) {

        char sub[3] = { input[0], input[1], '\0' };

        if(strcmp(sub, "0x") == 0)
            end += 2;
    }

    for(int i = len - 1; i >= end; i--) {

        if(input[i] >= '0' && input[i] <= '9') {

            dec += (input[i] - '0') * base;
            base *= 16;
                        
        } else if(input[i] >= 'A' && input[i] <= 'F') {

            dec += (10 + input[i] - 'A') * base;
            base *= 16;
            
        } else if(input[i] >= 'a' && input[i] <= 'f') {

            dec += (10 + input[i] - 'a') * base;
            base *= 16;

        } else {

            return -1;
        }
    }

    return dec;
}

int swap_bytes(char *buffer, const char *bytes, const int pos) {

    const int order[4] = { 2, 3, 0, 1 };

    if((8 + (pos * 4) + 3) >= strlen(bytes))
        return -1;

    for(int i = 0; i < 4; i++) {

        const int index = (8 + (pos * 4) + order[i]);
        buffer[i] = bytes[index];
    }

    buffer[4] = '\0';

    return 0;
}
