#include "string_helpers.h"
#include <stdlib.h>
#include <string.h>

char *new_string(int size) {
    return calloc(size + 1, 1);
}

char *concat(char *str1, char *str2) {
    int size = strlen(str1) + strlen(str2) + 1;
    char *ret = calloc(size, 1);
    
    strcat(ret, str1);
    strcat(ret, str2);
    free(str1);
    free(str2);

    return ret;
}

void destroy_string(char *str) {
    free(str);
}

int is_digit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

int is_letter(char c) {
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }

    return 0;
}