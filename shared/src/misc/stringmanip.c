/* String-Manipulators Implementation */

// C Header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

// Project Header
#include "misc/stringmanip.h"
#include "misc/memmanip.h"
#include "collections/list.h"
#include "collections/queue.h"

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

char* strxcat(const char *one, const char *two) {

    char *str;

    const int l1 = strlen(one);
    const int l2 = strlen(two);

    if((str = malloc((l1 + l2 + 1) * sizeof(char))) == NULL)
        return NULL;

    strncpy(str, one, l1);
    str[l1] = '\0';

    strncat(str, two, l2);
    str[l1 + l2] = '\0';

    return str;
}

char* strrepl(const char *str, const char *alias, const char *seq) {

    int pos = 0;

    const int len = strlen(str);
    const int seqlen = strlen(seq);

    queue_t *stream = queue_ctor();
    queue_put(stream, 1, str);

    while((pos = strpos(str, seq)) < 0) {

        char *sub = substr(str, 0, pos - 1);

        if(sub == NULL) {

            queue_dtor(stream);
            return NULL;
        }

        int p = 0;

        while(str[len - p - 1] == ' ')
            p += 1;

        char *suffix = substr(str, pos + seqlen, len - p);
        char *fill = strfill(' ', 0, p);

        queue_put(stream, 4, sub, alias, suffix, fill);
        nfree(3, sub, suffix, fill);
    }

    return queue_str(stream);
}

int fname_of(const char *path, char *buffer) {

    int pos;

    if((pos = strpos(path, ".")) < 0)
        return -1;

    char *name = substr(path, 0, pos - 1);

    if(name == NULL)
        return -1;

    strncpy(buffer, name, strlen(name));
    free(name);

    return 0;
}

int del_comment(const char *line, char *buffer) {

    int pos;

    if((pos = strpos(line, ";")) < 0)
        return -1;

    char *sub = substr(line, 0, pos - 1);

    if(sub == NULL)
        return -1;

    strncpy(buffer, sub, strlen(sub));
    free(sub);

    return 0;
}

char* strfill(const char fill, const int len, const int atl) {

    char *str = malloc((len >= atl ? 1 : atl - len + 1) * sizeof(char));

    if(len >= atl) {
        
        str[0] = '\0';
        return str;
    }

    for(int i = 0; i < (atl - len); i++)
        str[i] = fill;

    str[atl - len] = '\0';
    return str;
}

void split(const char *str, const char at, list_t *buffer) {

    int s = 0; int e = 0;

    for(int i = 0; i < strlen(str); i++) {

        if(str[i] == at && s == e) {

            s = ++e;
            continue;
        }

        if(str[i] == at) {

            char *sub = substr(str, s, e - 1);
            const size_t bytes = (strlen(sub) + 1) * sizeof(char);

            ls_push(buffer, (void*) sub, bytes);
            free(sub);

            s = e + 1;
        }

        e += 1;
    }

    if(s == e)
        return;

    char *sub = substr(str, s, e);
    const size_t bytes = (strlen(sub) + 1) * sizeof(char);

    ls_push(buffer, (void*) sub, bytes);
    free(sub);
}

int get_int(const char *str) {

    const int len = strlen(str);
    int acc = 0;

    for(int i = 0; i < len; i++) {

        const char c = ((int) str[i] - 48);

        if(c < 0 || c > 9)
            return -1;

        acc += ((int) c * pow(10, len - i - 1));
    }

    return acc;
}

char* get_str(const int num) {

    int i = 0; int k = 0;
    int n = num;

    if(num == 0) {

        char *str = malloc(2 * sizeof(char));
        str[0] = '0';
        str[1] = '\0';

        return str;
    }

    while(n > 0) {

        n /= 10;
        i += 1;
    }

    char *str = malloc((i + 1) * sizeof(char));
    n = num;

    while(n > 0) {

        str[i - k++ - 1] = (n % 10) + 48;
        n /= 10;
    }

    str[i] = '\0';
    return str;
}

char* nformat(uint64_t n) {
    
    int size = 0;
    uint64_t k = n;
    
    while(k > 0) {
    
        k /= 1000u;
        size += 1;
    }
    
    char **str = malloc(size * sizeof(char*));
    const int total = size;
    
    while(n > 0) {

        const int i = n % 1000;        
        
        str[--size] = malloc(4 * sizeof(char));
        
        if(n >= 1000)
            sprintf(str[size], "%03d", i);
        else
            sprintf(str[size], "%d", i);
        
        n /= 1000u;
    }
    
    queue_t *q = queue_ctor();
    
    for(int i = 0; i < total; i++) {
        
        queue_put(q, 1, str[i]);
        free(str[i]);
        
        if(i != total - 1)
            queue_put(q, 1, ".");
    }
    
    char *res = queue_str(q);
    
    free(str);
    queue_dtor(q);
    
    return res;
}

int htoi(const char *input) {

    unsigned int len = strlen(input);
    int dec = 0; int base = 1; int end = 0;

    if(len > 1) {

        char *sub = substr(input, 0, 1);

        if(strcmp(sub, "0x") == 0)
            end += 2;

        free(sub);
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

char* itoh(const int num) {

    char tmp[512];
    sprintf(tmp, "%x", num);

    const int len = strlen(tmp);

    char *hex = malloc((len + 1) * sizeof(char));
    strncpy(hex, tmp, len);
    hex[len] = '\0';

    return hex;
}

void to_hex(int8_t input, char *buffer) {

    int dec = 0; int i = 0;
    buffer[0] = buffer[1] = '0';

    for(int i = 0; i < 8; i++)
        dec |= ((1 << i) & input);

    while(dec != 0) {

        int res = (dec % 16);

        if(res < 10)
            buffer[i] = (res + 48);
        else
            buffer[i] = (res + 87);

        i += 1;
        dec /= 16;
    }

    char temp = buffer[0];

    buffer[0] = buffer[1];
    buffer[1] = temp;
    buffer[2] = '\0';
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
